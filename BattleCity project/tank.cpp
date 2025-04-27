#include "tank.h"
#include "model.h"

Tank::Tank()
{

}

int Tank::get_tank_type()
{
    return tank_type;
}
void Tank::set_tank_type(int n)
{
    tank_type = n;
    return;
}
int Tank::get_tank_health()
{
    return tank_health;
}
void Tank::set_tank_health(int n)
{
    tank_health = n;
    return;
}
int Tank::get_tank_speed()
{
    return tank_speed;
}
void Tank::set_tank_speed(int n)
{
    tank_speed = n;
    return;
}
int Tank::get_tank_fire_rate()
{
    return tank_fire_rate;
}
void Tank::set_tank_fire_rate(int n)
{
    tank_fire_rate = n;
    return;
}
int Tank::get_tank_damage()
{
    return tank_damage;
}
void Tank::set_tank_bullet_speed(int n)
{
    tank_damage = n;
    return;
}
sf::Sprite * Tank::get_tank_sprite_ptr()
{
    return tank_sprite_ptr;
}
void Tank::set_tank_sprite_ptr(sf::Sprite * sp)
{
    tank_sprite_ptr = sp;
    return;
}

int Tank::get_direction()
{
    return direction;
}

void Tank::set_direction(int n)
{
    direction = n;
    return;
}

void Tank::move(int base_speed)
{
    switch (direction)
    {
    case UP:
        tank_sprite_ptr->move(0.f, (float) -base_speed * tank_speed);
        break;
    case DOWN:
        tank_sprite_ptr->move(0.f, (float) base_speed * tank_speed);
        break;    
    case RIGHT:
        tank_sprite_ptr->move((float) base_speed * tank_speed, 0.f);
        break;
    case LEFT:
        tank_sprite_ptr->move((float) -base_speed * tank_speed, 0.f);
        break;
    default:
        break;
    }
    return;
}

void Tank::rotate()
{
    switch (direction)
    {
    case UP:
        if(tank_sprite_ptr->getRotation() != 0.f)
            tank_sprite_ptr->setRotation(0.f);
        break;

    case DOWN:
        if(tank_sprite_ptr->getRotation() != 180.f)
            tank_sprite_ptr->setRotation(180.f);
        break;
    case RIGHT:
        if(tank_sprite_ptr->getRotation() != 90.f)
            tank_sprite_ptr->setRotation(90.f);
        break;
    case LEFT:
        if(tank_sprite_ptr->getRotation() != 270.f)
            tank_sprite_ptr->setRotation(270.f);
        break;
    default:
        break;
    }
    return;
}