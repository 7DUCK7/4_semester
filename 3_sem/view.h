#ifndef VIEW_H
#define VIEW_H

#include <list>

class Model;
class Controller;

class View
{
    protected:
    Model * my_model;
    Controller * my_controller;
    public:
    virtual void draw() = 0;
    virtual void run() = 0;
    static View *getView(const char view_type);
    void setmodel(Model * A);
    void setcontroller(Controller * C);
};

class TView:View
{
    private:
        int size_of_screen;
        int finish = 0;
        void clrscr();
        int gotoxy(std::pair<int, int> coords);
        void setcolor(int color);
        void draw_wall();
        void drawSnake();
        void game_over();
    public:
        TView(int size)
        {
            size_of_screen = size;
        };
        virtual void run();
        virtual void draw();
};

#endif