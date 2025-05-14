#include "controller.h"
#include "tank.h"
#include "model.h"
#include "map.h"

Controller * Controller::get_controller(char contr_type)
{
    switch (contr_type)
    {
    case '1':   return (Controller*) (new First_Player_Controller());
    case '2':   return (Controller*) (new Second_Player_Controller());
    case 1:   return (Controller*) (new Enemy_1_Bot_Controller());
    case 2:   return (Controller*) (new Enemy_2_Bot_Controller());
    case 3:   return (Controller*) (new Enemy_2_Bot_Controller());
    default:    break;
    }
    return nullptr;
}

Tank * Controller::get_operating_tank_ptr()
{
    return operating_tank_ptr;
}

void Controller::set_operating_tank_ptr(Tank * t)
{
    operating_tank_ptr = t;
    return;
}

First_Player_Controller::First_Player_Controller()
{

}

void First_Player_Controller::process_input(Model * my_model, Map * my_map)
{ 
    //managing movement and movement
    bool is_space_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    bool is_w_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
    bool is_s_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool is_d_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    bool is_a_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    if(is_space_pressed)
    {
        operating_tank_ptr->set_need_to_shoot_par(true);
    }
    if(!is_w_pressed && !is_s_pressed && !is_d_pressed && !is_a_pressed)
        operating_tank_ptr->set_need_to_move_par(false);
    if(is_w_pressed)
    {
        operating_tank_ptr->set_direction(UP);
        operating_tank_ptr->set_need_to_move_par(true);
    }
    else if(is_s_pressed)
    {
        operating_tank_ptr->set_direction(DOWN);
        operating_tank_ptr->set_need_to_move_par(true);
    }
    else if(is_d_pressed)
    {
        operating_tank_ptr->set_direction(RIGHT);
        operating_tank_ptr->set_need_to_move_par(true);
    }
    else if(is_a_pressed)
    {
        operating_tank_ptr->set_direction(LEFT);
        operating_tank_ptr->set_need_to_move_par(true);
    }

}

Second_Player_Controller::Second_Player_Controller()
{

}

void Second_Player_Controller::process_input(Model * my_model, Map * my_map)
{ 
    //managing movement and movement
    bool is_Enter_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
    bool is_Up_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool is_Down_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    bool is_Right_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    bool is_Left_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    if(is_Enter_pressed)
    {
        operating_tank_ptr->set_need_to_shoot_par(true);
    }
    if(!is_Up_pressed && !is_Down_pressed && !is_Right_pressed && !is_Left_pressed)
        operating_tank_ptr->set_need_to_move_par(false);
    if(is_Up_pressed)
    {
        operating_tank_ptr->set_direction(UP);
        operating_tank_ptr->set_need_to_move_par(true);
    }
    else if(is_Down_pressed)
    {
        operating_tank_ptr->set_direction(DOWN);
        operating_tank_ptr->set_need_to_move_par(true);
    }
    else if(is_Right_pressed)
    {
        operating_tank_ptr->set_direction(RIGHT);
        operating_tank_ptr->set_need_to_move_par(true);
    }
    else if(is_Left_pressed)
    {
        operating_tank_ptr->set_direction(LEFT);
        operating_tank_ptr->set_need_to_move_par(true);
    }
}

Enemy_1_Bot_Controller::Enemy_1_Bot_Controller()
{

}

void Enemy_1_Bot_Controller::process_input(Model * my_model, Map * my_map)
{ 
    if(operating_tank_ptr->get_decision_timer_ptr()->getElapsedTime() < sf::seconds(1.f / FRAME_RATE) || operating_tank_ptr->get_forward_movement_timer_ptr()->getElapsedTime() < sf::seconds(1))
    {
        operating_tank_ptr->set_need_to_move_par(true);
        return;
    }
    else
    {
        operating_tank_ptr->get_decision_timer_ptr()->restart();
        operating_tank_ptr->get_forward_movement_timer_ptr()->restart();
    }

    decide_where_to_go(my_model, my_map);
}

