#include "map.h"

//user is responsible for map being properly inputed

Map::Map() : map_vect(MAP_SIZE, std::vector<char> (MAP_SIZE, 0)),  map_block_vect(MAP_SIZE, std::vector<Block> (MAP_SIZE))
{
    
};

bool Map::load_map_from_file(char * file_name)
{
    std::string allowed_symbols = "gwsbe ";
    std::string numbers = "1234";
    FILE* input_fd = fopen(file_name, "r");
    if(input_fd == nullptr)
    {
        std::cerr << __FILE__ << ": " << __func__ << "() line " <<  __LINE__  <<  ": failed to open input file" << std::endl;
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
                    std::cerr << __FILE__ << ": " << __func__ << "() line " <<  __LINE__  <<  ": file didn't contain enough key symbols to be a map file" << std::endl;
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
                std::cerr << __FILE__ << ": " << __func__ << "() line " <<  __LINE__  <<  ": found unwanted symbol" << std::endl;
                return 1;   //return 1 if we have found unwanted symbol
            }
        }
    }
    //считываем строку с информацией о противниках на уровне
    while((buf_int = fgetc(input_fd)) != EOF)
    {
        if(numbers.find(buf_int) != std::string::npos)
        {
            enemies_str.push_back(buf_int);
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
    fclose(input_fd);
    return 0;
}

bool Map::map_set_sprites(Sprites * sprites)
{
    //checking whether sprites are ready to be used
    if(!sprites->check_texture_readiness())
    {
        std::cerr << __FILE__ << ": " << __func__ << "() line " <<  __LINE__  <<  ": some sprites are not ready" << std::endl;
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
                map_block_vect[i][j].set_block_sprite_ptr(sprites->get_new_sprite_ptr(char_to_enum(map_block_vect[i][j].get_block_type())));
                sf::Sprite * current_sprite_ptr = map_block_vect[i][j].get_block_sprite_ptr();
                current_sprite_ptr->setPosition(j * block_size, i * block_size);
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
                    map_block_vect[i][j].set_block_sprite_ptr(sprites->get_new_sprite_ptr(char_to_enum(' ')));
                }
                else
                {
                    no_more_eagle_sprites = true;
                    map_block_vect[i][j].set_block_sprite_ptr(sprites->get_new_sprite_ptr(char_to_enum('e')));
                }
                map_block_vect[i][j].set_block_type('e');
                map_block_vect[i][j].set_block_health(1);
                map_block_vect[i][j].set_block_density(1);
                map_block_vect[i][j].block_set_collidable_par(true);
                sf::Sprite * current_sprite_ptr = map_block_vect[i][j].get_block_sprite_ptr();
                current_sprite_ptr->setPosition(j * block_size, i * block_size);
            }
            else    //'b' - brick
            {
                //map_block_vect[i][j] = Block();
                map_block_vect[i][j].set_block_type('b');
                map_block_vect[i][j].block_set_collidable_par(true);
                map_block_vect[i][j].set_block_health(1);
                map_block_vect[i][j].set_block_density(1);
                for(int x = 0; x < 2; x++)
                {
                    for(int y = 0; y < 2; y++)
                    {
                        Sub_Block * current_sb_ptr = map_block_vect[i][j].get_sub_block_ptr(x, y);
                        current_sb_ptr->set_sub_block_health(1);
                        current_sb_ptr->sub_block_set_collideble_par(true);
                        current_sb_ptr->sub_block_set_density(1);
                        if(x == 0 && y == 0) {current_sb_ptr->set_sub_block_sprite_ptr(sprites->get_new_sprite_ptr(BRICK_TL));}
                        else if (x == 0 && y == 1)  {current_sb_ptr->set_sub_block_sprite_ptr(sprites->get_new_sprite_ptr(BRICK_TR));}
                        else if (x == 1 && y == 0)  {current_sb_ptr->set_sub_block_sprite_ptr(sprites->get_new_sprite_ptr(BRICK_BL));}
                        else if (x == 1 && y == 1)  {current_sb_ptr->set_sub_block_sprite_ptr(sprites->get_new_sprite_ptr(BRICK_BR));}
                        current_sb_ptr->get_sub_block_sprite_ptr()->setPosition(j * block_size + y * sub_block_size, i * block_size + x * sub_block_size);
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

int Map::char_to_enum(char c)
{
    switch (c)
    {
    case 'g':   return BUSH;
    case 'w':   return WATER;
    case 's':   return STEEL;
    case 'e':   return EAGLE;
    case ' ':   return AIR;
    default:
        break;
    }
    return -1;
}

void Map::set_block_size(int n)
{
    block_size = n;
    return;
}

void Map::set_sub_block_size(int n)
{
    sub_block_size = n;
    return;
}

std::string Map::get_enemies_string()
{
    return enemies_str;
}