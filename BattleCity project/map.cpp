#include "map.h"

//user is responsible for map being properly inputed

Map::Map(Sprites * sprts) : map_vect(MAP_SIZE, std::vector<char> (MAP_SIZE, 0)),  map_block_vect(MAP_SIZE, std::vector<Block> (MAP_SIZE))
{
    std::cout << "I'm alive " << __FILE__ << ' ' << __LINE__ << std::endl;
    my_sprites = sprts;
};

bool Map::load_map_from_file(char * file_name)
{
    std::string allowed_symbols = "gwsbe ";
    FILE* input_fd = fopen(file_name, "r");
    if(input_fd == nullptr)
    {
        perror("load_map_from_file(): failed to open input file\n");
        return 1;
    }
    int buf_int;

    for(int i = 0; i < MAP_SIZE; i++)
    {
        
        for(int j = 0; j < MAP_SIZE; j++)
        {
            while(allowed_symbols.find(buf_int = fgetc(input_fd)) == std::string::npos) //skip unnecessary at the end of lines
            {
                if(buf_int == EOF)
                {   
                    perror("load_from_file(): file didn't contain enough key symbols to be a map file\n");
                    return 1;
                }
            }
            map_vect[i][j] = (char)buf_int;
        }
    }
    
    for(int i = 0; i < MAP_SIZE; i++)
    {
        for(int j = 0; j < MAP_SIZE; j++)
        {
            if(allowed_symbols.find(map_vect[i][j]) == std::string::npos)
            {
                return 1;   //return 1 if we have found unwanted symbol
            }
        }
    }

    /*  Вывод текстовой карты
    for(int i = 0; i < MAP_SIZE; i++)
    {
        for(int j = 0; j < MAP_SIZE; j++)
        {
            std::cout << map_vect[i][j];
        }
        std::cout << std::endl;
    }

    */
    return 0;
}

bool Map::map_set_sprites()
{
    //checking whether sprites are ready to be used
    if(!my_sprites->check_texture_readiness())
    {
        perror("check_texture_readiness(): function failed\n");
        return 1;
    }

    //creating blocks and setting sprites
    bool no_more_eagle_sprites = false;
    for(int i = 0; i < MAP_SIZE; i++)
    {
        for(int j = 0; j < MAP_SIZE; j++)
        {
            if(map_vect[i][j] != 'b' && map_vect[i][j] != 'e')
            {
                //map_block_vect[i][j] = Block();
                map_block_vect[i][j].set_block_type(map_vect[i][j]);
                map_block_vect[i][j].set_block_sprite_ptr(my_sprites->get_new_sprite_ptr(map_block_vect[i][j].get_block_type()));
                sf::Sprite * current_sprite_ptr = map_block_vect[i][j].get_block_sprite_ptr();
                current_sprite_ptr->setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);
                if(map_vect[i][j] == 'g' || map_vect[i][j] == ' ')
                {
                    map_block_vect[i][j].set_block_health(1);
                    map_block_vect[i][j].block_set_collidable_par(false);
                    map_block_vect[i][j].set_block_density(0);
                }
                else if(map_vect[i][j] == 'w')
                {
                    map_block_vect[i][j].set_block_health(1);
                    map_block_vect[i][j].block_set_collidable_par(true);
                    map_block_vect[i][j].set_block_density(0);
                }
                else // 's'
                {
                    map_block_vect[i][j].set_block_health(1);
                    map_block_vect[i][j].block_set_collidable_par(true);
                    map_block_vect[i][j].set_block_density(2);                    
                }
            }
            else if(map_vect[i][j] == 'e')
            {
                //map_block_vect[i][j] = Block();
                if(no_more_eagle_sprites)
                {
                    map_block_vect[i][j].set_block_sprite_ptr(my_sprites->get_new_sprite_ptr(' '));
                }
                else
                {
                    no_more_eagle_sprites = true;
                    map_block_vect[i][j].set_block_sprite_ptr(my_sprites->get_new_sprite_ptr('e'));
                }
                map_block_vect[i][j].set_block_type('e');
                map_block_vect[i][j].set_block_health(1);
                map_block_vect[i][j].set_block_density(1);
                map_block_vect[i][j].block_set_collidable_par(true);
                sf::Sprite * current_sprite_ptr = map_block_vect[i][j].get_block_sprite_ptr();
                current_sprite_ptr->setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);
            }
            else    //'b' - brick
            {
                //map_block_vect[i][j] = Block();
                map_block_vect[i][j].set_block_type('b');
                for(int x = 0; x < 2; x++)
                {
                    for(int y = 0; y < 2; y++)
                    {
                        Sub_Block * current_sb_ptr = map_block_vect[i][j].get_sub_block_ptr(x, y);
                        current_sb_ptr->set_sub_block_health(1);
                        current_sb_ptr->sub_block_set_collidable_par(true);
                        if(x == 0 && y == 0) {current_sb_ptr->set_sub_block_sprite_ptr(my_sprites->get_new_sprite_ptr('1'));}
                        else if (x == 0 && y == 1)  {current_sb_ptr->set_sub_block_sprite_ptr(my_sprites->get_new_sprite_ptr('2'));}
                        else if (x == 1 && y == 0)  {current_sb_ptr->set_sub_block_sprite_ptr(my_sprites->get_new_sprite_ptr('3'));}
                        else if (x == 1 && y == 1)  {current_sb_ptr->set_sub_block_sprite_ptr(my_sprites->get_new_sprite_ptr('4'));}
                        current_sb_ptr->get_sub_block_sprite_ptr()->setPosition(j * BLOCK_SIZE + y * SUB_BLOCK_SIZE, i * BLOCK_SIZE + x * SUB_BLOCK_SIZE);
                    }
                }
            }
        }
    }
    return 0;
}

Block * Map::get_block_ptr(int i, int j)
{
    return &map_block_vect[i][j];
}