void Enemy_1_Bot_Controller::get_environment(Model * my_model, Map * my_map, int* direction_massive, int * bricks_direction_massive)
{
    std::vector<sf::Sprite *> nearby_collidble_sprites_ptr_vect;
    my_model->get_nearby_collideble_map_sprites(&nearby_collidble_sprites_ptr_vect, operating_tank_ptr->get_tank_sprite_ptr(), my_map);

    sf::Vector2f current_position = operating_tank_ptr->get_tank_sprite_ptr()->getPosition();
    std::array<sf::Vector2f, 4> sprite_corners;
    std::pair<float, float> inc = {0., 0.};
    for(int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case UP:
            inc = {0.f, -my_model->get_sub_block_size()};
            break;
        case DOWN:
            inc = {0.f, my_model->get_sub_block_size()};
            break;
        case LEFT:
            inc = {-my_model->get_sub_block_size(), 0.f};
            break;
        case RIGHT:
            inc = {my_model->get_sub_block_size(), 0.f};
            break;
        default:
            break;
        }
        //двигаем спрайт на размер субблока и проверяем столкновение со стенками
        operating_tank_ptr->get_tank_sprite_ptr()->setPosition(current_position.x + inc.first, current_position.y + inc.second);
        sprite_corners = my_model->get_sprite_corners(operating_tank_ptr->get_tank_sprite_ptr());
        for(auto m = nearby_collidble_sprites_ptr_vect.begin(); m != nearby_collidble_sprites_ptr_vect.end(); m++)
        {
            if(operating_tank_ptr->get_tank_sprite_ptr()->getGlobalBounds().intersects((*m)->getGlobalBounds()))
            {
                if((*m)->getGlobalBounds().width > my_model->get_sub_block_size())
                {
                    bricks_direction_massive[i] = 0;
                }
                direction_massive[i] = 0;
            }
        }

        //проверяем выход за пределы карты
        for(int j = 0; j < 4; j++)
        {
            if( sprite_corners[j].x > MAP_SIZE * my_model->get_block_size() ||
                sprite_corners[j].x < 0 ||
                sprite_corners[j].y > MAP_SIZE * my_model->get_block_size() ||
                sprite_corners[j].y < 0)
            {
                bricks_direction_massive[i] = 0;
                direction_massive[i] = 0;
            }
        }
        //проверяем столкновение с другими !!!ЖИВЫМИ!!! танками
        for(auto t = my_model->get_tanks_vect_ptr()->begin(); t != my_model->get_tanks_vect_ptr()->end(); t++)
        {
            //пропускаем мёртвые танки и чтобы танк не сталкивался сам с собой)))
            if(!(*t)->get_is_alive_par() || *t == operating_tank_ptr)
            {
                continue;
            }
            if((*t)->get_tank_sprite_ptr()->getGlobalBounds().intersects(operating_tank_ptr->get_tank_sprite_ptr()->getGlobalBounds()))
            {
                bricks_direction_massive[i] = 0;
                direction_massive[i] = 0;
            }
        }
        operating_tank_ptr->get_tank_sprite_ptr()->setPosition(current_position);
    }
    return;
}

