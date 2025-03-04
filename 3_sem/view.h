#include "model.h"

class View
{
    public:
    virtual void draw() = 0;
    static View *getView(const char view_type);
    void setmodel(Model * A);
};

class TView:View
{
    int size_of_screen;
    public:
    TView(int size)
    {
        size_of_screen = size;
    };
    //virtual void run();
    virtual void draw();
    private:
        void clrscr();
        void gotoxy(int x, int y);
        void setcolor(int color);
        void draw_wall();
};