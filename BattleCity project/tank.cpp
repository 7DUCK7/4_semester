#include "tank.h"
#include "model.h"
#include "bullet.h"

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

float Tank::get_tank_reload_time()
{
    return tank_reload_time;;
}

void Tank::set_tank_reload_time(float n)
{
    tank_reload_time = n;
    return;
}

int Tank::get_tank_damage()
{
    return tank_damage;
}

void Tank::set_tank_damage(int n)
{
    tank_damage = n;
    return;
}

int Tank::get_tank_bullet_power()
{
    return tank_bullet_power;
}

void Tank::set_tank_bullet_power(int n)
{
    tank_bullet_power = n;
    return;
}

void Tank::set_tank_bullet_speed(float n)
{
    tank_bullet_speed = n;
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

void Tank::set_need_to_shoot_par(bool n)
{
    need_to_shoot = n;
    return;
}

bool Tank::get_need_to_shoot_par()
{
    return need_to_shoot;
}

void Tank::set_ready_to_shoot_par(bool n)
{
    ready_to_shoot = n;
    return;
}

bool Tank::get_ready_to_shoot_par()
{
    return ready_to_shoot;
}

void Tank::shoot(Sprites * my_sprites, int bullet_size)
{
    if(reload_clock.getElapsedTime() < sf::seconds(tank_reload_time))
    {
        need_to_shoot = false;
        return;
    }
    else
        reload_clock.restart();
    Bullet * buf_bullet_ptr = new Bullet(); //добавили новую пулю, принадлежащую этому танку
    add_bullet_ptr_to_vect(buf_bullet_ptr);
    buf_bullet_ptr->set_bullet_speed(tank_bullet_speed);//выставили базовые характеристики пули, исходя из характеристик танка
    buf_bullet_ptr->set_bullet_damage(tank_damage);
    buf_bullet_ptr->set_bullet_power(tank_bullet_power);
    
    sf::Sprite * buf_sprite_ptr = my_sprites->get_new_sprite_ptr(BULLET);//добавили спрайт для пули
    
    //повернули спрайт в сторону движения пули
    float current_tank_rotation = tank_sprite_ptr->getRotation();
    if(current_tank_rotation == 0.f)
    {
        buf_sprite_ptr->setRotation(0.f);
        buf_bullet_ptr->set_bullet_direction(UP);
    }
    else if(current_tank_rotation == 180.f)
    {
        buf_sprite_ptr->setRotation(180.f);
        buf_bullet_ptr->set_bullet_direction(DOWN);
    }
    else if(current_tank_rotation == 90.f)
    {
        buf_sprite_ptr->setRotation(90.f);
        buf_bullet_ptr->set_bullet_direction(RIGHT);
    }
    else if(current_tank_rotation == 270.f)
    {
        buf_sprite_ptr->setRotation(270.f);
        buf_bullet_ptr->set_bullet_direction(LEFT);
    }

    //теперь выставляем кооридинаты спавна пули
    sf::FloatRect tank_bounds = tank_sprite_ptr->getGlobalBounds();
    buf_sprite_ptr->setOrigin(bullet_size / 2, bullet_size / 2);
    if(current_tank_rotation == 0.f)
    {
        buf_sprite_ptr->setPosition(tank_bounds.left + tank_bounds.width / 2, tank_bounds.top - bullet_size / 2);
    }
    else if(current_tank_rotation == 180.f)
    {
        buf_sprite_ptr->setPosition(tank_bounds.left + tank_bounds.width / 2, tank_bounds.top + tank_bounds.height + bullet_size / 2);
    }
    else if(current_tank_rotation == 90.f)
    {
        buf_sprite_ptr->setPosition(tank_bounds.left + tank_bounds.width + bullet_size / 2, tank_bounds.top + tank_bounds.height / 2);
    }
    else if(current_tank_rotation == 270.f)
    {
        buf_sprite_ptr->setPosition(tank_bounds.left - bullet_size / 2, tank_bounds.top + tank_bounds.height / 2);
    }

    
    buf_bullet_ptr->set_bullet_sprite_ptr(buf_sprite_ptr);

    //sf::Vector2f bullet_coords = (*(--bullet_ptr_vect.end()))->get_bullet_sprite_ptr()->getPosition();
    //std::cout << "printing coords of bullet that has been shot " << 
    
    
    need_to_shoot = false;
    ready_to_shoot = false;
}

void Tank::add_bullet_ptr_to_vect(Bullet * b)
{
    bullet_ptr_vect.push_back(b);
    return;
}

std::vector<Bullet*>* Tank::get_bullet_ptr_vect()
{
    return &bullet_ptr_vect;
}

sf::Clock * Tank::get_reload_clock_ptr()
{
    return &reload_clock;
}

bool Tank::is_ready_to_shoot()
{
    return ready_to_shoot;
}


void Tank::set_need_to_move_par(bool n)
{
    need_to_move = n;
    return;
}

bool Tank::get_need_to_move_par()
{
    return need_to_move;
}
/*зарождение будущей жизни
void Tank::set_can_move_par(bool n)
{
    can_move = n;
    return;
}

bool Tank::get_can_move_par()
{
    return can_move;
}
    */