void Enemy_1_Bot_Controller::decide_where_to_go(Model * my_model, Map * my_map)
{
    int direction_massive[4] = {1, 1, 1, 1};
    int bricks_direction_massive[4] = {1, 1, 1, 1};
    get_environment(my_model, my_map, direction_massive, bricks_direction_massive);
    int chance = rand() % 100;
    int number_of_available_alternative_directions = 0;
    operating_tank_ptr->set_need_to_move_par(true);
    operating_tank_ptr->set_need_to_shoot_par(true);
    
    if((direction_massive[operating_tank_ptr->get_direction()] == 1) && chance < 80)
    {
        return;
    }

    if(direction_massive[operating_tank_ptr->get_direction()] == 0 || chance >= 80)
    {
        for(int i = 0; i < 4; i++)
        {
            if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                number_of_available_alternative_directions++;
        }

        if(number_of_available_alternative_directions == 0)
        {
            return;
        }
        else if(number_of_available_alternative_directions == 1)
        {
            for(int i = 0; i < 4; i++)
            {
                if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                    operating_tank_ptr->set_direction(i);
            }
        }
        else if(number_of_available_alternative_directions == 2)
        {
            chance = rand() % 2;
            if(chance)
            {
                for(int i = 0; i < 4; i++)
                {
                    if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                    {
                        operating_tank_ptr->set_direction(i);
                        break;
                    }
                }
            }
            else
            {
                for(int i = 3; i >= 0; i--)
                {
                    if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                    {
                        operating_tank_ptr->set_direction(i);
                        break;
                    }
                }
            }
        }
        else if(number_of_available_alternative_directions == 3)
        {
            chance = rand() % 3;
            int counter = 0;
            for(int i = 0; i < 4; i++)
            {
                if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                {
                    if(chance == counter)
                    {
                        operating_tank_ptr->set_direction(i);
                        break;
                    }
                    else
                    {
                        counter++;
                    }
                }
            }
        }

    }    
}

Enemy_2_Bot_Controller::Enemy_2_Bot_Controller()
{

}

void Enemy_2_Bot_Controller::process_input(Model * my_model, Map * my_map)
{ 
    if(operating_tank_ptr->get_decision_timer_ptr()->getElapsedTime() < sf::seconds(1.f / FRAME_RATE) || operating_tank_ptr->get_forward_movement_timer_ptr()->getElapsedTime() < sf::seconds(0.8) && !forsee_future_not_to_miss_player(my_model, my_map))
    {
        operating_tank_ptr->set_need_to_move_par(true);
        return;
    }
    else 
    {
        operating_tank_ptr->get_decision_timer_ptr()->restart();
        operating_tank_ptr->get_forward_movement_timer_ptr()->restart();
    }

    decide_where_to_go(my_model, my_map);
}

