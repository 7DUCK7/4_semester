#include <string.h>
#include "view.h"
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
    return;
}


void TView::clrscr()
{
    std::cout << "\033[H\033[J";
    return;
}

void TView::gotoxy(int x, int y)
{
    if(x < 0 || y < 0 || x > size_of_screen || y > size_of_screen)
        return;
    std::cout << "\033[" << size_of_screen - y << ";" << x << "H";
    return;
}

void TView::setcolor(int color)
{
    std::cout << "\033[" << color << "m";
    return;
}

void TView::draw_wall()
{
    for(int i = 0; i < size_of_screen; i++)
        std::cout << "@@";
    std::cout << "\n";
    for(int i = 1; i < size_of_screen - 1; i++)
        std::cout << "@\n";
    for(int i = 0; i < size_of_screen; i++)
    {
        std::cout << "\033[" << i << ";" << 2 * size_of_screen << "H";
        std::cout << "@";
    }
    std::cout << "\n";
    for(int i = 0; i < size_of_screen; i++)
        std::cout << "@@";
        std::cout << "\n";
    return;
}

void TView::draw()
{
    clrscr();
    draw_wall();
    gotoxy(size_of_screen - strlen("Hello") / 2, size_of_screen / 2);
    setcolor(31);
    std::cout << "Hello";
    setcolor(0);
    getchar();
    clrscr();
    return;
}
