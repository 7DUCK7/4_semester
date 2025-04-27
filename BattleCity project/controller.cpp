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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        operating_tank_ptr->set_direction(UP);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        operating_tank_ptr->set_direction(DOWN);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        operating_tank_ptr->set_direction(RIGHT);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        operating_tank_ptr->set_direction(LEFT);
    }
    else 
    {
        operating_tank_ptr->set_direction(NONE);
    }
}