void Enemy_2_Bot_Controller::get_environment(Model * my_model, Map * my_map, int* direction_massive, int * bricks_direction_massive, int * players_direction_massive)
{
    std::vector<sf::Sprite *> nearby_collidble_sprites_ptr_vect;
    std::vector<sf::Sprite *> players_sprites;
    if (my_model->get_number_of_players() == 1)
    {
        if((*my_model->get_tanks_vect_ptr())[0]->get_is_alive_par())
            players_sprites.push_back((*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr());
    }
    else
    {
        if((*my_model->get_tanks_vect_ptr())[0]->get_is_alive_par())
            players_sprites.push_back((*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr());
        if((*my_model->get_tanks_vect_ptr())[1]->get_is_alive_par())
            players_sprites.push_back((*my_model->get_tanks_vect_ptr())[1]->get_tank_sprite_ptr());
    }
    
    my_model->get_nearby_collideble_map_sprites(&nearby_collidble_sprites_ptr_vect, operating_tank_ptr->get_tank_sprite_ptr(), my_map);

    sf::Vector2f current_position = operating_tank_ptr->get_tank_sprite_ptr()->getPosition();
    std::array<sf::Vector2f, 4> sprite_corners;
    std::pair<float, float> inc = {0., 0.};

    for(int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case UP:
            inc = {0.f, -my_model->get_sub_block_size()};
            break;
        case DOWN:
            inc = {0.f, my_model->get_sub_block_size()};
            break;
        case LEFT:
            inc = {-my_model->get_sub_block_size(), 0.f};
            break;
        case RIGHT:
            inc = {my_model->get_sub_block_size(), 0.f};
            break;
        default:
            break;
        }

        //двигаем спрайт на размер субблока и определяем в какую сторону путь свободен, в какую сторону можно проделать путь выстрелами
        operating_tank_ptr->get_tank_sprite_ptr()->setPosition(current_position.x + inc.first, current_position.y + inc.second);
        sprite_corners = my_model->get_sprite_corners(operating_tank_ptr->get_tank_sprite_ptr());
        for(auto m = nearby_collidble_sprites_ptr_vect.begin(); m != nearby_collidble_sprites_ptr_vect.end(); m++)
        {
            if(operating_tank_ptr->get_tank_sprite_ptr()->getGlobalBounds().intersects((*m)->getGlobalBounds()))
            {
                if((*m)->getGlobalBounds().width > my_model->get_sub_block_size())
                {
                    bricks_direction_massive[i] = 0;
                }
                direction_massive[i] = 0;
            }
        }

        for(int j = 0; j < 4; j++)
        {
            //проверяем выход за пределы карты
            if( sprite_corners[j].x > MAP_SIZE * my_model->get_block_size() ||
                sprite_corners[j].x < 0 ||
                sprite_corners[j].y > MAP_SIZE * my_model->get_block_size() ||
                sprite_corners[j].y < 0)
            {
                bricks_direction_massive[i] = 0;
                direction_massive[i] = 0;
            }
        }

        //проверяем столкновение с другими танками
        for(auto t = my_model->get_tanks_vect_ptr()->begin(); t != my_model->get_tanks_vect_ptr()->end(); t++)
        {
            //пропускаем мёртвые танки и чтобы танк не сталкивался сам с собой)))
            if(!(*t)->get_is_alive_par() || *t == operating_tank_ptr)
            {
                continue;
            }

            if((*t)->get_tank_sprite_ptr()->getGlobalBounds().intersects(operating_tank_ptr->get_tank_sprite_ptr()->getGlobalBounds()))
            {
                bricks_direction_massive[i] = 0;
                direction_massive[i] = 0;
            }
        }

        //проверяем в какую сторону идти, чтобы быть стать ближе к любому из игроков
        for(auto p = players_sprites.begin(); p != players_sprites.end(); p++)
        {
            sf::Vector2f player_position = (*p)->getPosition();
            if( (player_position.x - current_position.x) * (player_position.x - current_position.x) + (player_position.y - current_position.y) * (player_position.y - current_position.y) > 
                (player_position.x - operating_tank_ptr->get_tank_sprite_ptr()->getPosition().x) * (player_position.x - operating_tank_ptr->get_tank_sprite_ptr()->getPosition().x) + (player_position.y - operating_tank_ptr->get_tank_sprite_ptr()->getPosition().y) * (player_position.y - operating_tank_ptr->get_tank_sprite_ptr()->getPosition().y))
            {
                players_direction_massive[i] = 1;
            }
        }
        operating_tank_ptr->get_tank_sprite_ptr()->setPosition(current_position);
    }
    return;
}

void Enemy_2_Bot_Controller::decide_where_to_go(Model * my_model, Map * my_map)
{
    int direction_massive[4] = {1, 1, 1, 1};
    int bricks_direction_massive[4] = {1, 1, 1, 1};
    int players_direction_massive[4] = {0, 0, 0, 0};
    get_environment(my_model, my_map, direction_massive, bricks_direction_massive, players_direction_massive);
    int chance = rand() % 100;
    int number_of_available_alternative_directions = 0;
    operating_tank_ptr->set_need_to_move_par(true);
    operating_tank_ptr->set_need_to_shoot_par(true);
    //проверяем, вдруг танк застрял
    if( operating_tank_ptr->get_previos_postion()->x == operating_tank_ptr->get_tank_sprite_ptr()->getPosition().x &&
        operating_tank_ptr->get_previos_postion()->y == operating_tank_ptr->get_tank_sprite_ptr()->getPosition().y
        )
    {
        //если танк не сдвинулся, проверим сколько времени он стоит на месте
        if(operating_tank_ptr->get_not_moving_timer_ptr()->getElapsedTime() >= sf::seconds(5))
        {
            //выберем танку случайное новое направление движения
            int buf_direction;
            while((buf_direction = rand() % 4) == operating_tank_ptr->get_direction())
                ;
            operating_tank_ptr->set_direction(buf_direction);
        }
    }
    else    //если не застрял - обнуляем таймер
    {
        operating_tank_ptr->get_not_moving_timer_ptr()->restart();
    }

    if((direction_massive[operating_tank_ptr->get_direction()] == 1) && players_direction_massive[operating_tank_ptr->get_direction()] == 1)
    {
        return;
    }

    if(direction_massive[operating_tank_ptr->get_direction()] == 0 || players_direction_massive[operating_tank_ptr->get_direction()] == 0)
    {
        for(int i = 0; i < 4; i++)
        {
            if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                number_of_available_alternative_directions++;
        }

        if(number_of_available_alternative_directions == 0)
        {
            return;
        }
        else if(number_of_available_alternative_directions == 1)
        {
            for(int i = 0; i < 4; i++)
            {
                if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                    operating_tank_ptr->set_direction(i);
            }
        }
        else if(number_of_available_alternative_directions == 2)
        {
            //сначала попробуем пойти ближе к игрокам, если не получится, пойдём случайным путём
            bool went_to_players = false;
            for(int i = 0; i < 4; i++)
            {
                if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction() && players_direction_massive[i] == 1)
                {
                    went_to_players = true;
                    operating_tank_ptr->set_direction(i);
                    break;
                }
            }
            
            if(!went_to_players)
            {
                chance = rand() % 2;
                if(chance)
                {
                    for(int i = 0; i < 4; i++)
                    {
                        if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                        {
                            operating_tank_ptr->set_direction(i);
                            break;
                        }
                    }
                }
                else
                {
                    for(int i = 3; i >= 0; i--)
                    {
                        if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                        {
                            operating_tank_ptr->set_direction(i);
                            break;
                        }
                    }
                }
            }
        }
        else if(number_of_available_alternative_directions == 3)
        {
            //сначала попробуем пойти ближе к игрокам, если не получится, пойдём случайным путём
            bool went_to_players = false;
            for(int i = 0; i < 4; i++)
            {
                if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction() && players_direction_massive[i])
                {
                    went_to_players = true;
                    operating_tank_ptr->set_direction(i);
                    break;
                }
            }
            if(!went_to_players)
            {
                chance = rand() % 3;
                int counter = 0;
                for(int i = 0; i < 4; i++)
                {
                    if(direction_massive[i] == 1 && i != operating_tank_ptr->get_direction())
                    {
                        if(chance == counter)
                        {
                            operating_tank_ptr->set_direction(i);
                            break;
                        }
                        else
                        {
                            counter++;
                        }
                    }
                }
            }
        }

    }    
}

