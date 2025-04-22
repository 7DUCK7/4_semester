#ifndef SPRITES_H
#define SPRITES_H

#define SPRITE_SIZE 16

#include <SFML/Graphics.hpp>

class Sprites
{
    private:    //these are on;y images that are used to create a sprite
    //map textures
    sf::Texture bricks_texture_tl;
    sf::Texture bricks_texture_tr;
    sf::Texture bricks_texture_bl;
    sf::Texture bricks_texture_br;
    sf::Texture water_texture;
    sf::Texture steel_texture;
    sf::Texture bush_texture;
    sf::Texture eagle_texture;

    //tank textures
    sf::Texture tank_texture_green;
    sf::Texture tank_texture_yellow;
    sf::Texture tank_texture_enemy_1;
    sf::Texture tank_texture_enemy_2;
    sf::Texture tank_texture_enemy_3;
    sf::Texture tank_texture_enemy_4;

    bool textures_readiness = 0;
    public:
    Sprites();
    bool check_texture_readiness();
    sf::Sprite * get_new_sprite_ptr(char c);
};






#endif