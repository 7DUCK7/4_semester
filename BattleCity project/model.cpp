#include "model.h"
#include "sprites.h"
#include <iostream>

Model::Model()
{
    add_tank_to_vect(new Tank());   //рубрыка эксперименты: просто добавим один танк и попробуем покататься по карте
    tanks[0]->set_tank_type(TANK_GREEN);
    tanks[0]->set_tank_health(1);
    tanks[0]->set_tank_speed(1);
    movement_rights.push_back(0);
}

bool Model::check_collision(sf::Sprite sprite1, sf::Sprite sprite2)
{
    if(sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds()))
        return true;
    return false;
}

void Model::manage_walls_collsion(Map * my_map)
{

}

void Model::manage_borders_collision()
{
    int map_size = block_size * MAP_SIZE;
    int i = 0;
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {
        (*t)->move(base_tank_speed);
        std::array<sf::Vector2f, 4> buf_corners_array = get_sprite_corners((*t)->get_tank_sprite_ptr());
        if(std::round(buf_corners_array[TL].y) < 0 || std::round(buf_corners_array[TR].x >= map_size) ||
        std::round(buf_corners_array[BR].y) >= map_size || std::round(buf_corners_array[BL].x) < 0)
        {
            set_movement_right(i, 0);
            (*t)->move(-base_tank_speed);
        }
        else
        {
            set_movement_right(i, 1);
        }
        i++;
    }
}

void Model::rotate_and_move_tanks()
{
    //rotation
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {
        (*t)->rotate();
    }
    //movement
    int i = 0;
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {
        if(get_movement_right(i) == 1)
        {
            (*t)->move(base_tank_speed);
        }
        i++;
    }
}

std::vector<Tank *>* Model::get_tanks_vect_ptr()
{
    return &tanks;
}
std::vector<Block *>* Model::get_walls_vect_ptr()
{
    return &walls;
}
void Model::add_tank_to_vect(Tank* t)
{
    tanks.push_back(t);
    return;
}

void Model::set_block_size(int n)
{
    block_size = n;
    return;
}

void Model::set_sub_block_size(int n)
{
    sub_block_size = n;
    return;
}

void Model::set_tank_size(int n)
{
    tank_size = n;
    return;
}

int Model::get_block_size()
{
    return block_size;
}
int Model::get_sub_block_size()
{
    return sub_block_size;
}
int Model::get_tank_size()
{
    return tank_size;
}

void Model::set_base_tank_speed(int n)
{
    base_tank_speed = n;
    return;
}
void Model::set_base_bullet_speed(int n)
{
    base_bullet_speed = n;
    return;
}

int Model::get_base_tank_speed()
{
    return base_tank_speed;
}

int Model::get_base_bullet_speed()
{
    return base_bullet_speed;
}

std::vector<Tank *> Model::get_tanks_vect()
{
    return tanks;
}

void Model::update()
{
    manage_borders_collision();
    rotate_and_move_tanks();
}

int Model::get_movement_right(int n)
{
    return movement_rights[n];
}

void Model::set_movement_right(int n, int value)
{
    movement_rights[n] = value;
    return;
}

std::array<sf::Vector2f, 4> Model::get_sprite_corners(sf::Sprite * current_sprite)
{
    //это просто прямоугольник ассоциируемый со спрайтом независимо от его поворота
    sf::FloatRect bounds = current_sprite->getGlobalBounds();
    sf::Vector2f topLeft = {bounds.left, bounds.top};
    sf::Vector2f topRight = {bounds.left + bounds.width, bounds.top};
    sf::Vector2f bottomLeft = {bounds.left, bounds.top + bounds.height};
    sf::Vector2f bottomRight = {bounds.left + bounds.width, bounds.top + bounds.height};
    return {topLeft, topRight, bottomLeft, bottomRight};
}