bool Enemy_2_Bot_Controller::forsee_future_not_to_miss_player(Model * my_model, Map * my_map)
{
    //получаем текущие данные о танке
    int direction_massive[4] = {1, 1, 1, 1};
    int bricks_direction_massive[4] = {1, 1, 1, 1};
    int players_direction_massive[4] = {0, 0, 0, 0};
    get_environment(my_model, my_map, direction_massive, bricks_direction_massive, players_direction_massive);
    std::pair<float, float> inc = {0., 0.};
    int current_player_direction = players_direction_massive[operating_tank_ptr->get_direction()];
    sf::Vector2f old_position = operating_tank_ptr->get_tank_sprite_ptr()->getPosition();
    //сетаем инкримент для будущего виртуального перемещения
    switch (operating_tank_ptr->get_direction())
    {
    case RIGHT:
        inc.first = operating_tank_ptr->get_tank_speed() * my_model->get_base_tank_speed();
        break;
    case UP:
        inc.second = -operating_tank_ptr->get_tank_speed() * my_model->get_base_tank_speed();
        break;
    case LEFT:
        inc.first = -operating_tank_ptr->get_tank_speed() * my_model->get_base_tank_speed();
        break;
    case DOWN:
        inc.second = operating_tank_ptr->get_tank_speed() * my_model->get_base_tank_speed();
        break;
    default:
        break;
    }
    //виртуально перемещаем и смотрим, не поменяется ли направление к игроку
    operating_tank_ptr->get_tank_sprite_ptr()->setPosition(old_position.x + inc.first, old_position.y + inc.second);
    get_environment(my_model, my_map, direction_massive, bricks_direction_massive, players_direction_massive);
    operating_tank_ptr->get_tank_sprite_ptr()->setPosition(old_position.x, old_position.y);
    //если мы проехали мимо игрока, то надо что-то делать 
    if(current_player_direction != players_direction_massive[operating_tank_ptr->get_direction()])
    {
        return true;
    }
    return false;
}

