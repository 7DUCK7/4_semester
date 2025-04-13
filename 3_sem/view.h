#ifndef VIEW_H
#define VIEW_H

#include <list>
#include <termios.h>
#include <unistd.h>
#include <poll.h>

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
    Controller * first_player_controller;
    Controller * second_player_controller;
    Controller * dumb_bot_controller;
    Controller * smart_bot_controller;
    public:
    virtual void draw() = 0;
    virtual void run() = 0;
    static View *getView(const char view_type);
    void setmodel(Model * A);
    void set_first_player_controller(Controller * C);
    void set_second_player_controller(Controller * C);
    void set_dumb_bot_controller(Controller * C);
    void set_smart_bot_controller(Controller * C);
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
        void drawWalls();
        void game_over();
        void view_scoreboard();
        long wait_input();
        void set_console_settings();
        void disable_custom_console_settings();
        void call_process_inputs(char c);
        void call_process_inputs_for_bots();
        void print_current_score();

        struct termios original;
        struct termios raw;
        struct pollfd fd_in = {  .fd = STDIN_FILENO, .events = POLLIN};
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