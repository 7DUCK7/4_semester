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
    if(operating_tank_ptr->get_decision_timer_ptr()->getElapsedTime() < sf::seconds(1 / FRAME_RATE) || operating_tank_ptr->get_forward_movement_timer_ptr()->getElapsedTime() < sf::seconds(1.5))
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

void Enemy_1_Bot_Controller::get_environment(Model * my_model, Map * my_map, int* direction_massive)
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
        //двигаем спрайт на размер субблока
        operating_tank_ptr->get_tank_sprite_ptr()->setPosition(current_position.x + inc.first, current_position.y + inc.second);
        sprite_corners = my_model->get_sprite_corners(operating_tank_ptr->get_tank_sprite_ptr());
        for(auto m = nearby_collidble_sprites_ptr_vect.begin(); m != nearby_collidble_sprites_ptr_vect.end(); m++)
        {
            if(operating_tank_ptr->get_tank_sprite_ptr()->getGlobalBounds().intersects((*m)->getGlobalBounds()))
            {
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
    get_environment(my_model, my_map, direction_massive);
    int chance = rand() % 100;
    int number_of_available_alternative_directions = 0;
    operating_tank_ptr->set_need_to_move_par(true);
    operating_tank_ptr->set_need_to_shoot_par(true);
    
    if(direction_massive[operating_tank_ptr->get_direction()] == 1 && chance < 90)
    {
        return;
    }

    if(direction_massive[operating_tank_ptr->get_direction()] == 0 || chance >= 90)
    {
        for(int i = 0; i < 4; i++)
        {
            if(direction_massive[i] == 1 && i != operating_tank_ptr->get_direction())
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
                if(direction_massive[i] == 1 && i != operating_tank_ptr->get_direction())
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
                    if(direction_massive[i] == 1 && i != operating_tank_ptr->get_direction())
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
                    if(direction_massive[i] == 1 && i != operating_tank_ptr->get_direction())
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