Enemy_3_Bot_Controller::Enemy_3_Bot_Controller()
{

}

void Enemy_3_Bot_Controller::get_environment(Model * my_model, Map * my_map, int* direction_massive, int * bricks_direction_massive, int * players_direction_massive)
{
    std::vector<sf::Sprite *> nearby_collidble_sprites_ptr_vect;
    std::vector<sf::Sprite *> players_sprites;
    if (my_model->get_number_of_players() == 1)
    {
        if((*my_model->get_tanks_vect_ptr())[0]->get_is_alive_par())
            players_sprites.push_back((*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr());
    }
    else
    {
        if((*my_model->get_tanks_vect_ptr())[0]->get_is_alive_par())
            players_sprites.push_back((*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr());
        if((*my_model->get_tanks_vect_ptr())[1]->get_is_alive_par())
            players_sprites.push_back((*my_model->get_tanks_vect_ptr())[1]->get_tank_sprite_ptr());
    }
    
    my_model->get_nearby_collideble_map_sprites(&nearby_collidble_sprites_ptr_vect, operating_tank_ptr->get_tank_sprite_ptr(), my_map);

    sf::Vector2f current_position = operating_tank_ptr->get_tank_sprite_ptr()->getPosition();
    std::array<sf::Vector2f, 4> sprite_corners;
    std::pair<float, float> inc = {0., 0.};

    for(int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case UP:
            inc = {0.f, -my_model->get_sub_block_size()};
            break;
        case DOWN:
            inc = {0.f, my_model->get_sub_block_size()};
            break;
        case LEFT:
            inc = {-my_model->get_sub_block_size(), 0.f};
            break;
        case RIGHT:
            inc = {my_model->get_sub_block_size(), 0.f};
            break;
        default:
            break;
        }

        //двигаем спрайт на размер субблока и определяем в какую сторону путь свободен, в какую сторону можно проделать путь выстрелами
        operating_tank_ptr->get_tank_sprite_ptr()->setPosition(current_position.x + inc.first, current_position.y + inc.second);
        sprite_corners = my_model->get_sprite_corners(operating_tank_ptr->get_tank_sprite_ptr());
        for(auto m = nearby_collidble_sprites_ptr_vect.begin(); m != nearby_collidble_sprites_ptr_vect.end(); m++)
        {
            if(operating_tank_ptr->get_tank_sprite_ptr()->getGlobalBounds().intersects((*m)->getGlobalBounds()))
            {
                if((*m)->getGlobalBounds().width > my_model->get_sub_block_size())
                {
                    bricks_direction_massive[i] = 0;
                }
                direction_massive[i] = 0;
            }
        }

        for(int j = 0; j < 4; j++)
        {
            //проверяем выход за пределы карты
            if( sprite_corners[j].x > MAP_SIZE * my_model->get_block_size() ||
                sprite_corners[j].x < 0 ||
                sprite_corners[j].y > MAP_SIZE * my_model->get_block_size() ||
                sprite_corners[j].y < 0)
            {
                bricks_direction_massive[i] = 0;
                direction_massive[i] = 0;
            }
        }

        //проверяем столкновение с другими танками
        for(auto t = my_model->get_tanks_vect_ptr()->begin(); t != my_model->get_tanks_vect_ptr()->end(); t++)
        {
            //пропускаем мёртвые танки и чтобы танк не сталкивался сам с собой)))
            if(!(*t)->get_is_alive_par() || *t == operating_tank_ptr)
            {
                continue;
            }

            if((*t)->get_tank_sprite_ptr()->getGlobalBounds().intersects(operating_tank_ptr->get_tank_sprite_ptr()->getGlobalBounds()))
            {
                bricks_direction_massive[i] = 0;
                direction_massive[i] = 0;
            }
        }

        //проверяем в какую сторону идти, чтобы быть стать ближе к любому из игроков
        for(auto p = players_sprites.begin(); p != players_sprites.end(); p++)
        {
            sf::Vector2f player_position = (*p)->getPosition();
            if( (player_position.x - current_position.x) * (player_position.x - current_position.x) + (player_position.y - current_position.y) * (player_position.y - current_position.y) > 
                (player_position.x - operating_tank_ptr->get_tank_sprite_ptr()->getPosition().x) * (player_position.x - operating_tank_ptr->get_tank_sprite_ptr()->getPosition().x) + (player_position.y - operating_tank_ptr->get_tank_sprite_ptr()->getPosition().y) * (player_position.y - operating_tank_ptr->get_tank_sprite_ptr()->getPosition().y))
            {
                players_direction_massive[i] = 1;
            }
        }
        operating_tank_ptr->get_tank_sprite_ptr()->setPosition(current_position);
    }
    return;
}

void Enemy_3_Bot_Controller::decide_where_to_go(Model * my_model, Map * my_map)
{
    int direction_massive[4] = {1, 1, 1, 1};
    int bricks_direction_massive[4] = {1, 1, 1, 1};
    int players_direction_massive[4] = {0, 0, 0, 0};
    get_environment(my_model, my_map, direction_massive, bricks_direction_massive, players_direction_massive);
    int chance = rand() % 100;
    int number_of_available_alternative_directions = 0;
    operating_tank_ptr->set_need_to_move_par(true);
    
    //проверяем, вдруг танк застрял
    if( operating_tank_ptr->get_previos_postion()->x == operating_tank_ptr->get_tank_sprite_ptr()->getPosition().x &&
        operating_tank_ptr->get_previos_postion()->y == operating_tank_ptr->get_tank_sprite_ptr()->getPosition().y
        )
    {
        //если танк не сдвинулся, проверим сколько времени он стоит на месте
        if(operating_tank_ptr->get_not_moving_timer_ptr()->getElapsedTime() >= sf::seconds(5))
        {
            //выберем танку случайное новое направление движения
            int buf_direction;
            while((buf_direction = rand() % 4) == operating_tank_ptr->get_direction())
                ;
            operating_tank_ptr->set_direction(buf_direction);
        }
    }
    else    //если не застрял - обнуляем таймер
    {
        operating_tank_ptr->get_not_moving_timer_ptr()->restart();
    }

    if((direction_massive[operating_tank_ptr->get_direction()] == 1) && players_direction_massive[operating_tank_ptr->get_direction()] == 1)
    {
        return;
    }

    if(direction_massive[operating_tank_ptr->get_direction()] == 0 || players_direction_massive[operating_tank_ptr->get_direction()] == 0)
    {
        for(int i = 0; i < 4; i++)
        {
            if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                number_of_available_alternative_directions++;
        }

        if(number_of_available_alternative_directions == 0)
        {
            return;
        }
        else if(number_of_available_alternative_directions == 1)
        {
            for(int i = 0; i < 4; i++)
            {
                if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                    operating_tank_ptr->set_direction(i);
            }
        }
        else if(number_of_available_alternative_directions == 2)
        {
            //сначала попробуем пойти ближе к игрокам, если не получится, пойдём случайным путём
            bool went_to_players = false;
            for(int i = 0; i < 4; i++)
            {
                if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction() && players_direction_massive[i] == 1)
                {
                    went_to_players = true;
                    operating_tank_ptr->set_direction(i);
                    break;
                }
            }
            
            if(!went_to_players)
            {
                chance = rand() % 2;
                if(chance)
                {
                    for(int i = 0; i < 4; i++)
                    {
                        if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                        {
                            operating_tank_ptr->set_direction(i);
                            break;
                        }
                    }
                }
                else
                {
                    for(int i = 3; i >= 0; i--)
                    {
                        if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction())
                        {
                            operating_tank_ptr->set_direction(i);
                            break;
                        }
                    }
                }
            }
        }
        else if(number_of_available_alternative_directions == 3)
        {
            //сначала попробуем пойти ближе к игрокам, если не получится, пойдём случайным путём
            bool went_to_players = false;
            for(int i = 0; i < 4; i++)
            {
                if((direction_massive[i] == 1 || bricks_direction_massive[i] == 1) && i != operating_tank_ptr->get_direction() && players_direction_massive[i])
                {
                    went_to_players = true;
                    operating_tank_ptr->set_direction(i);
                    break;
                }
            }
            if(!went_to_players)
            {
                chance = rand() % 3;
                int counter = 0;
                for(int i = 0; i < 4; i++)
                {
                    if(direction_massive[i] == 1 && i != operating_tank_ptr->get_direction())
                    {
                        if(chance == counter)
                        {
                            operating_tank_ptr->set_direction(i);
                            break;
                        }
                        else
                        {
                            counter++;
                        }
                    }
                }
            }
        }
    }   

    //если будем пытаться пройти сквозь кирпичи, надо стрелять
    if(bricks_direction_massive[operating_tank_ptr->get_direction()])
    {
        operating_tank_ptr->set_need_to_shoot_par(true);
    }
    //если игрок на линии огня, то стреляем
    sf::Vector2f buf_position;
    sf::Vector2f oper_t_pos = operating_tank_ptr->get_tank_sprite_ptr()->getPosition();
    int direction_to_player;
    for(auto t = my_model->get_tanks_vect_ptr()->begin(); t !=my_model->get_tanks_vect_ptr()->end(); t++)
    {
        if((*t)->get_tank_type() == TANK_GREEN || (*t)->get_tank_type() == TANK_YELLOW)
        {
            buf_position = (*t)->get_tank_sprite_ptr()->getPosition();
            //если игрок под нами, в одном с нами столбце и наш танк повёрнут в нужном направлении(вниз)    //крч перебираем все подобные варианты, при которых мы можем попасть в игрока
            if( buf_position.y > oper_t_pos.y && std::abs(buf_position.x - oper_t_pos.x) < my_model->get_tank_size() && operating_tank_ptr->get_direction() == DOWN ||
                buf_position.y < oper_t_pos.y && std::abs(buf_position.x - oper_t_pos.x) < my_model->get_tank_size() && operating_tank_ptr->get_direction() == UP ||
                buf_position.x > oper_t_pos.x && std::abs(buf_position.y - oper_t_pos.y) < my_model->get_tank_size() && operating_tank_ptr->get_direction() == RIGHT ||
                buf_position.x < oper_t_pos.x && std::abs(buf_position.y - oper_t_pos.y) < my_model->get_tank_size() && operating_tank_ptr->get_direction() == LEFT 
            )
            {
                operating_tank_ptr->set_need_to_shoot_par(true);
            }
        }
    }

}

 void Enemy_3_Bot_Controller::process_input(Model * my_model, Map * my_map)
 {
    if(operating_tank_ptr->get_decision_timer_ptr()->getElapsedTime() < sf::seconds(1.f / FRAME_RATE / 8))
    {
        operating_tank_ptr->set_need_to_move_par(true);
        return;
    }
    else
    {
        operating_tank_ptr->get_decision_timer_ptr()->restart();
        operating_tank_ptr->get_forward_movement_timer_ptr()->restart();
    }

    decide_where_to_go(my_model, my_map);
 }