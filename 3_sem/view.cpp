#include "view.h"
#include "model.h"
#include "controller.h"

#include <thread>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

View * View::getView(const char view_type)
{
    struct winsize buf = {0, 0, 0, 0};
    ioctl(0, TIOCGWINSZ, &buf);
    if(view_type == 't')
    {
        View *v = (View *) new TView(buf.ws_col, buf.ws_row);
        return v;
    }
    return NULL;
}

void View::setmodel(Model * A)
{
    my_model = A;
    return;
}

void View::set_first_player_controller(Controller * C)
{
    first_player_controller = C;
    return;
}

void View::set_second_player_controller(Controller * C)
{
    second_player_controller = C;
    return;
}

void View::set_dumb_bot_controller(Controller * C)
{
    dumb_bot_controller = C;
    return;
}
void View::set_smart_bot_controller(Controller * C)
{
    smart_bot_controller = C;
    return;
}

void TView::clrscr()
{
    std::cout << "\033[H\033[J";
    return;
}

int TView::gotoxy(std::pair<int, int> coords)
{
    if(coords.first < 2 || coords.second < 2 || coords.first > size_of_screen.first - 1 || coords.second > size_of_screen.second - 1)
    {
        std::cout << "\033[" << size_of_screen.second - coords.second + 1 << ";" << coords.first << "H";
        return -1;
    }
    std::cout << "\033[" << size_of_screen.second - coords.second + 1 << ";" << coords.first << "H";
    return 0;
}

void TView::setcolor(int color)
{
    std::cout << "\033[" << color << "m";
    return;
}

void TView::draw_wall()
{
    clrscr();
    setcolor(CYAN);
    for(int i = 0; i < size_of_screen.first; i++)
    {
        std::cout << "@";
    }
    
    for(int i = 2; i <= size_of_screen.second; i++)
    {
        std::cout << "\033[" << i << ";" << 1 << "H";
        std::cout << "@";
    }

    for(int i = 1; i < size_of_screen.first; i++)
    {
        std::cout << "@";
    }
    
    for(int i = 2; i < size_of_screen.second; i++)
    {
        std::cout << "\033[" << i << ";" << size_of_screen.first << "H";
        std::cout << "@";
    }
    fflush(stdout);
    setcolor(WHITE);
    return;
}

void TView::run()
{   
    set_console_settings();
    my_model->spawn_apples(size_of_screen);
    my_model->spawn_players(size_of_screen);
    my_model->spawn_walls(size_of_screen);
    my_model->spawn_bots(size_of_screen);
    
    draw();
    drawWalls();
    wait_input();
    my_model->update(size_of_screen);
    while(!finish)
    {
        drawApples();
        drawSnake();
        drawWalls();   
        print_current_score();
        gotoxy({1, 1});
        fflush(stdout);
        std::this_thread::sleep_for(std::chrono::nanoseconds(TIMEOUT - wait_input()));
        my_model->update(size_of_screen);
        call_process_inputs_for_bots();
    }
    game_over();
    view_scoreboard();
    disable_custom_console_settings();
    return;
}

void TView::draw()
{
    clrscr();
    draw_wall();
    drawSnake();   
    return;
}

void TView::drawSnake()
{
    std::list<Snake>* current_snakes = my_model->get_snakes();
    int is_anyone_alive = 0;
    for(auto it = current_snakes->begin(); it != current_snakes->end(); it++)
    {
        if(it->is_alive())
            is_anyone_alive = 1;
    }

    if(is_anyone_alive == 0)
        finish = 1;

    //let's draw snakes' heads
    for(auto it = current_snakes->begin(); it != current_snakes->end(); it++)
    {
        setcolor(it->get_color());
        std::list<Segment>* list_of_segments_to_set_head = it->get_segments_to_set_head();
        for(auto s = list_of_segments_to_set_head->begin(); s != list_of_segments_to_set_head->end(); s++)
        {
            gotoxy(s->get_coords());
            char buf_char = it->get_direction();
            char head_symbol;
            switch (buf_char)
            {
            case 'r':
                head_symbol = '>';
                break;
            
            case 'f':
                head_symbol = '^';
                break;

            case 'l':
                head_symbol = '<';
                break;

            case 'b':
                head_symbol = 'v';
                break;
            }
            std::cout << head_symbol;
        }
        setcolor(WHITE);
    }

    //let's draw body parts
    for(auto it = current_snakes->begin(); it != current_snakes->end(); it++)
    {
        setcolor(it->get_color());
        std::list<Segment>* list_of_segments_to_set_body = it->get_segments_to_set_body();
        for(auto s = list_of_segments_to_set_body->begin(); s != list_of_segments_to_set_body->end(); s++)
        {
            gotoxy(s->get_coords());
            std::cout << '0';
        }
        setcolor(WHITE);
    }

    //let's delete unnecessary segments
    for(auto it = current_snakes->begin(); it != current_snakes->end(); it++)
    {
        std::list<Segment>* list_of_segments_to_be_deleted = it->get_segments_to_delete();
        for(auto s = list_of_segments_to_be_deleted->begin(); s != list_of_segments_to_be_deleted->end(); s++)
        {
            gotoxy(s->get_coords());
            std::cout << ' ';
        }
        
    }

    //let's clear to do lists as long as we have drawn everything
    for(auto it = current_snakes->begin(); it != current_snakes->end(); it++)
    {
        it->clear_to_do_lists('a');
    }
    fflush(stdout);
    setcolor(WHITE);
    return;
}

