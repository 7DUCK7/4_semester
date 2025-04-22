#ifndef MAP_H
#define MAP_H

/*  blockstable
g - bush (grass)
w - water
s - steel
b - bricks
e - eagle
" " (space symbol) - air
*/
#include "sprites.h"
#include "block.h"

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

#define MAP_SIZE 26 //size of map in blocks

class Map
{
    private:
    Sprites * my_sprites;
    std::vector<std::vector<char>> map_vect;
    public:
    std::vector<std::vector<Block>> map_block_vect;
    Map(Sprites * sprts);
    bool load_map_from_file(char * file_name);
    bool map_set_sprites();
    Block * get_block_ptr(int i, int j);
};

#endif