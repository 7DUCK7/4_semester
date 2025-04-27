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
    int block_size;
    int sub_block_size;
    std::vector<std::vector<Block>> map_block_vect;
    std::vector<std::vector<char>> map_vect;
    public:
    Map();
    bool load_map_from_file(char * file_name);
    bool map_set_sprites(Sprites * sprites);
    Block * get_block_ptr(int i, int j);
    int char_to_enum(char c);
    void set_block_size(int n);
    void set_sub_block_size(int n);
};

#endif