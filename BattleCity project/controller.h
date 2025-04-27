#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "tank.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>

class Controller
{
    protected:
    Tank * operating_tank_ptr;
    float base_tank_speed;
    float base_bullet_speed;
    public:
    virtual void process_input() = 0;
    static Controller * get_controller(char contr_type);
    Tank * get_operating_tank_ptr();
    void set_operating_tank_ptr(Tank * t);
    void set_base_tank_speed(int n);
    void set_base_bullet_speed(int n);
};

class First_Player_Controller:Controller
{
    public:
    First_Player_Controller();
    virtual void process_input();
};

#endif