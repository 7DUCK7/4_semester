#include "bullet.h"
#include "model.h"

Bullet::Bullet()
{
    
}


float Bullet::get_bullet_speed()
{
    return speed;
}

void Bullet::set_bullet_speed(float n)
{
    speed = n;
    return;
}

int Bullet::get_bullet_damage()
{
    return damage;
}

void Bullet::set_bullet_damage(int n)
{
    damage = n;
    return;
}

int Bullet::get_bullet_direction()
{
    return direction;
}

void Bullet::set_bullet_direction(int n)
{
    direction = n;
    return;
}

sf::Sprite * Bullet::get_bullet_sprite_ptr()
{
    return sprite_ptr;
}

void Bullet::set_bullet_sprite_ptr(sf::Sprite * sp)
{
    sprite_ptr = sp;
    return;
}

void Bullet::move(int base_speed)
{
    switch (direction)
    {
    case UP:
        sprite_ptr->move(0.f, (float) -base_speed * speed);
        break;
    case DOWN:
        sprite_ptr->move(0.f, (float) base_speed * speed);
        break;    
    case RIGHT:
        sprite_ptr->move((float) base_speed * speed, 0.f);
        break;
    case LEFT:
        sprite_ptr->move((float) -base_speed * speed, 0.f);
        break;
    default:
        break;
    }
    return;
}

int Bullet::get_bullet_power()
{
    return bullet_power;
}
void Bullet::set_bullet_power(int n)
{
    bullet_power = n;
    return;
}