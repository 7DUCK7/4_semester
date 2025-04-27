#include "sprites.h"
#include <iostream>
#include "block.h"

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

sf::Sprite * Sprites::get_new_sprite_ptr(int c)
{
    sf::Sprite * buf_sprite_ptr;
    int target_size;
    sf::Texture buf_texture;
    switch (c)
    {
    case BUSH:   
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_bush_texture);
        return buf_sprite_ptr;
    }
    case WATER:
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_water_texture);
        return buf_sprite_ptr;
    }
    case STEEL:
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_steel_texture);
        return buf_sprite_ptr;
    }
    case BRICK_TL:   
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_bricks_texture_tl);
        return buf_sprite_ptr;
    }
    case BRICK_TR:   
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_bricks_texture_tr);
        return buf_sprite_ptr;
    }
    case BRICK_BL:   
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_bricks_texture_bl);
        return buf_sprite_ptr;
    }
    case BRICK_BR:   
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_bricks_texture_br);
        return buf_sprite_ptr;
    }
    case EAGLE:
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_eagle_texture);
        return buf_sprite_ptr;
    }
    case AIR:
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTextureRect(sf::IntRect(0, 0, block_size, block_size));
        return buf_sprite_ptr;
    }
    case SMALL_AIR:   // small air to replace destroyed brick sub_blocks
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTextureRect(sf::IntRect(0, 0, sub_block_size, sub_block_size));
        return buf_sprite_ptr;
    }
    case TANK_YELLOW:
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_tank_texture_yellow);
        return buf_sprite_ptr;
    }
    case TANK_GREEN:
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_tank_texture_green);
        return buf_sprite_ptr;
    }
    case TANK_ENEMY_1:
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_tank_texture_enemy_1);
        return buf_sprite_ptr;
    }
    case TANK_ENEMY_2:
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_tank_texture_enemy_2);
        return buf_sprite_ptr;
    }
    case TANK_ENEMY_3:
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_tank_texture_enemy_3);
        return buf_sprite_ptr;
    }
    case TANK_ENEMY_4:
    {
        buf_sprite_ptr = new sf::Sprite;
        buf_sprite_ptr->setTexture(stretched_tank_texture_enemy_4);
        return buf_sprite_ptr;
    }
    default:
    {
        break;
    }
    }

    perror("get_sprite_ptr(): function failed");
    return nullptr;
}

void Sprites::set_block_size(int n)
{
    block_size = n;
    return;
}

void Sprites::set_sub_block_size(int n)
{
    sub_block_size = n;
    return;
}

void Sprites::stretch_all_textures()
{
    stretched_bush_texture = stretch_texture(block_size, bush_texture);
    stretched_water_texture = stretch_texture(block_size, water_texture);
    stretched_steel_texture = stretch_texture(block_size, steel_texture);
    stretched_bricks_texture_tl = stretch_texture(sub_block_size, bricks_texture_tl);
    stretched_bricks_texture_tr = stretch_texture(sub_block_size, bricks_texture_tr);
    stretched_bricks_texture_bl = stretch_texture(sub_block_size, bricks_texture_bl);
    stretched_bricks_texture_br = stretch_texture(sub_block_size, bricks_texture_br);
    stretched_eagle_texture = stretch_texture(block_size * 2, eagle_texture);
    stretched_tank_texture_green = stretch_texture((block_size * 2 - 7), tank_texture_green);
    stretched_tank_texture_yellow = stretch_texture((block_size * 2 - 7), tank_texture_yellow);
    stretched_tank_texture_enemy_1 = stretch_texture((block_size * 2 - 7), tank_texture_enemy_1);
    stretched_tank_texture_enemy_2 = stretch_texture((block_size * 2 - 7), tank_texture_enemy_2);
    stretched_tank_texture_enemy_3 = stretch_texture((block_size * 2 - 7), tank_texture_enemy_3);
    stretched_tank_texture_enemy_4 = stretch_texture((block_size * 2 - 7), tank_texture_enemy_4);
}

sf::Texture Sprites::stretch_texture(int target_size, sf::Texture buf_texture)
{
    sf::RenderTexture renderTexture;
    sf::Sprite tempSprite;
    renderTexture.create(target_size, target_size);
    tempSprite.setTexture(buf_texture);
    tempSprite.setScale(target_size / tempSprite.getLocalBounds().width, target_size / tempSprite.getLocalBounds().height);

    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(tempSprite);
    renderTexture.display();

    return renderTexture.getTexture();
}
