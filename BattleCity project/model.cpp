#include "model.h"
#include "sprites.h"
#include "bullet.h"
#include <iostream>

Model::Model()
{
    add_tank_to_vect(new Tank());   //рубрыка эксперименты: просто добавим один танк и попробуем покататься по карте
    tanks[0]->set_tank_type(TANK_GREEN);
    tanks[0]->set_tank_health(1);
    tanks[0]->set_tank_speed(1);
    tanks[0]->set_tank_bullet_speed(1.f);
    tanks[0]->set_tank_reload_time(1.f);
    tanks[0]->set_tank_bullet_power(1);
    tanks[0]->set_tank_damage(1);
    movement_rights.push_back(0);
}

bool Model::check_collision(sf::Sprite sprite1, sf::Sprite sprite2)
{
    if(sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds()))
        return true;
    return false;
}

void Model::manage_walls_collsion(Map * my_map, Sprites * my_sprites)
{
    std::vector<sf::Sprite *> nearby_collidble_sprites_ptr_vect;
    int i = 0;
    bool collision_took_place = false;
    //tanks' collisions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {   
        float x_depth, y_depth;
        float x_depth_sum = 0., y_depth_sum = 0.;
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
                x_depth_sum += std::abs(std::min(tank_bounds.left + tank_bounds.width - map_bounds.left, map_bounds.left + map_bounds.width - tank_bounds.left));
                y_depth_sum += std::abs(std::min(tank_bounds.top + tank_bounds.width - map_bounds.top, map_bounds.top + map_bounds.width - tank_bounds.top));
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
            //std::cout << "raleative position: " << tank_position_ralatively_to_map_sprite.first << ' ' << tank_position_ralatively_to_map_sprite.second << std::endl;
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
            /*  Отложим пока в долгий ящик скругление углов для более простого перемещения
            if(y_depth_sum < 8 * x_depth_sum)
            {
                if(tank_position_ralatively_to_map_sprite.first == LEFT && (*t)->get_direction() == RIGHT)
                {
                    if(tank_position_ralatively_to_map_sprite.second == DOWN)
                        (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x, buf_position.y + y_depth + EPSILON);
                    else
                        (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x, buf_position.y - y_depth - EPSILON);
                }
                else if(tank_position_ralatively_to_map_sprite.first == RIGHT && (*t)->get_direction() == LEFT)
                {
                    if(tank_position_ralatively_to_map_sprite.second == DOWN)
                        (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x, buf_position.y + y_depth + EPSILON);
                    else
                        (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x, buf_position.y - y_depth - EPSILON);
                }
            }
            if(x_depth_sum < 8 * y_depth_sum)
            {
                if(tank_position_ralatively_to_map_sprite.second == DOWN && (*t)->get_direction() == UP)
                {
                    if(tank_position_ralatively_to_map_sprite.first == LEFT)
                        (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x - x_depth - EPSILON, buf_position.y);
                    else
                        (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x + x_depth + EPSILON, buf_position.y);
                }
                else if(tank_position_ralatively_to_map_sprite.second == UP && (*t)->get_direction() == DOWN)
                {
                    if(tank_position_ralatively_to_map_sprite.first == RIGHT)
                        (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x - x_depth - EPSILON, buf_position.y);
                    else
                        (*t)->get_tank_sprite_ptr()->setPosition(buf_position.x + x_depth + EPSILON, buf_position.y);
                }
            }*/
        }
        
        i++;
    }

    //bullets' collisions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    i = 0;
    collision_took_place = false;
    std::vector<Block *> nearby_dense_blocks_ptr_vect;
    std::vector<Bullet *> * current_bullets_ptr_vector;
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {   
        current_bullets_ptr_vector = (*t)->get_bullet_ptr_vect();
        for(auto b = current_bullets_ptr_vector->begin(); b != current_bullets_ptr_vector->end(); b++)
        {
            bool delete_bullet = false;
            get_nearby_dense_map_blocks(&nearby_dense_blocks_ptr_vect, (*b)->get_bullet_sprite_ptr(), my_map);
            for(auto m = nearby_dense_blocks_ptr_vect.begin(); m != nearby_dense_blocks_ptr_vect.end(); m++)
            {
                //разбираемся с некирпичами так как это проще
                if((*m)->get_block_type() != 'b')
                {
                    if((*b)->get_bullet_sprite_ptr()->getGlobalBounds().intersects((*m)->get_block_sprite_ptr()->getGlobalBounds()))
                    {
                        //уменьшаем здоровье блока, если сила пули позволяет наносить этому блоку урон
                        if((*b)->get_bullet_power() >= (*m)->get_block_density())
                        {
                            if((*m)->get_block_health() > (*b)->get_bullet_damage())
                            {
                                (*m)->set_block_health((*m)->get_block_health() - (*b)->get_bullet_damage());
                            }
                            else
                            {   //превращаем блок в воздух
                                (*m)->set_block_health(0);
                                (*m)->block_set_collidable_par(0);
                                (*m)->set_block_density(0);
                                (*m)->set_block_type(' ');
                                //!!!!сохраняя прежнюю позицию спрайта
                                sf::Vector2f old_position = (*m)->get_block_sprite_ptr()->getPosition();
                                delete (*m)->get_block_sprite_ptr();
                                (*m)->set_block_sprite_ptr(my_sprites->get_new_sprite_ptr(AIR));
                                (*m)->get_block_sprite_ptr()->setPosition(old_position.x, old_position.y);
                            }
                        }
                        //ставим флаг на удаление пули 
                        delete_bullet = true;
                    }
                }
                else    //теперь разбираемся с кирпичами (тут уже нужно обработать субблоки)
                {
                    int sum_brick_health = 0;   //посчитаем-ка заодно суммарное здоровье субблоков кирпича, дабы потом в случае нуля заменить блок-кирпич на блок-воздух
                    for(int i = 0; i < 2; i++)
                    {
                        for(int j = 0; j < 2; j++)
                        {   
                            sum_brick_health += (*m)->get_sub_block_ptr(i, j)->get_sub_block_health();
                            //тут к сожалению мы получили все субблоки, а не только c density > 0, поэтому не рассматриваем прозрачные блоки
                            if((*m)->get_sub_block_ptr(i, j)->sub_block_get_density() > 0)
                            {
                                if((*m)->get_sub_block_ptr(i, j)->get_sub_block_sprite_ptr()->getGlobalBounds().intersects((*b)->get_bullet_sprite_ptr()->getGlobalBounds()))
                                {
                                    
                                    //уменьшаем здоровье субблока, если сила пули позвляет наносить этому субблоку урон
                                    //std::cout << __FILE__ << ' ' << __LINE__ << " subblock density == " << (*m)->get_sub_block_ptr(i, j)->sub_block_get_density() << " bullet power == " << (*b)->get_bullet_power() <<  std::endl;
                                    if((*m)->get_sub_block_ptr(i, j)->sub_block_get_density() <= (*b)->get_bullet_power())
                                    {
                                        //std::cout << __FILE__ << ' ' << __LINE__ << " dealing with bricks' subblocks that passed density test" << std::endl;
                                        //std::cout << __FILE__ << ' ' << __LINE__ << " subblock health == " << (*m)->get_sub_block_ptr(i, j)->get_sub_block_health() << " bullet dmg == " << (*b)->get_bullet_damage() <<  std::endl;
                                        if((*m)->get_sub_block_ptr(i, j)->get_sub_block_health() > (*b)->get_bullet_damage())
                                        {
                                            //std::cout << __FILE__ << ' ' << __LINE__ << " dealing with subblock that has more hp than bullet dmg is" << std::endl;
                                            (*m)->get_sub_block_ptr(i, j)->set_sub_block_health((*m)->get_sub_block_ptr(i, j)->get_sub_block_health() - (*b)->get_bullet_damage());    
                                        }                                        
                                        else
                                        {
                                            //std::cout << __FILE__ << ' ' << __LINE__ << " dealing with subblock that has less hp than bullet dmg is" << std::endl;
                                            (*m)->get_sub_block_ptr(i, j)->set_sub_block_health(0);
                                            (*m)->get_sub_block_ptr(i, j)->sub_block_set_density(0);
                                            (*m)->get_sub_block_ptr(i, j)->sub_block_set_collideble_par(false);
                                            sf::Vector2f old_position = (*m)->get_sub_block_ptr(i, j)->get_sub_block_sprite_ptr()->getPosition();
                                            delete (*m)->get_sub_block_ptr(i, j)->get_sub_block_sprite_ptr();
                                            (*m)->get_sub_block_ptr(i, j)->set_sub_block_sprite_ptr(my_sprites->get_new_sprite_ptr(SMALL_AIR));
                                            (*m)->get_sub_block_ptr(i, j)->get_sub_block_sprite_ptr()->setPosition(old_position.x, old_position.y);
                                        }
                                    }
                                    delete_bullet = true;
                                }
                            }
                        }
                    }
                    //если все субблоки кирпича мертвы
                    if(sum_brick_health == 0)
                    {
                        sf::Vector2f old_position = (*m)->get_sub_block_ptr(0, 0)->get_sub_block_sprite_ptr()->getPosition();
                        for(int i = 0; i < 2; i++)
                        {
                            for(int j = 0; j < 2; j++)
                            {   //удаляем спрайты субблоков(запоминая позицию левого верхнего угла)
                                delete (*m)->get_sub_block_ptr(i, j)->get_sub_block_sprite_ptr();                        
                            }
                        }
                        //превращаем кирпич в воздух-блок
                        (*m)->set_block_health(0);
                        (*m)->block_set_collidable_par(0);
                        (*m)->set_block_density(0);
                        (*m)->set_block_type(' ');
                        (*m)->set_block_sprite_ptr(my_sprites->get_new_sprite_ptr(AIR));
                        (*m)->get_block_sprite_ptr()->setPosition(old_position.x, old_position.y);
                    }
                }
            }
            if(delete_bullet)
            {
                //удаляем пулю
                delete *b;
                if(b != (--current_bullets_ptr_vector->end()))
                {
                    b = current_bullets_ptr_vector->erase(b);
                }
                else
                {
                    current_bullets_ptr_vector->erase(b);
                    break;
                }
            }
        }

    }
}

