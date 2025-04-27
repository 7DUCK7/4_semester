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
    std::vector<sf::Sprite *> nearby_collidble_sprites_ptr_vect;
    int i = 0;
    bool collision_took_place = false;
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {   
        float x_depth, y_depth;
        std::pair<int, int> tank_position_ralatively_to_map_sprite = {-1, -1};
        
        sf::Sprite current_tank_sprite = *(*t)->get_tank_sprite_ptr();
        get_nearby_collideble_map_sprites(&nearby_collidble_sprites_ptr_vect, (*t)->get_tank_sprite_ptr(), my_map);

        //(*t)->move(base_tank_speed);
        sf::FloatRect tank_bounds = (*t)->get_tank_sprite_ptr()->getGlobalBounds();
        for(auto m = nearby_collidble_sprites_ptr_vect.begin(); m != nearby_collidble_sprites_ptr_vect.end(); m++)
        {
            
            sf::Sprite current_map_sprite = *(*m);
            if(current_tank_sprite.getGlobalBounds().intersects(current_map_sprite.getGlobalBounds()))
            {
                collision_took_place = true;
                sf::FloatRect map_bounds = (*m)->getGlobalBounds();
                x_depth = std::max(std::abs(std::min(tank_bounds.left + tank_bounds.width - map_bounds.left, map_bounds.left + map_bounds.width - tank_bounds.left)), x_depth);
                y_depth = std::max(std::abs(std::min(tank_bounds.top + tank_bounds.width - map_bounds.top, map_bounds.top + map_bounds.width - tank_bounds.top)), y_depth);
                if(tank_bounds.left < map_bounds.left)
                {
                    tank_position_ralatively_to_map_sprite.first = LEFT;
                }
                else 
                {
                    tank_position_ralatively_to_map_sprite.first = RIGHT;
                }
                if(tank_bounds.top > map_bounds.top)
                {
                    tank_position_ralatively_to_map_sprite.second = DOWN;   
                }
                else
                {  
                    tank_position_ralatively_to_map_sprite.second = UP;
                }
                set_movement_right(i, 0);
                break;
            } 
            else
            {
                set_movement_right(i, 1 * get_movement_right(i)); //!!!Будем проверять коллизию с объектами после проверки коллизии с границей экрана
            }
        }

        if(collision_took_place)
        {
            sf::Vector2f buf_position = (*t)->get_tank_sprite_ptr()->getPosition();
            if(x_depth < y_depth)
            {
                if(tank_position_ralatively_to_map_sprite.first == LEFT)
                {
                    (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x - x_depth - EPSILON, buf_position.y);
                }
                else
                {
                    (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x + x_depth + EPSILON, buf_position.y);
                }
            }
            else
            {
                if(tank_position_ralatively_to_map_sprite.second == UP)
                {
                    (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x, buf_position.y - y_depth - EPSILON);
                }
                else
                {
                    (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x, buf_position.y + y_depth + EPSILON);
                }
            }
        }
        
        i++;
    }
}

void Model::manage_borders_collision()
{
    int map_size = block_size * MAP_SIZE;
    int i = 0;
    sf::Vector2f old_coords;
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {
        (*t)->move(base_tank_speed);
        std::array<sf::Vector2f, 4> buf_corners_array = get_sprite_corners((*t)->get_tank_sprite_ptr());
        if(std::round(buf_corners_array[TL].y) < 0 || std::round(buf_corners_array[TR].x > map_size) ||
        std::round(buf_corners_array[BR].y) > map_size || std::round(buf_corners_array[BL].x) < 0)
        {
            set_movement_right(i, 0);
        }
        else
        {
            set_movement_right(i, 1);   
        }
        (*t)->move(-base_tank_speed);
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

void Model::update(Map * my_map)
{
    manage_borders_collision();
    manage_walls_collsion(my_map);
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
    //это просто прямоугольник, ассоциируемый со спрайтом, независимо от его поворота
    sf::FloatRect bounds = current_sprite->getGlobalBounds();
    sf::Vector2f topLeft = {bounds.left, bounds.top};
    sf::Vector2f topRight = {bounds.left + bounds.width, bounds.top};
    sf::Vector2f bottomLeft = {bounds.left, bounds.top + bounds.height};
    sf::Vector2f bottomRight = {bounds.left + bounds.width, bounds.top + bounds.height};
    return {topLeft, topRight, bottomLeft, bottomRight};
}

void Model::get_nearby_collideble_map_sprites(std::vector<sf::Sprite *> * nearby_collidble_sprites_ptr_vect, sf::Sprite * tank_sprite, Map * my_map)
{
    nearby_collidble_sprites_ptr_vect->clear();
    std::array<sf::Vector2f, 4> buf_array = get_sprite_corners(tank_sprite);
    sf::Vector2f tank_sprite_TL = buf_array[TL];

    for(int x = 0; x < MAP_SIZE; x++)
    {
        for(int y = 0; y < MAP_SIZE; y++)
        {
            Block * current_block_ptr = my_map->get_block_ptr(y, x);
            if( ((x * block_size - tank_sprite_TL.x) * (x *  block_size - tank_sprite_TL.x) +
                (y *  block_size - tank_sprite_TL.y) * (y *  block_size - tank_sprite_TL.y) < 2 * 4 *block_size * 4 * block_size + 0.5)
                && current_block_ptr->is_block_collidable())
            {
                if(current_block_ptr->get_block_type() != 'b')
                {
                    nearby_collidble_sprites_ptr_vect->push_back(current_block_ptr->get_block_sprite_ptr());
                }
                else
                {
                    for(int i = 0; i < 2; i++)
                    {
                        for(int j = 0; j < 2; j++)
                        {
                            nearby_collidble_sprites_ptr_vect->push_back(current_block_ptr->get_sub_block_ptr(i, j)->get_sub_block_sprite_ptr());
                        }
                    }
                }
            }

        }
    }
}