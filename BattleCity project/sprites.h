#ifndef SPRITES_H
#define SPRITES_H

#define SPRITE_SIZE 16

#include <SFML/Graphics.hpp>

enum texture_codes
{
    AIR,
    SMALL_AIR,
    BRICK_TL,
    BRICK_TR,
    BRICK_BL,
    BRICK_BR,
    WATER,
    STEEL,
    BUSH,
    EAGLE,
    TANK_YELLOW,
    TANK_GREEN,
    TANK_ENEMY_1,
    TANK_ENEMY_2,
    TANK_ENEMY_3,
    TANK_ENEMY_4
};

class Sprites
{
    private:    //these are on;y images that are used to create a sprite
    //map textures
    int block_size;
    int sub_block_size;

    sf::Texture bricks_texture_tl;
    sf::Texture bricks_texture_tr;
    sf::Texture bricks_texture_bl;
    sf::Texture bricks_texture_br;
    sf::Texture water_texture;
    sf::Texture steel_texture;
    sf::Texture bush_texture;
    sf::Texture eagle_texture;

    //stretched map textures

    sf::Texture stretched_bricks_texture_tl;
    sf::Texture stretched_bricks_texture_tr;
    sf::Texture stretched_bricks_texture_bl;
    sf::Texture stretched_bricks_texture_br;
    sf::Texture stretched_water_texture;
    sf::Texture stretched_steel_texture;
    sf::Texture stretched_bush_texture;
    sf::Texture stretched_eagle_texture;

    //tank textures
    sf::Texture tank_texture_green;
    sf::Texture tank_texture_yellow;
    sf::Texture tank_texture_enemy_1;
    sf::Texture tank_texture_enemy_2;
    sf::Texture tank_texture_enemy_3;
    sf::Texture tank_texture_enemy_4;

    //stretched tank textures
    sf::Texture stretched_tank_texture_green;
    sf::Texture stretched_tank_texture_yellow;
    sf::Texture stretched_tank_texture_enemy_1;
    sf::Texture stretched_tank_texture_enemy_2;
    sf::Texture stretched_tank_texture_enemy_3;
    sf::Texture stretched_tank_texture_enemy_4;

    bool textures_readiness = 0;
    public:
    Sprites();
    bool check_texture_readiness();
    sf::Sprite * get_new_sprite_ptr(int c);
    void set_block_size(int n);
    void set_sub_block_size(int n);
    void stretch_all_textures();
    sf::Texture stretch_texture(int target_size, sf::Texture buf_texture);
};






#endif