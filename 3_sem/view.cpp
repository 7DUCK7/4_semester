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

void View::setcontroller(Controller * C)
{
    my_controller = C;
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
    draw();
    my_controller->wait_input(my_model);
    my_model->update(size_of_screen);
    while(!finish)
    {
        drawApples();
        drawSnake(); 
        std::this_thread::sleep_for(std::chrono::nanoseconds(TIMEOUT - my_controller->wait_input(my_model)));
        my_model->update(size_of_screen);
    }
    game_over();
    view_scoreboard();
    my_controller->disable_custom_controller();
    return;
}

void TView::draw()
{
    clrscr();
    draw_wall();
    std::pair<int, int> center_of_screen(size_of_screen.first - 3 / 2, size_of_screen.second / 2);
    gotoxy(center_of_screen);
    setcolor(RED);
    drawSnake();
    fflush(stdout);
    setcolor(WHITE);
    
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
        std::cout << "*";
        fflush(stdout);
    }
    return;
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