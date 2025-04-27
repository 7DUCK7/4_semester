#include "controller.h"
#include "tank.h"
#include <iostream>
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

void Controller::set_base_tank_speed(int n)
{
    base_tank_speed = (float)n;
}

void Controller::set_base_bullet_speed(int n)
{
    base_bullet_speed = (float)n;
}

First_Player_Controller::First_Player_Controller()
{

}

void First_Player_Controller::process_input()
{
    
    float tank_sprite_rotation = operating_tank_ptr->get_tank_sprite_ptr()->getRotation();    

    //managing movement and movement
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if(tank_sprite_rotation != 0.f)
            operating_tank_ptr->get_tank_sprite_ptr()->setRotation(0.f);
        
        operating_tank_ptr->get_tank_sprite_ptr()->move(0.f, (float)(-base_tank_speed * operating_tank_ptr->get_tank_speed()));
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        if(tank_sprite_rotation != 180.f)
            operating_tank_ptr->get_tank_sprite_ptr()->setRotation(180.f);

        operating_tank_ptr->get_tank_sprite_ptr()->move(0.f, (float)(base_tank_speed * operating_tank_ptr->get_tank_speed()));
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if(tank_sprite_rotation != 90.f)
            operating_tank_ptr->get_tank_sprite_ptr()->setRotation(90.f);

        operating_tank_ptr->get_tank_sprite_ptr()->move((float)(base_tank_speed * operating_tank_ptr->get_tank_speed()), 0.f);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if(tank_sprite_rotation != 270.f)
            operating_tank_ptr->get_tank_sprite_ptr()->setRotation(270.f);
            
        operating_tank_ptr->get_tank_sprite_ptr()->move((float)(-base_tank_speed * operating_tank_ptr->get_tank_speed()), 0.f);
    }
}