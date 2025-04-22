#include "GUI.h"


GUI::GUI(Map * map)
{
    my_map = map;
}

void GUI::run()
{
    sf::RenderWindow my_window(sf::VideoMode(MAP_SIZE * SPRITE_SIZE, MAP_SIZE * SPRITE_SIZE), "Game window");
    my_map->load_map_from_file("levels/level_1.txt");
    my_map->map_set_sprites();

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

        my_window.display();
    }
}

