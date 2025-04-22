#ifndef GUI_H
#define GUI_H

#include <SFML/Window.hpp>

#include "map.h"

class Map;
class Controller;

class GUI
{
    private:
    Map * my_map;
    Controller * first_player_controller;
    Controller * second_player_controller;
    Controller * bot_controller;
    std::pair <int, int> window_size;
    int finish = 0;
    
    public:
    GUI(Map * map);
    ~GUI()  {};
    void run();

};


#endif 