void TView::drawApples()
{
    std::list<Apple> * current_apples_list_ptr = my_model->get_apples();
    for(auto ap = current_apples_list_ptr->begin(); ap != current_apples_list_ptr->end(); ap++)
    {
        gotoxy(ap->get_coords());
        std::cout << "*" << std::flush;
    }
    return;
}

void TView::drawWalls()
{
    std::list<Wall> * current_walls_list = my_model->get_walls();
    for(auto w = current_walls_list->begin(); w != current_walls_list->end(); w++)
    {
        //drawing new blocks
        std::list<Segment> * list_of_blocks_to_draw = w->get_segments_to_set_blocks();

        for(auto it = list_of_blocks_to_draw->begin(); it != list_of_blocks_to_draw->end(); it++)
        {
            setcolor(YELLOW);
            gotoxy(it->get_coords());
            std::cout << "#";
            setcolor(WHITE);
        }
        //deleting outdated blocks
        std::list<Segment> * list_of_blocks_to_delete = w->get_blocks_to_delete();
        for(auto it = list_of_blocks_to_delete->begin(); it != list_of_blocks_to_delete->end(); it++)
        {
            gotoxy(it->get_coords());
            std::cout << ' ';
        }        
        fflush(stdout);
        w->clear_all_to_do_lists();
    }
}

void TView::game_over()
{
    clrscr();
    gotoxy(std::pair (size_of_screen.first / 2, size_of_screen.second / 2));
    setcolor(33);
    std::cout << "GAME OVER";
    fflush(stdout);
    sleep(5);
    finish = 1;
    clrscr();
    setcolor(WHITE);
    return;
}
void TView::view_scoreboard()
{
    std::list<Snake> * current_snakes = my_model->get_snakes();
    auto sn = current_snakes->begin();
    for(int i = 1; i <= my_model->get_number_of_players(); i++)
    {
        setcolor(sn->get_color());
        std::cout << "Player " << i << "\033[" << WHITE << "m" << " has eaten " << sn->get_apple_score() << " apple(-s)\r\n";
        sn++;
    }
    std::cout << "Type q to quit\r\n";
    while(getchar() != 'q')
        ;
    clrscr();
    return;
}

long TView::wait_input()
{
    char read_symbol;
    struct timespec start, end;
    long time_wasted = TIMEOUT;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int n = poll(&(fd_in), 1, TIMEOUT / 1000000);
    
    if(n == 1)
    {
        read(0, &read_symbol, 1);
        call_process_inputs(read_symbol);
    }

    tcflush(STDIN_FILENO, TCIFLUSH);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_wasted = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    if(read_symbol == 'q')
    {
        finish = 1;
        return time_wasted;
    }
    return time_wasted;  // наносекунды
}

void TView::set_console_settings()
{
    tcgetattr(STDIN_FILENO, &original);
    raw = original;
    cfmakeraw(&raw);    //переводим терминал в raw режим для посимвольного чтения ввода
    raw.c_lflag &= ~ECHO;   //отключаем ECHO, чтобы ввод в терминал не выводился на экран
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void TView::disable_custom_console_settings()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original);
    return;
}

void TView::call_process_inputs(char c)
{
    switch (my_model->get_number_of_players())
    {
    case 1:
        first_player_controller->process_input(c, my_model, {0, 0}, 0);
        break;
    case 2:
        first_player_controller->process_input(c, my_model, {0, 0}, 0);
        second_player_controller->process_input(c, my_model, {0, 0}, 1 );
        break;
    default:
        break;
    }
}

void TView::call_process_inputs_for_bots()
{
    std::list <Snake> * current_snakes = my_model->get_snakes(); 
    int i = 0;
    for(auto sn = current_snakes->begin(); sn != current_snakes->end(); sn++)
    {
        if(sn->get_controller_type() == 'd')
            dumb_bot_controller->process_input(' ', my_model, size_of_screen, i);
        if(sn->get_controller_type() == 's')
            smart_bot_controller->process_input(' ', my_model, size_of_screen, i);
        i++;
    }   
}

void TView::print_current_score()
{
    
    if(my_model->get_number_of_players() == 2)
    {
        gotoxy({size_of_screen.first / 3 - 8, 1});
        setcolor(my_model->get_snakes()->begin()->get_color());
        std::cout << "Player 1: " << my_model->get_snakes()->begin()->get_apple_score() << std::flush;
        gotoxy({size_of_screen.first / 3 * 2 - 8, 1});
        setcolor((++(my_model->get_snakes()->begin()))->get_color());
        std::cout << "Player 2: " << (++(my_model->get_snakes()->begin()))->get_apple_score() << std::flush;
    }
    else if(my_model->get_number_of_players() == 1)
    {
        gotoxy({size_of_screen.first / 2 - 8, 1});
        setcolor(my_model->get_snakes()->begin()->get_color());
        std::cout << "Player 1: " << my_model->get_snakes()->begin()->get_apple_score() << std::flush;
    }
    setcolor(WHITE);
    return;
}