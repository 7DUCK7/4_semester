#include "sprites.h"

Sprites::Sprites()
{
    if
    (
        //loading textures
        !bricks_texture_tl.loadFromFile("textures/map/bricks_texture_tl.png")                 ||
        !bricks_texture_tr.loadFromFile("textures/map/bricks_texture_tr.png")                 ||
        !bricks_texture_bl.loadFromFile("textures/map/bricks_texture_bl.png")                 ||
        !bricks_texture_br.loadFromFile("textures/map/bricks_texture_br.png")                 ||
        !water_texture.loadFromFile("textures/map/water_texture.png")                   ||
        !steel_texture.loadFromFile("textures/map/steel_texture.png")                   ||
        !bush_texture.loadFromFile("textures/map/bush_texture.png")                     ||
        !eagle_texture.loadFromFile("textures/map/eagle_texture.png")                   ||
        !tank_texture_green.loadFromFile("textures/tank/tank_texture_green.png")        ||
        !tank_texture_yellow.loadFromFile("textures/tank/tank_texture_yellow.png")      ||
        !tank_texture_enemy_1.loadFromFile("textures/tank/tank_texture_enemy_1.png")    ||
        !tank_texture_enemy_2.loadFromFile("textures/tank/tank_texture_enemy_2.png")    ||
        !tank_texture_enemy_3.loadFromFile("textures/tank/tank_texture_enemy_3.png")    ||
        !tank_texture_enemy_4.loadFromFile("textures/tank/tank_texture_enemy_4.png")    
    )   {return;}
    textures_readiness = 1;
    
    return;
}

bool Sprites::check_texture_readiness()
{
    return textures_readiness;
}

sf::Sprite * Sprites::get_new_sprite_ptr(char c)
{
    sf::Sprite * buf_sprite_ptr;
    switch (c)
    {
    case 'g':   
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(bush_texture);
        return buf_sprite_ptr;
    }
    case 'w':
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(water_texture);
        return buf_sprite_ptr;
    }
    case 's':
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(steel_texture);
        return buf_sprite_ptr;
    }
    case '1':   
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(bricks_texture_tl);
        return buf_sprite_ptr;
    }
    case '2':   
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(bricks_texture_tr);
        return buf_sprite_ptr;
    }
    case '3':   
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(bricks_texture_bl);
        return buf_sprite_ptr;
    }
    case '4':   
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(bricks_texture_br);
        return buf_sprite_ptr;
    }
    case 'e':
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(eagle_texture);
        return buf_sprite_ptr;
    }
    case ' ':
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTextureRect(sf::IntRect(0, 0, 16, 16));;
        return buf_sprite_ptr;
    }
    case '5':   // small air to replace destroyed brick sub_blocks
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTextureRect(sf::IntRect(0, 0, 8, 8));;
        return buf_sprite_ptr;
    }
    default:
        break;
    }
    perror("get_sprite_ptr(): function failed");
    return nullptr;
}