#include "tank.h"
#include "model.h"
#include "bullet.h"

Tank::Tank(int type)
{
    tank_type = type;

    switch (type)
    {
    case TANK_GREEN:
        tank_health = 1;
        tank_speed = 1.f;
        tank_bullet_speed = 1.f;
        tank_reload_time = 1.f;
        tank_bullet_power = 1;
        tank_damage = 1;
        is_alive = 1;
        direction = UP;
        break;

    case TANK_YELLOW:
        tank_health = 1;
        tank_speed = 1.f;
        tank_bullet_speed = 1.f;
        tank_reload_time = 1.f;
        tank_bullet_power = 1;
        tank_damage = 1;
        is_alive = 1;
        direction = UP;
        break;

    case TANK_ENEMY_1:
        tank_health = 1;
        tank_speed = 0.8f;
        tank_bullet_speed = 1.f;
        tank_reload_time = 1.2f;
        tank_bullet_power = 1;
        tank_damage = 1;
        is_alive = 0;
        direction = DOWN;
        break;

    case TANK_ENEMY_2:
        tank_health = 1;
        tank_speed = 1.2f;
        tank_bullet_speed = 1.f;
        tank_reload_time = 1.2f;
        tank_bullet_power = 1;
        tank_damage = 1;
        is_alive = 0;
        direction = DOWN;
        break;

    case TANK_ENEMY_3:
        tank_health = 1;
        tank_speed = 0.8f;
        tank_bullet_speed = 1.2f;
        tank_reload_time = 1.f;
        tank_bullet_power = 1;
        tank_damage = 1;
        is_alive = 0;
        direction = DOWN;
        break;
    default:
        break;
    }
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
float Tank::get_tank_speed()
{
    return tank_speed;
}
void Tank::set_tank_speed(float n)
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
    previos_position = tank_sprite_ptr->getPosition();
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
    if(reload_clock.getElapsedTime() < sf::seconds(tank_reload_time) && bullet_ptr_vect.size() > 0)
    {
        need_to_shoot = false;
        return;
    }
    else if(reload_clock.getElapsedTime() < sf::seconds(tank_reload_time / 3) && bullet_ptr_vect.size() == 0 && (tank_type == TANK_GREEN || tank_type == TANK_YELLOW))
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
    buf_bullet_ptr->set_bullet_owner_tank_type(tank_type);
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

sf::Clock * Tank::get_decision_timer_ptr()
{
    return &decision_timer;
}
sf::Clock * Tank::get_forward_movement_timer_ptr()
{
    return &forward_movement_timer;
}

sf::Clock * Tank::get_stun_timer()
{
    return &stun_timer;
}

sf::Clock * Tank::get_not_moving_timer_ptr()
{
    return &not_moving_timer;
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

bool Tank::get_is_alive_par()
{
    return is_alive;
}
void Tank::set_is_alive_par(bool n)
{
    is_alive = n;
    return;
}

bool Tank::get_is_stunned_par()
{
    return is_stunned;
}
void Tank::set_is_stunned_par(bool n)
{
    is_stunned = n;
    return;
}

sf::Vector2f * Tank::get_previos_postion()
{
    return &previos_position;
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