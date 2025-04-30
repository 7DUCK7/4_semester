#include "GUI.h"
#include "bullet.h"

void GUI::prepare_tanks()
{

}

void GUI::get_window_size_and_manage_sprites_sizes()
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    
    screen_height = std::min(desktop.height, desktop.width);
    screen_height = screen_height / (MAP_SIZE * 2) * (MAP_SIZE * 2);
    screen_width = screen_height;
    if(screen_height <= 0 || screen_width <= 0)
        perror("problems with screen sizes in GUI::get_window_size_and_manage_sprites_sizes\n");
    my_sprites->set_block_size(screen_height / MAP_SIZE);
    my_sprites->set_sub_block_size(screen_height / (MAP_SIZE * 2));
    my_map->set_block_size(screen_height / MAP_SIZE);
    my_map->set_sub_block_size(screen_height / (MAP_SIZE * 2));  
    my_model->set_block_size(screen_height / MAP_SIZE);
    my_model->set_sub_block_size(screen_height / (MAP_SIZE * 2));  
    int blck_size = my_model->get_block_size();
    int sb_blck_size = my_model->get_sub_block_size();
    my_model->set_tank_size((blck_size * 2 - 2 * blck_size / 8) - (blck_size * 2 - 2 * blck_size / 8) % 2);
    my_model->set_bullet_size(sb_blck_size + 1);
    my_sprites->set_bullet_size(my_model->get_bullet_size());
    my_sprites->set_tank_size(my_model->get_tank_size());
    return;
}
void GUI::set_tanks_and_bullets_speeds()
{
    my_model->set_base_tank_speed(60 * my_model->get_tank_size() / 16 / FRAME_RATE);
    my_model->set_base_bullet_speed(60 * my_model->get_tank_size() / 16 / FRAME_RATE * 2);
}

void GUI::draw_bullets(sf::RenderWindow* my_window)
{
    std::vector<Tank*>* current_tanks_list_ptr = my_model->get_tanks_vect_ptr();
    for(auto t = current_tanks_list_ptr->begin(); t != current_tanks_list_ptr->end(); t++)
    {
        std::vector<Bullet*> * current_bullet_vect_ptr = (*t)->get_bullet_ptr_vect();
        
        for(auto b = current_bullet_vect_ptr->begin(); b != current_bullet_vect_ptr->end(); b++)
        {
            my_window->draw(*(*b)->get_bullet_sprite_ptr());
            sf::Vector2f bullet_coords = (*b)->get_bullet_sprite_ptr()->getPosition();
        }
    }
}

GUI::GUI(Map * map, Model * model, Sprites * sprites)
{
    my_map = map;
    my_model = model;
    my_sprites = sprites;
}

void GUI::run()
{
    get_window_size_and_manage_sprites_sizes();
    my_sprites->stretch_all_textures();
    set_tanks_and_bullets_speeds();
    sf::RenderWindow my_window(sf::VideoMode(screen_width, screen_height), "Game window");
    my_window.setFramerateLimit(FRAME_RATE);
    my_map->load_map_from_file("levels/level_1.txt");
    my_map->map_set_sprites(my_sprites);
    prepare_players_tanks();
    prepare_bot_tanks();

    //render cycle
    while(my_window.isOpen())
    {
        sf::Event event;
        while(my_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                my_window.close();
        }

        my_window.clear();
        //printing map
        
        //drawing everything except for bushes(grass)
        for(int x = 0; x < MAP_SIZE; x++)
        {
            for(int y = 0; y < MAP_SIZE; y++)
            {
                Block * current_block_ptr = my_map->get_block_ptr(x, y);
                if(current_block_ptr->get_block_type() != 'b' && current_block_ptr->get_block_type() != 'g')
                {
                    my_window.draw(*current_block_ptr->get_block_sprite_ptr());
                    //drawDebugBounds(my_window, *current_block_ptr->get_block_sprite_ptr());
                }
                else if(current_block_ptr->get_block_type() == 'b')
                {
                    for(int i = 0; i < 2; i++)
                    {
                        for(int j = 0; j < 2; j++)
                        {
                            Sub_Block * current_sub_block_ptr = current_block_ptr->get_sub_block_ptr(i, j);
                            my_window.draw(*current_sub_block_ptr->get_sub_block_sprite_ptr());
                            //drawDebugBounds(my_window, *current_sub_block_ptr->get_sub_block_sprite_ptr());
                        }
                    }
                }
            }
        }
        
        //printing only nearest blocks and bullets
        draw_bullets(&my_window);
        /*
        std::vector<sf::Sprite *> nearby_collidble_sprites_ptr_vect;
        my_model->get_nearby_collideble_map_sprites(&nearby_collidble_sprites_ptr_vect, (*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr(), my_map);
        for(auto m = nearby_collidble_sprites_ptr_vect.begin(); m != nearby_collidble_sprites_ptr_vect.end(); m++)
        {
            my_window.draw(*(*m));
            drawDebugBounds(my_window, *(*m));
        }
            */
        //get keyboard input
        all_controllers_process_input();
        //update model according to input
        my_model->update(my_map, my_sprites);
        draw_all_tanks(&my_window);
        //рисуем кусты последними, чтобы они были поверх остальных спрайтов
        for(int x = 0; x < MAP_SIZE; x++)
        {
            for(int y = 0; y < MAP_SIZE; y++)
            {
                Block * current_block_ptr = my_map->get_block_ptr(x, y);
                if(current_block_ptr->get_block_type() == 'g')
                {
                    my_window.draw(*current_block_ptr->get_block_sprite_ptr());
                    //drawDebugBounds(my_window, *current_block_ptr->get_block_sprite_ptr());
                }
            }
        }
        //drawDebugBounds(my_window, *(*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr());
        my_window.display();
    }
}

