#include "tank.h"

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