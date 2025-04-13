#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <list>

enum Directions
{
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
};

#define TIMEOUT 2*100000000
class Model;
class Apple;
class Snake;

class Controller
{
    protected:
    int snake_number;
    public:
    virtual void process_input(char c, Model * my_model, std::pair<int, int> screen_size, int snake_ind) = 0;
    static Controller *getController(const char cont_type);
};

class WASD_Controller:Controller
{
    private:

    public:
    WASD_Controller();
    virtual void process_input(char c, Model * my_model, std::pair<int, int> screen_size, int snake_ind);
};

class IJKL_Controller:Controller
{
    private:

    public:
    IJKL_Controller();
    virtual void process_input(char c, Model * my_model, std::pair<int, int> screen_size, int snake_ind);
};



class Dumb_Bot_Controller:Controller
{
    private:
        Apple * current_closest_apple;
        void get_closest_apple(Model * my_model);
        void decide_where_to_go(Model * my_model, std::pair <int, int> screen_size);
        void check_nearby_walls(int * walls, Model * my_model, std::pair <int, int> screen_size);
    public:
        Dumb_Bot_Controller();
        virtual void process_input(char c, Model * my_model, std::pair<int, int> screen_size, int snake_ind);
};

class Smart_Bot_Controller:Controller
{
    private:
        Apple * current_closest_apple;
        void get_closest_apple(Model * my_model);
        void decide_where_to_go(Model * my_model, std::pair <int, int> screen_size);
        void check_nearby_walls(int * walls, Model * my_model, std::pair <int, int> screen_size);
    public:
        Smart_Bot_Controller();
        virtual void process_input(char c, Model * my_model, std::pair<int, int> screen_size, int snake_ind);
};

#endif
