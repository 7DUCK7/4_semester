#ifndef GUI_H
#define GUI_H

#include <SFML/Window.hpp>

#include "map.h"
#include "controller.h"
#include "model.h"

#define FRAME_RATE 90
class Map;
class Controller;
class Model;

class GUI
{
    private:
    int screen_height;
    int screen_width;

    Map * my_map;
    Model * my_model;
    Sprites * my_sprites;
    Controller * first_player_controller;
    Controller * second_player_controller;
    Controller * bot_controller;
    std::pair <int, int> window_size;
    int finish = 0;
    void prepare_tanks();
    void get_window_size_and_manage_sprites_sizes();
    void set_tanks_speeds();
    public:
    GUI(Map * map, Model * model, Sprites * sprites);
    ~GUI()  {};
    void run();
    void set_first_player_controller(Controller * c);
};


#endif 