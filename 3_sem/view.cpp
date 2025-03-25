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

/*
Цвета:
30 — черный
31 — красный
32 — зеленый
33 — желтый
34 — синий
35 — пурпурный
36 — голубой
37 — белый
*/

View * View::getView(const char view_type)
{
    if(view_type == 't')
    {
        View *v = (View *) new TView(20);
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
    if(coords.first < 2 || coords.second < 2 || coords.first > size_of_screen - 1 || coords.second > size_of_screen - 1)
    {
        return -1;
    }
    std::cout << "\033[" << size_of_screen - coords.second + 1 << ";" << coords.first << "H";
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
    for(int i = 0; i < size_of_screen; i++)
    {
        std::cout << "@";
    }
    
    for(int i = 2; i <= size_of_screen; i++)
    {
        std::cout << "\033[" << i << ";" << 1 << "H";
        std::cout << "@";
    }

    for(int i = 1; i < size_of_screen; i++)
    {
        std::cout << "@";
    }
    
    for(int i = 2; i < size_of_screen; i++)
    {
        std::cout << "\033[" << i << ";" << size_of_screen << "H";
        std::cout << "@";
    }
    fflush(stdout);
    return;
}

void TView::run()
{   
    draw();
    my_controller->wait_input(my_model);
    my_model->update();
    while(!finish)
    {
        drawSnake();
        std::this_thread::sleep_for(std::chrono::nanoseconds(5*100000000 - my_controller->wait_input(my_model)));
        my_model->update();
    }
    my_controller->disable_custom_controller();
    return;
}

void TView::draw()
{
    clrscr();
    draw_wall();
    std::pair<int, int> center_of_screen(size_of_screen - 3 / 2, size_of_screen / 2);
    gotoxy(center_of_screen);
    setcolor(31);
    drawSnake();
    fflush(stdout);
    setcolor(37);
    
    return;
}

void TView::drawSnake()
{
    setcolor(31);
    std::list<Snake>* current_snakes = my_model->get_snakes();
    int is_anyone_alive = 0;
    for(auto it = current_snakes->begin(); it != current_snakes->end(); it++)
    {
        if(it->is_alive())
            is_anyone_alive = 1;
    }

    if(is_anyone_alive == 0)
        game_over();

    //let's draw snakes' heads
    for(auto it = current_snakes->begin(); it != current_snakes->end(); it++)
    {
        std::list<Segment>* list_of_segments_to_set_head = it->get_segments_to_set_head();
        for(auto s = list_of_segments_to_set_head->begin(); s != list_of_segments_to_set_head->end(); s++)
        {
            if(gotoxy(s->get_coords()) < 0) //если голова змейки окажется вне поля, то "убиваем её", не рисуя головы
            {
                it->death();
                break;
            }
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
    }

    //let's draw body parts
    for(auto it = current_snakes->begin(); it != current_snakes->end(); it++)
    {
        std::list<Segment>* list_of_segments_to_set_body = it->get_segments_to_set_body();
        for(auto s = list_of_segments_to_set_body->begin(); s != list_of_segments_to_set_body->end(); s++)
        {
            gotoxy(s->get_coords());
            std::cout << '0';
        }
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
        it->clear_to_do_lists();
    }
    fflush(stdout);
    setcolor(37);
    return;
}

void TView::game_over()
{
    clrscr();
    gotoxy(std::pair (size_of_screen / 2, size_of_screen / 2));
    setcolor(33);
    std::cout << "GAME OVER";
    fflush(stdout);
    sleep(5);
    finish = 1;
    clrscr();
    return;
}