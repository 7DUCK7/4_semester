#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>

class Tank
{
    private:
    int tank_type;  //-1 - first player; -2 - second player; 1 - enemy_1; 2 - enemy_2; 3 - enemy_3; 4 - enemy_4
    int tank_health;
    int tank_speed;
    int tank_fire_rate;
    int tank_damage;
    int tank_bullet_speed;
    int direction;
    sf::Sprite * tank_sprite_ptr;

    public:
    Tank();
    int get_tank_type();
    void set_tank_type(int n);
    int get_tank_health();
    void set_tank_health(int n);
    int get_tank_speed();
    void set_tank_speed(int n);
    int get_tank_fire_rate();
    void set_tank_fire_rate(int n);
    int get_tank_damage();
    void set_tank_bullet_speed(int n);
    sf::Sprite * get_tank_sprite_ptr();
    void set_tank_sprite_ptr(sf::Sprite * sp);
    int get_direction();
    void set_direction(int n);
    void move(int base_speed);
    void rotate();
};


#endif