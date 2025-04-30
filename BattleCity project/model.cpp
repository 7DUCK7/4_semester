#include "model.h"
#include "sprites.h"
#include "bullet.h"
#include <iostream>


Model::Model(int num_of_players)
{
    number_of_players = num_of_players;
    add_tank_to_vect(new Tank(TANK_GREEN));   
    movement_rights.push_back(1);
    if(number_of_players == 2)
    {
        add_tank_to_vect(new Tank(TANK_YELLOW));   
        movement_rights.push_back(1);
    }
}

bool Model::check_collision(sf::Sprite sprite1, sf::Sprite sprite2)
{
    if(sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds()))
        return true;
    return false;
}

void Model::manage_walls_collsion_v2(Map * my_map, Sprites * my_sprites)
{
    std::vector<sf::Sprite *> nearby_collidble_sprites_ptr_vect;
    //tanks' collisions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {   
        if(!(*t)->get_is_alive_par())
            continue;
        get_nearby_collideble_map_sprites(&nearby_collidble_sprites_ptr_vect, (*t)->get_tank_sprite_ptr(), my_map);
        for(auto m = nearby_collidble_sprites_ptr_vect.begin(); m != nearby_collidble_sprites_ptr_vect.end(); m++)
        {
            float const1 = 0., const2 = 0., x_depth = 0.f, y_depth = 0.f;
            if((*m)->getGlobalBounds().intersects((*t)->get_tank_sprite_ptr()->getGlobalBounds()))  //спрайт танка пересёкся со спрайтом карты
            {
                sf::Vector2f sprite_center_coords = {(*m)->getPosition().x + (*m)->getGlobalBounds().width / 2, (*m)->getPosition().y + (*m)->getGlobalBounds().height / 2};
                const1 = ((*m)->getGlobalBounds().width + tank_size) / 2 - ((*t)->get_tank_sprite_ptr()->getPosition().x - sprite_center_coords.x);
                const2 = ((*m)->getGlobalBounds().height + tank_size) / 2 - ((*t)->get_tank_sprite_ptr()->getPosition().y - sprite_center_coords.y);     
                x_depth = ((*m)->getGlobalBounds().width + tank_size) / 2 - std::abs((*t)->get_tank_sprite_ptr()->getPosition().x - sprite_center_coords.x);     
                y_depth = ((*m)->getGlobalBounds().height + tank_size) / 2 - std::abs((*t)->get_tank_sprite_ptr()->getPosition().y - sprite_center_coords.y);                                       
                if(x_depth < y_depth)   //горизонтальное столкновение
                {
                    if(const1 > ((*m)->getGlobalBounds().width + tank_size) / 2)   //const1 > ((*m)->getGlobalBounds().width + tank_size) / 2 ----> t левее m
                    {
                        
                        (*t)->get_tank_sprite_ptr()->move(-x_depth - EPSILON, 0.f);
                    }
                    else    //const1 < ((*m)->getGlobalBounds().width + tank_size) / 2 ----> t правее m
                    {
                        (*t)->get_tank_sprite_ptr()->move(x_depth + EPSILON, 0.f);
                    }
                }
                else    //вертикальное столкновение
                {
                    if(const2 > ((*m)->getGlobalBounds().height + tank_size) / 2)  //const2 > ((*m)->getGlobalBounds().height + tank_size) / 2 ----> t выше m
                    {
                        (*t)->get_tank_sprite_ptr()->move(0.f, -y_depth - EPSILON);
                    }
                    else    //const2 < ((*m)->getGlobalBounds().height + tank_size) / 2 ----> t ниже m
                    {
                        (*t)->get_tank_sprite_ptr()->move(0.f, y_depth + EPSILON);
                    }
                }
            }
        }

    }
        //bullets' collisions////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
        int num = 0;
        bool collision_took_place = false;
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
                                        //будем как в оригинале удалять по два соседних субблока, перпендикулярных направлению движения пули                    
                                        //уменьшаем здоровье субблока, если сила пули позвляет наносить этому субблоку урон                                        
                                        if((*m)->get_sub_block_ptr(i, j)->sub_block_get_density() <= (*b)->get_bullet_power())
                                        {                                                                                        
                                            if((*m)->get_sub_block_ptr(i, j)->get_sub_block_health() > (*b)->get_bullet_damage())
                                            {                                                
                                                (*m)->get_sub_block_ptr(i, j)->set_sub_block_health((*m)->get_sub_block_ptr(i, j)->get_sub_block_health() - (*b)->get_bullet_damage());    
                                            }                                        
                                            else
                                            {   //вот тут уже удалим целую горизонтальную полоску                                                                                            
                                                if((*b)->get_bullet_direction() == UP || (*b)->get_bullet_direction() == DOWN)  //вертикально летящая пуля
                                                {
                                                    for(int k = 0; k < 2; k++)
                                                    {
                                                        (*m)->get_sub_block_ptr(i, k)->set_sub_block_health(0);
                                                        (*m)->get_sub_block_ptr(i, k)->sub_block_set_density(0);
                                                        (*m)->get_sub_block_ptr(i, k)->sub_block_set_collideble_par(false);
                                                        sf::Vector2f old_position = (*m)->get_sub_block_ptr(i, k)->get_sub_block_sprite_ptr()->getPosition();
                                                        delete (*m)->get_sub_block_ptr(i, k)->get_sub_block_sprite_ptr();
                                                        (*m)->get_sub_block_ptr(i, k)->set_sub_block_sprite_ptr(my_sprites->get_new_sprite_ptr(SMALL_AIR));
                                                        (*m)->get_sub_block_ptr(i, k)->get_sub_block_sprite_ptr()->setPosition(old_position.x, old_position.y);
                                                    }
                                                }
                                                else    //горизонтально летящая пуля
                                                {
                                                    for(int k = 0; k < 2; k++)
                                                    {
                                                        (*m)->get_sub_block_ptr(k, j)->set_sub_block_health(0);
                                                        (*m)->get_sub_block_ptr(k, j)->sub_block_set_density(0);
                                                        (*m)->get_sub_block_ptr(k, j)->sub_block_set_collideble_par(false);
                                                        sf::Vector2f old_position = (*m)->get_sub_block_ptr(i, j)->get_sub_block_sprite_ptr()->getPosition();
                                                        delete (*m)->get_sub_block_ptr(k, j)->get_sub_block_sprite_ptr();
                                                        (*m)->get_sub_block_ptr(k, j)->set_sub_block_sprite_ptr(my_sprites->get_new_sprite_ptr(SMALL_AIR));
                                                        (*m)->get_sub_block_ptr(k, j)->get_sub_block_sprite_ptr()->setPosition(old_position.x, old_position.y);
                                                    }
                                                }
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

void Model::manage_borders_collision_v2()
{
    //tanks collision
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {
        if(!(*t)->get_is_alive_par())
            continue;
        sf::Vector2f cur_t_position = (*t)->get_tank_sprite_ptr()->getPosition();
        if(cur_t_position.x + tank_size / 2 > MAP_SIZE * block_size)  //попали за правую стенку
        {
            (*t)->get_tank_sprite_ptr()->move(MAP_SIZE * block_size - cur_t_position.x - tank_size / 2 - EPSILON, 0.f);
        }
        if(cur_t_position.x - tank_size / 2 < 0) //попали за левую стенку
        {
            (*t)->get_tank_sprite_ptr()->move(-cur_t_position.x + tank_size / 2 + EPSILON, 0.f);
        }
        if(cur_t_position.y + tank_size / 2 > MAP_SIZE * block_size)  //попали за нижнюю стенку
        {
            (*t)->get_tank_sprite_ptr()->move(0.f, MAP_SIZE * block_size - cur_t_position.y - tank_size / 2 - EPSILON);
        }
        if(cur_t_position.y - tank_size / 2 < 0) //попали за верхнюю стенку
        {
            (*t)->get_tank_sprite_ptr()->move(0.f, -cur_t_position.y + tank_size / 2 + EPSILON);
        }
    }

    //bullets collision
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

void Model::manage_tank_tank_collsion()
{
    std::vector<Tank *> nearby_tanks_ptr_vect;
    sf::Vector2f buf_position;
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {
        if(!(*t)->get_is_alive_par())
            continue;
        bool collision_happend = false;
        get_nearby_tanks(&nearby_tanks_ptr_vect, (*t)->get_tank_sprite_ptr());
        for(auto nt = nearby_tanks_ptr_vect.begin(); nt != nearby_tanks_ptr_vect.end(); nt++)
        {
            float const1 = 0., const2 = 0., x_depth = 0.f, y_depth = 0.f;
            if((*nt)->get_tank_sprite_ptr()->getGlobalBounds().intersects((*t)->get_tank_sprite_ptr()->getGlobalBounds()))
            {
                const1 = tank_size - ((*nt)->get_tank_sprite_ptr()->getPosition().x - (*t)->get_tank_sprite_ptr()->getPosition().x);
                const2 = tank_size - ((*nt)->get_tank_sprite_ptr()->getPosition().y - (*t)->get_tank_sprite_ptr()->getPosition().y);
                x_depth = tank_size - std::abs((*nt)->get_tank_sprite_ptr()->getPosition().x - (*t)->get_tank_sprite_ptr()->getPosition().x);
                y_depth = tank_size - std::abs((*nt)->get_tank_sprite_ptr()->getPosition().y - (*t)->get_tank_sprite_ptr()->getPosition().y);
                if(x_depth < y_depth)   //боковое столкновение
                {
                    if(const1 < tank_size) //const1 < tank_size --> nt правее t
                    {
                        (*nt)->get_tank_sprite_ptr()->move(x_depth / 2 + EPSILON, 0);
                        (*t)->get_tank_sprite_ptr()->move(-x_depth / 2 - EPSILON, 0);
                    }
                    else    //const1 > tank_size --> t правее nt
                    {
                        (*t)->get_tank_sprite_ptr()->move(x_depth / 2 + EPSILON, 0);
                        (*nt)->get_tank_sprite_ptr()->move(-x_depth / 2 - EPSILON, 0);
                    }
                }
                else    //вертикальное столкновение
                {
                    if(y_depth < tank_size) //const2 < tank_size --> nt ниже t
                    {
                        (*nt)->get_tank_sprite_ptr()->move(0, y_depth / 2 + EPSILON);
                        (*t)->get_tank_sprite_ptr()->move(0, -y_depth / 2 - EPSILON);
                    }
                    else    //const2 > tank_size --> t ниже nt
                    {
                        (*nt)->get_tank_sprite_ptr()->move(0, -y_depth / 2 - EPSILON);
                        (*t)->get_tank_sprite_ptr()->move(0, y_depth / 2 + EPSILON);
                    }
                }
            }
        }
    }
}

void Model::manage_tank_bullet_collision()  //to - tank owner, b - bullet, t - tank target
{

    for(auto to = tanks.begin(); to != tanks.end(); to++)
    {
        for(auto b = (*to)->get_bullet_ptr_vect()->begin(); b != (*to)->get_bullet_ptr_vect()->end(); b++)
        {
            bool delete_bullet = false;
            for(auto t = tanks.begin(); t != tanks.end(); t++)
            {
                if((*t)->get_is_alive_par())
                {
                    if((*b)->get_bullet_sprite_ptr()->getGlobalBounds().intersects((*t)->get_tank_sprite_ptr()->getGlobalBounds()))
                    {
                        delete_bullet = true;
                        if( (((*to)->get_tank_type() == TANK_GREEN || (*to)->get_tank_type() == TANK_YELLOW) && ((*t)->get_tank_type() != TANK_GREEN && (*t)->get_tank_type() != TANK_YELLOW)) ||
                            (((*t)->get_tank_type() == TANK_GREEN || (*t)->get_tank_type() == TANK_YELLOW) && ((*to)->get_tank_type() != TANK_GREEN && (*to)->get_tank_type() != TANK_YELLOW)))
                        {
                            if((*b)->get_bullet_damage() < (*t)->get_tank_health())
                            {
                                (*t)->set_tank_health((*t)->get_tank_health() - (*b)->get_bullet_damage());
                            }
                            else
                            {
                                (*t)->set_tank_health(0);
                                (*t)->set_is_alive_par(0);
                                (*t)->set_need_to_move_par(0);
                                (*t)->set_need_to_shoot_par(0);
                                delete (*t)->get_tank_sprite_ptr();
                                (*t)->set_tank_sprite_ptr(nullptr);
                                (*t)->set_tank_type(DEFEATED);
                            }
                        }
                        else if(((*to)->get_tank_type() == TANK_GREEN || (*to)->get_tank_type() == TANK_YELLOW) && ((*t)->get_tank_type() == TANK_GREEN || (*t)->get_tank_type() == TANK_YELLOW))
                        {
                            (*t)->set_is_stunned_par(true);
                            (*t)->get_stun_timer()->restart();
                        }
                    }
                }
            }
            if(delete_bullet)
            {
                //удаляем пулю
                delete *b;
                if(b != (--(*to)->get_bullet_ptr_vect()->end()))
                {
                    b = (*to)->get_bullet_ptr_vect()->erase(b);
                }
                else
                {
                    (*to)->get_bullet_ptr_vect()->erase(b);
                    break;
                }
            }
        }
    }
}

void Model::unstun_stunned_tanks()
{
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {
        if((*t)->get_is_alive_par() && (*t)->get_is_stunned_par())
        {
            if((*t)->get_stun_timer()->getElapsedTime() < sf::seconds(STUN_TIME))
                return;
            else
            {
                (*t)->set_is_stunned_par(false);
            }
        }
    }
}

void Model::manage_bullet_bullet_collision()
{
    for(auto to = tanks.begin(); to != tanks.end(); to++)
    {
        if((*to)->get_tank_type() != TANK_GREEN && (*to)->get_tank_type() != TANK_YELLOW)
            continue;
        for(auto b = (*to)->get_bullet_ptr_vect()->begin(); b != (*to)->get_bullet_ptr_vect()->end(); b++)
        {
            bool delete_bullet = false;
            for(auto t = tanks.begin(); t != tanks.end(); t++)
            {
                if((*t)->get_tank_type() == TANK_GREEN || (*t)->get_tank_type() == TANK_YELLOW)
                    continue;
                for(auto bt = (*t)->get_bullet_ptr_vect()->begin(); bt != (*t)->get_bullet_ptr_vect()->end(); bt++)
                {
                    if((*b)->get_bullet_sprite_ptr()->getGlobalBounds().intersects((*bt)->get_bullet_sprite_ptr()->getGlobalBounds()))
                    {
                        delete_bullet = true;
                        //удаляем пулю bt
                        delete *bt;
                        if(bt != (--(*t)->get_bullet_ptr_vect()->end()))
                        {
                            bt = (*t)->get_bullet_ptr_vect()->erase(bt);
                        }
                        else
                        {
                            (*t)->get_bullet_ptr_vect()->erase(bt);
                            break;
                        }
                    }
                }
            }
            if(delete_bullet)
            {
                //удаляем пулю
                delete *b;
                if(b != (--(*to)->get_bullet_ptr_vect()->end()))
                {
                    b = (*to)->get_bullet_ptr_vect()->erase(b);
                }
                else
                {
                    (*to)->get_bullet_ptr_vect()->erase(b);
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
        if((*t)->get_is_alive_par() && !(*t)->get_is_stunned_par())
            (*t)->rotate();
    }
    //movement
    int i = 0;
    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {
        if(get_movement_right(i) == 1 && (*t)->get_need_to_move_par() && (*t)->get_is_alive_par() && !(*t)->get_is_stunned_par())
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

int Model::get_number_of_players()
{
    return number_of_players;
}

void Model::set_number_of_players(int n)
{
    number_of_players = n;
    return;
}


void Model::update(Map * my_map, Sprites * my_sprites)
{
    spawn_enemies();
    all_tanks_shoot(my_sprites);
    manage_borders_collision_v2();
    manage_walls_collsion_v2(my_map, my_sprites);
    manage_tank_tank_collsion();
    manage_tank_bullet_collision();
    manage_bullet_bullet_collision();
    unstun_stunned_tanks();
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
                            {
                                nearby_collidble_sprites_ptr_vect->push_back(current_block_ptr->get_sub_block_ptr(i, j)->get_sub_block_sprite_ptr());
                            }
                        }
                    }
                }
            }

        }
    }
    return;
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

void Model::get_nearby_tanks(std::vector<Tank *> * nearby_tanks_ptr_vect, sf::Sprite * sprite)
{
    nearby_tanks_ptr_vect->clear();
    std::array<sf::Vector2f, 4> buf_array = get_sprite_corners(sprite);
    sf::Vector2f sprite_TL = buf_array[TL];

    for(auto t = tanks.begin(); t != tanks.end(); t++)
    {  
        //не рассматриваем самостолкновения
        if((*t)->get_is_alive_par() && (*t)->get_tank_sprite_ptr() == sprite)
            continue;
        if((*t)->get_is_alive_par())
        {
            std::array<sf::Vector2f, 4> buf_array = get_sprite_corners((*t)->get_tank_sprite_ptr());
            sf::Vector2f tank_TL = buf_array[TL];  
            if( (tank_TL.x - sprite_TL.x) * (tank_TL.x - sprite_TL.x) + (tank_TL.y - sprite_TL.y) * (tank_TL.y - sprite_TL.y) < 
                2 * 4 *block_size * 4 * block_size + 0.5)
            {
                nearby_tanks_ptr_vect->push_back(*t);
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
        if((*t)->get_need_to_shoot_par() && (*t)->get_is_alive_par())
            (*t)->shoot(my_sprites, bullet_size);
    }
}

void Model::prepare_enemy_tanks(std::string enemies_str)
{
    for(auto e = enemies_str.begin(); e != enemies_str.end(); e++)
    {
        switch (*e)
        {
        case '1':
            add_tank_to_vect(new Tank(TANK_ENEMY_1));   
            movement_rights.push_back(1);
            break;
        default:
            break;
        }
    }
}

void Model::spawn_enemies()
{
    int counter = 0;
    if(enemies_spawn_cooldown.getElapsedTime() >= sf::seconds(ENEMIES_SPAWN_COOLDOWN))
        enemies_spawn_cooldown.restart();
    else
        return;

    for(auto t = tanks.begin(); t != tanks.end() && counter != NUMBER_OF_ENEMIES_SPAWNING_AT_ONCE; t++)
    {
        if((*t)->get_tank_type() != TANK_GREEN && (*t)->get_tank_type() != TANK_YELLOW && (*t)->get_tank_type() != DEFEATED && !(*t)->get_is_alive_par())
        {
            (*t)->set_is_alive_par(true);
            (*t)->get_tank_sprite_ptr()->setPosition(get_spawn_position());
            counter++;
        }
    }
}

sf::Vector2f Model::get_spawn_position()
{
    sf::Vector2f buf_vector_2f = {(float)block_size, (float)block_size};
    int chance = rand() % 3;
    if(chance == 0) 
    {
        buf_vector_2f.x = (MAP_SIZE / 2) * block_size;
    }
    else if(chance == 1)
    {
        buf_vector_2f.x = (MAP_SIZE - 1) * block_size;
    }
    return buf_vector_2f;
}