#ifndef VIEW_H
#define VIEW_H

#include <list>

enum Color
{
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    PURPLE = 35,
    CYAN = 36,      //голубой
    WHITE = 37
};

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
        std::pair<int, int> size_of_screen;
        int finish = 0;
        void clrscr();
        int gotoxy(std::pair<int, int> coords);
        void setcolor(int color);
        void draw_wall();
        void drawSnake();
        void drawApples();
        void game_over();
        void view_scoreboard();
    public:
        TView(int width, int height)
        {
            size_of_screen.first = width;
            size_of_screen.second = height;
        };
        virtual void run();
        virtual void draw();
};

#endif