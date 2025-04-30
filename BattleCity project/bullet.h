#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet
{
    private:
    float speed;
    int damage;
    int direction;
    int bullet_power;
    int owner_tank_type;
    sf::Sprite * sprite_ptr;

    public:
    Bullet();
    ~Bullet()
    {
        if(sprite_ptr != nullptr)
        {
            delete sprite_ptr;
            sprite_ptr = nullptr;
        }
    }
    float get_bullet_speed();
    void set_bullet_speed(float n);
    int get_bullet_damage();
    void set_bullet_damage(int n);
    int get_bullet_direction();
    void set_bullet_direction(int n);
    int get_bullet_power();
    void set_bullet_power(int n);
    sf::Sprite * get_bullet_sprite_ptr();
    void set_bullet_sprite_ptr(sf::Sprite * sp);
    void move(int base_speed);
    void set_bullet_owner_tank_type(int n);
    int get_bullet_owner_tank_type();
};









#endif