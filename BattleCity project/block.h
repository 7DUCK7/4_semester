#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>
#include <vector>


class Sub_Block;

class Block
{
    private:
    char block_type;
    std::vector<std::vector<Sub_Block>> sub_blocks;
    int block_health;
    bool block_collidable;
    int block_density;  //1 - brick, eagle, 2 - steel, 0 water, bushes, air (transparent for bullets)
    sf::Sprite * block_sprite_ptr;

    public:
    Block();
    char get_block_type();
    void set_block_type(char c);
    int get_block_health();
    void set_block_health(int hp);
    sf::Sprite * get_block_sprite_ptr();
    void set_block_sprite_ptr(sf::Sprite * sprite_ptr);
    bool is_block_collidable();
    void block_set_collidable_par(bool n);
    Sub_Block * get_sub_block_ptr(int i, int j);                                                              
    void block_delete_sprite();
    int get_block_density();
    void set_block_density(int n);
};

class Sub_Block
{
    private:
    int sub_block_health;
    bool sub_block_collidable;
    int density;
    sf::Sprite * sub_block_sprite_ptr;

    public:
    Sub_Block();
    int get_sub_block_health();
    void set_sub_block_health(int hp);
    sf::Sprite * get_sub_block_sprite_ptr();
    void set_sub_block_sprite_ptr(sf::Sprite * sprite_ptr);
    bool is_sub_block_collidable();
    void sub_block_set_collideble_par(bool n);
    bool sub_block_get_collideble_par();
    void sub_block_delete_sprite();
    int sub_block_get_density();
    void sub_block_set_density(int n);
};



#endif