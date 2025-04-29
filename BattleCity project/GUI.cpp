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
        //std::cout << "rn this amuont of bullets is present: " << current_bullet_vect_ptr->size() << std::endl;
        for(auto b = current_bullet_vect_ptr->begin(); b != current_bullet_vect_ptr->end(); b++)
        {
            my_window->draw(*(*b)->get_bullet_sprite_ptr());
            sf::Vector2f bullet_coords = (*b)->get_bullet_sprite_ptr()->getPosition();
            //std::cout << "drew a bullet with following coords: " << bullet_coords.x << ' ' << bullet_coords.y << std::endl;
            //std::cout << "rn this amuont of bullets is present: " << current_bullet_vect_ptr->size() << std::endl;
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
    std::vector<Tank *> * current_tank_vect_ptr = my_model->get_tanks_vect_ptr();

    sf::RenderWindow my_window(sf::VideoMode(screen_width, screen_height), "Game window");
    my_window.setFramerateLimit(FRAME_RATE);
    my_map->load_map_from_file("levels/level_1.txt");
    my_map->map_set_sprites(my_sprites);
    first_player_controller->set_operating_tank_ptr((*current_tank_vect_ptr)[0]);
    (*current_tank_vect_ptr)[0]->set_tank_sprite_ptr(my_sprites->get_new_sprite_ptr(TANK_GREEN));
    (*current_tank_vect_ptr)[0]->get_tank_sprite_ptr()->setPosition((float)(my_model->get_tank_size() / 2) + 2, ((float)my_model->get_tank_size() / 2) + 2);
    (*current_tank_vect_ptr)[0]->get_tank_sprite_ptr()->setOrigin((float)(my_model->get_tank_size() / 2), ((float)my_model->get_tank_size() / 2));
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
        first_player_controller->process_input();
        //update model according to input
        my_model->update(my_map, my_sprites);
        my_window.draw(*(*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr());
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