#ifndef MODEL_H
#define MODEL_H

#include <math.h>
#include <vector>

#include "tank.h"
#include "block.h"
#include "map.h"
#include <string>
#include <SFML/System/Time.hpp>

#define STUN_TIME 4
#define EPSILON 0.05f
#define ENEMIES_SPAWN_COOLDOWN 1.f
#define NUMBER_OF_ENEMIES_SPAWNING_AT_ONCE 3
enum directions
{
    RIGHT,
    UP,
    LEFT,
    DOWN,
    NONE
};

enum corners
{
    TL,
    TR,
    BL,
    BR
};

class Model
{
    private:
    std::vector<Tank *> tanks;
    std::vector<Block *> walls; //blocks that restrict tank's movement
    std::vector<Block *> base;
    std::vector<int> movement_rights;
    int block_size;
    int sub_block_size;
    int tank_size;
    int bullet_size;
    float base_tank_speed;
    float base_bullet_speed;
    int number_of_players;
    sf::Clock enemies_spawn_cooldown;
    std::string enemies;
    bool check_collision(sf::Sprite sprite1, sf::Sprite sprite2);
    void manage_walls_collsion_v2(Map * my_map, Sprites * my_sprites);
    void manage_borders_collision_v2();
    void rotate_and_move_tanks();
    void manage_tank_tank_collsion();
    void manage_tank_bullet_collision();
    void unstun_stunned_tanks();
    void manage_bullet_bullet_collision();
    public:
    Model(int num_of_players);
    std::vector<Tank *>* get_tanks_vect_ptr();
    std::vector<Block *>* get_walls_vect_ptr();
    void add_tank_to_vect(Tank* t);
    void set_block_size(int n);
    void set_sub_block_size(int n);
    void set_tank_size(int n);
    int get_block_size();
    int get_sub_block_size();
    int get_tank_size();
    void set_base_tank_speed(int n);
    void set_base_bullet_speed(int n);
    int get_base_tank_speed();
    int get_base_bullet_speed();
    int get_number_of_players();
    void set_number_of_players(int n);
    void update(Map * my_map, Sprites * my_sprites);
    int get_movement_right(int n);
    void set_movement_right(int n, int value);
    std::array<sf::Vector2f, 4> get_sprite_corners(sf::Sprite * current_sprite);
    void get_nearby_collideble_map_sprites(std::vector<sf::Sprite *> * nearby_collidble_sprites_ptr_vect, sf::Sprite * sprite, Map * my_map);
    void get_nearby_dense_map_blocks(std::vector<Block *> * nearby_dense_blocks_ptr_vect, sf::Sprite * sprite, Map * my_map);
    void get_nearby_tanks(std::vector<Tank *> * nearby_tanks_ptr_vect, sf::Sprite * sprite);
    void set_bullet_size(int n);
    int get_bullet_size();
    void move_all_bullets();
    void all_tanks_shoot(Sprites * my_sprites);
    void prepare_enemy_tanks(std::string enemies_str);
    void spawn_enemies();
    sf::Vector2f get_spawn_position();

};


#endif 