void Model::manage_borders_collision()
{
    int map_size = block_size * MAP_SIZE;
    int i = 0;
    sf::Vector2f old_coords;
    //tanks collision
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
    //let's just delete bullets that went out of the map
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {
        std::vector<Bullet*> * current_bullet_vect_ptr = (*t)->get_bullet_ptr_vect();
        for(auto b = current_bullet_vect_ptr->begin(); b != current_bullet_vect_ptr->end(); b++)
        {
            if((*b)->get_bullet_sprite_ptr() == nullptr)
                perror("tried to get sprite features through nullptr in Model::manage_borders_collision()");
 
            std::array<sf::Vector2f, 4> buf_array = get_sprite_corners((*b)->get_bullet_sprite_ptr());
            
            if( buf_array[TL].y > MAP_SIZE * block_size ||
                buf_array[BR].y < 0 ||
                buf_array[BL].x > MAP_SIZE * block_size ||
                buf_array[TR].x < 0)
            {
                
                if(*b != nullptr)
                    delete *b;
                else
                    perror("tried to delete not nullptr bullet in Model::manage_borders_collision()\n");
                if(b != (--current_bullet_vect_ptr->end()))
                    b = current_bullet_vect_ptr->erase(b);
                else
                {
                    current_bullet_vect_ptr->erase(b);
                    break;
                }
            }
        }
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
        if(get_movement_right(i) == 1 && (*t)->get_need_to_move_par())
        {
            (*t)->move(base_tank_speed);
        }
        (*t)->set_need_to_move_par(false);
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

void Model::update(Map * my_map, Sprites * my_sprites)
{
    all_tanks_shoot(my_sprites);
    manage_borders_collision();
    manage_walls_collsion(my_map, my_sprites);
    rotate_and_move_tanks();
    move_all_bullets();
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

void Model::get_nearby_collideble_map_sprites(std::vector<sf::Sprite *> * nearby_collidble_sprites_ptr_vect, sf::Sprite * sprite, Map * my_map)
{
    nearby_collidble_sprites_ptr_vect->clear();
    std::array<sf::Vector2f, 4> buf_array = get_sprite_corners(sprite);
    sf::Vector2f sprite_TL = buf_array[TL];

    for(int x = 0; x < MAP_SIZE; x++)
    {
        for(int y = 0; y < MAP_SIZE; y++)
        {
            Block * current_block_ptr = my_map->get_block_ptr(y, x);
            if( ((x * block_size - sprite_TL.x) * (x *  block_size - sprite_TL.x) +
                (y *  block_size - sprite_TL.y) * (y *  block_size - sprite_TL.y) < 2 * 4 *block_size * 4 * block_size + 0.5)
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
                            if(current_block_ptr->get_sub_block_ptr(i, j)->sub_block_get_collideble_par())
                                nearby_collidble_sprites_ptr_vect->push_back(current_block_ptr->get_sub_block_ptr(i, j)->get_sub_block_sprite_ptr());
                        }
                    }
                }
            }

        }
    }
}

