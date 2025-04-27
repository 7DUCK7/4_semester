#include "GUI.h"

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
    my_model->set_tank_size(my_model->get_block_size() * 2 - 6);
    return;
}
void GUI::set_tanks_speeds()
{
    my_model->set_base_tank_speed(60 * my_model->get_tank_size() / 16 / FRAME_RATE);
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
    set_tanks_speeds();
    std::vector<Tank *> * current_tank_vect_ptr = my_model->get_tanks_vect_ptr();

    sf::RenderWindow my_window(sf::VideoMode(screen_width, screen_height), "Game window");
    my_window.setFramerateLimit(FRAME_RATE);
    my_map->load_map_from_file("levels/level_1.txt");
    my_map->map_set_sprites(my_sprites);
    first_player_controller->set_operating_tank_ptr((*current_tank_vect_ptr)[0]);
    (*current_tank_vect_ptr)[0]->set_tank_sprite_ptr(my_sprites->get_new_sprite_ptr(TANK_GREEN));
    (*current_tank_vect_ptr)[0]->get_tank_sprite_ptr()->setPosition((float)(my_model->get_tank_size() / 2), ((float)my_model->get_tank_size() / 2));
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
                if(current_block_ptr->get_block_type() != 'b')
                {
                    my_window.draw(*current_block_ptr->get_block_sprite_ptr());
                }
                else
                {
                    for(int i = 0; i < 2; i++)
                    {
                        for(int j = 0; j < 2; j++)
                        {
                            Sub_Block * current_sub_block_ptr = current_block_ptr->get_sub_block_ptr(i, j);
                            my_window.draw(*current_sub_block_ptr->get_sub_block_sprite_ptr());
                        }
                    }
                }
            }
        }
        //get heyboard input
        first_player_controller->process_input();
        //update model according to input
        my_model->update();
        my_window.draw(*(*my_model->get_tanks_vect_ptr())[0]->get_tank_sprite_ptr());
        my_window.display();
    }
}

void GUI::set_first_player_controller(Controller * c)
{
    first_player_controller = c;
    return;
}