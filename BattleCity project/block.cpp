#include "block.h"

Block::Block() : sub_blocks(2, std::vector<Sub_Block>(2))
{

};

char Block::get_block_type()
{
    return block_type;
}

void Block::set_block_type(char c)
{
    block_type = c;
    return;
}

int Block::get_block_health()
{
    return block_health;
}

void Block::set_block_health(int hp)
{
    block_health = hp;
    return;
}

sf::Sprite * Block::get_block_sprite_ptr()
{
    return block_sprite_ptr;
}

void Block::set_block_sprite_ptr(sf::Sprite * sprite_ptr)
{
    block_sprite_ptr = sprite_ptr;
}

bool Block::is_block_collidable()
{
    return block_collidable;
}

void Block::block_set_collidable_par(bool n)
{
    block_collidable = n;    
    return;
}


Sub_Block * Block::get_sub_block_ptr(int i, int j)
{
    return &sub_blocks[i][j];
}

void Block::block_delete_sprite()
{
    delete block_sprite_ptr;
    block_sprite_ptr = nullptr;
    return;
}

Sub_Block::Sub_Block()
{

}

int Block::get_block_density()
{
    return block_density;
}
void Block::set_block_density(int n)
{
    block_density = n;
    return;
}

int Sub_Block::get_sub_block_health()
{
    return sub_block_health;
}

void Sub_Block::set_sub_block_health(int hp)
{
    sub_block_health= hp;
    return;
}

sf::Sprite * Sub_Block::get_sub_block_sprite_ptr()
{
    return sub_block_sprite_ptr;
}

void Sub_Block::set_sub_block_sprite_ptr(sf::Sprite * sprite_ptr)
{
    sub_block_sprite_ptr = sprite_ptr;
    return;
}

bool Sub_Block::is_sub_block_collidable()
{
    return sub_block_collidable;
}

void Sub_Block::sub_block_set_collidable_par(bool n)
{
    sub_block_collidable = n;    
    return;
}

void Sub_Block::sub_block_delete_sprite()
{
    delete sub_block_sprite_ptr;
    sub_block_sprite_ptr = nullptr;
    return;
}