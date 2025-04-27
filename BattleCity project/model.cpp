#include "model.h"
#include "sprites.h"
#include <iostream>

Model::Model()
{
    add_tank_to_vect(new Tank());   //рубрыка эксперименты: просто добавим один танк и попробуем покататься по карте
    tanks[0]->set_tank_type(TANK_GREEN);
    tanks[0]->set_tank_health(1);
    tanks[0]->set_tank_speed(1);
    std::cout << tanks.size() << std::endl;
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

std::vector<Tank *> Model::get_tanks_vect()
{
    return tanks;
}