void Model::get_nearby_dense_map_blocks(std::vector<Block *> * nearby_dense_blocks_ptr_vect, sf::Sprite * sprite, Map * my_map)
{
    nearby_dense_blocks_ptr_vect->clear();
    std::array<sf::Vector2f, 4> buf_array = get_sprite_corners(sprite);
    sf::Vector2f sprite_TL = buf_array[TL];

    for(int x = 0; x < MAP_SIZE; x++)
    {
        for(int y = 0; y < MAP_SIZE; y++)
        {
            Block * current_block_ptr = my_map->get_block_ptr(y, x);
            if( ((x * block_size - sprite_TL.x) * (x *  block_size - sprite_TL.x) +
                (y *  block_size - sprite_TL.y) * (y *  block_size - sprite_TL.y) < 2 * 4 *block_size * 4 * block_size + 0.5)
                && current_block_ptr->get_block_density() > 0)
            {
                nearby_dense_blocks_ptr_vect->push_back(current_block_ptr);
            }
        }
    }
}

void Model::set_bullet_size(int n)
{
    bullet_size = n;
    return;
}

int Model::get_bullet_size()
{
    return bullet_size;
}

void Model::move_all_bullets()
{
    for(auto t = tanks.begin(); t < tanks.end(); t++)
    {
        std::vector<Bullet *>* current_bullet_vector_ptr =  (*t)->get_bullet_ptr_vect();
        for(auto b = current_bullet_vector_ptr->begin(); b != current_bullet_vector_ptr->end(); b++)
            (*b)->move(base_bullet_speed);
    }
}

void Model::all_tanks_shoot(Sprites * my_sprites)
{
    for(auto t = tanks.begin(); t < tanks.end(); t++)
    {
        if((*t)->get_need_to_shoot_par())
            (*t)->shoot(my_sprites, bullet_size);
    }
}