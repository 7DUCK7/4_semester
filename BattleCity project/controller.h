#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "tank.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include "GUI.h"

class Model;
class Map;
class Sprites;

class Controller
{   
    protected:
    Tank * operating_tank_ptr;

    public:
    virtual void process_input(Model * my_model, Map * my_map) = 0;
    static Controller * get_controller(char contr_type);
    Tank * get_operating_tank_ptr();
    void set_operating_tank_ptr(Tank * t);
};

class First_Player_Controller:Controller
{
    public:
    First_Player_Controller();
    virtual void process_input(Model * my_model, Map * my_map);
};

class Second_Player_Controller:Controller
{
    public:
    Second_Player_Controller();
    virtual void process_input(Model * my_model, Map * my_map);
};

class Enemy_1_Bot_Controller:Controller
{
    private:
    void get_environment(Model * my_model, Map * my_map, int* direction_massive, int * bricks_direction_massive);
    void decide_where_to_go(Model * my_model, Map * my_map);
    public:
    Enemy_1_Bot_Controller();
    virtual void process_input(Model * my_model, Map * my_map);
};

class Enemy_2_Bot_Controller:Controller
{
    private:
    void get_environment(Model * my_model, Map * my_map, int* direction_massive, int * bricks_direction_massive, int * players_direction_massive);
    void decide_where_to_go(Model * my_model, Map * my_map);
    bool forsee_future_not_to_miss_player(Model * my_model, Map * my_map);
    public:
    Enemy_2_Bot_Controller();
    virtual void process_input(Model * my_model, Map * my_map);
};

class Enemy_3_Bot_Controller:Controller
{
    private:
    void get_environment(Model * my_model, Map * my_map, int* direction_massive, int * bricks_direction_massive, int * players_direction_massive);
    void decide_where_to_go(Model * my_model, Map * my_map);
    public:
    Enemy_3_Bot_Controller();
    virtual void process_input(Model * my_model, Map * my_map);
};

#endif