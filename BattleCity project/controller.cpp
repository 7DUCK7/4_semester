#include "controller.h"
#include "tank.h"
#include "model.h"

Controller * Controller::get_controller(char contr_type)
{
    switch (contr_type)
    {
    case '1':   return (Controller*) (new First_Player_Controller());
    //case '2':   return (Controller*) (new Second_Player_Controller());
    //case 'b':   return (Controller*) (new Bot_Controller());
    default:    break;
    }
    return nullptr;
}

Tank * Controller::get_operating_tank_ptr()
{
    return operating_tank_ptr;
}

void Controller::set_operating_tank_ptr(Tank * t)
{
    operating_tank_ptr = t;
    return;
}

First_Player_Controller::First_Player_Controller()
{

}

void First_Player_Controller::process_input()
{ 
    //managing movement and movement
    bool is_space_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool is_w_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool is_s_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool is_d_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool is_a_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    if(is_space_pressed)
    {
        operating_tank_ptr->set_need_to_shoot_par(true);
    }
    if(!is_w_pressed && !is_s_pressed && !is_d_pressed && !is_a_pressed)
        operating_tank_ptr->set_need_to_move_par(false);
    if(is_w_pressed)
    {
        operating_tank_ptr->set_direction(UP);
        operating_tank_ptr->set_need_to_move_par(true);
    }
    else if(is_s_pressed)
    {
        operating_tank_ptr->set_direction(DOWN);
        operating_tank_ptr->set_need_to_move_par(true);
    }
    else if(is_d_pressed)
    {
        operating_tank_ptr->set_direction(RIGHT);
        operating_tank_ptr->set_need_to_move_par(true);
    }
    else if(is_a_pressed)
    {
        operating_tank_ptr->set_direction(LEFT);
        operating_tank_ptr->set_need_to_move_par(true);
    }

}