void GUI::set_first_player_controller(Controller * c)
{
    first_player_controller = c;
    return;
}

void GUI::set_second_player_controller(Controller * c)
{
    second_player_controller = c;
    return;
}

void GUI::set_enemy_1_bot_controller(Controller * c)
{
    enemy_1_bot_controller = c;
    return;
}

void GUI::drawDebugBounds(sf::RenderWindow& window, const sf::Sprite& sprite) 
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::RectangleShape rect({bounds.width, bounds.height});
    rect.setPosition(bounds.left, bounds.top);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(1);
    window.draw(rect);
}

    //first_player_controller->set_operating_tank_ptr((*my_model->get_tanks_vect_ptr())[0]);
    //(*my_model->get_tanks_vect_ptr())[0]->set_tank_sprite_ptr(my_sprites->get_new_sprite_ptr(TANK_GREEN));
    //(*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr()->setPosition((float)(my_model->get_tank_size() / 2) + 2, ((float)my_model->get_tank_size() / 2) + 2);
    //(*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr()->setOrigin((float)(my_model->get_tank_size() / 2), ((float)my_model->get_tank_size() / 2));

void GUI::prepare_bot_tanks()
{
    my_model->prepare_enemy_tanks(my_map->get_enemies_string());
    for(auto t = my_model->get_tanks_vect_ptr()->begin(); t != my_model->get_tanks_vect_ptr()->end(); t++)
    {
        switch ((*t)->get_tank_type())
        {
        case TANK_ENEMY_1:
            (*t)->set_tank_sprite_ptr(my_sprites->get_new_sprite_ptr(TANK_ENEMY_1));
            (*t)->get_tank_sprite_ptr()->setOrigin((float)(my_model->get_tank_size() / 2), ((float)my_model->get_tank_size() / 2));
            (*t)->get_tank_sprite_ptr()->setRotation(180.f);
            break;
        
        default:
            break;
        }
    }
}

void GUI::prepare_players_tanks()
{
    first_player_controller->set_operating_tank_ptr((*my_model->get_tanks_vect_ptr())[0]);
    (*my_model->get_tanks_vect_ptr())[0]->set_tank_sprite_ptr(my_sprites->get_new_sprite_ptr(TANK_GREEN));
    (*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr()->setPosition((float)(9 * my_model->get_block_size()), (float)(25 * my_model->get_block_size()));
    (*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr()->setOrigin((float)(my_model->get_tank_size() / 2), ((float)my_model->get_tank_size() / 2));
    if(my_model->get_number_of_players() == 2)
    {
        second_player_controller->set_operating_tank_ptr((*my_model->get_tanks_vect_ptr())[1]);
        (*my_model->get_tanks_vect_ptr())[1]->set_tank_sprite_ptr(my_sprites->get_new_sprite_ptr(TANK_YELLOW));
        (*my_model->get_tanks_vect_ptr())[1]->get_tank_sprite_ptr()->setPosition((float)(17 * my_model->get_block_size()), (float)(25 * my_model->get_block_size()));
        (*my_model->get_tanks_vect_ptr())[1]->get_tank_sprite_ptr()->setOrigin((float)(my_model->get_tank_size() / 2), ((float)my_model->get_tank_size() / 2));
    }
}

void GUI::all_controllers_process_input()
{
    first_player_controller->process_input(nullptr, nullptr);
    if(my_model->get_number_of_players() == 2)
        second_player_controller->process_input(nullptr, nullptr);
    for(auto t = my_model->get_tanks_vect_ptr()->begin(); t != my_model->get_tanks_vect_ptr()->end(); t++)
    {
        if((*t)->get_tank_type() == TANK_ENEMY_1 && ((*t)->get_is_alive_par()))
        {
            enemy_1_bot_controller->set_operating_tank_ptr(*t);
            enemy_1_bot_controller->process_input(my_model, my_map);
        }
    }
}

void GUI::draw_all_tanks(sf::RenderWindow * my_window)
{
    for(auto t = my_model->get_tanks_vect_ptr()->begin(); t != my_model->get_tanks_vect_ptr()->end(); t++)
    {
        if((*t) == nullptr)
            std::cout << "bad ptr " << __LINE__ << std::endl;
        if((*t)->get_is_alive_par())
            my_window->draw(*((*t)->get_tank_sprite_ptr()));
    }
}