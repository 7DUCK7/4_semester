#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
class Bullet;
class Model;
class Sprites;

class Tank
{
    private:
    int tank_type;  //-1 - first player; -2 - second player; 1 - enemy_1; 2 - enemy_2; 3 - enemy_3; 4 - enemy_4
    int tank_health;
    float tank_speed;
    float tank_reload_time;
    int tank_damage;
    int tank_bullet_power;
    float tank_bullet_speed;
    int direction;
    bool need_to_shoot = false;
    bool ready_to_shoot = true;
    bool need_to_move;
    bool can_move;
    bool is_alive;
    bool is_stunned = false;
    sf::Clock reload_clock;
    sf::Sprite * tank_sprite_ptr;
    std::vector<Bullet *> bullet_ptr_vect;
    sf::Clock decision_timer;
    sf::Clock forward_movement_timer;
    sf::Clock stun_timer;
    public:
    Tank(int type);
    int get_tank_type();
    void set_tank_type(int n);
    int get_tank_health();
    void set_tank_health(int n);
    float get_tank_speed();
    void set_tank_speed(float n);
    float get_tank_reload_time();
    void set_tank_reload_time(float n);
    int get_tank_bullet_power();
    void set_tank_bullet_power(int n);
    int get_tank_damage();
    void set_tank_damage(int n);
    void set_tank_bullet_speed(float n);
    sf::Sprite * get_tank_sprite_ptr();
    void set_tank_sprite_ptr(sf::Sprite * sp);
    int get_direction();
    void set_direction(int n);
    void move(int base_speed);
    void rotate();
    void set_need_to_shoot_par(bool n);
    bool get_need_to_shoot_par();
    void set_ready_to_shoot_par(bool n);
    bool get_ready_to_shoot_par();
    void shoot(Sprites * my_sprites, int bullet_size);
    void add_bullet_ptr_to_vect(Bullet * b);
    std::vector<Bullet*>* get_bullet_ptr_vect();
    sf::Clock * get_reload_clock_ptr(); 
    sf::Clock * get_decision_timer_ptr(); 
    sf::Clock * get_forward_movement_timer_ptr();
    sf::Clock * get_stun_timer();
    bool is_ready_to_shoot();
    
    void set_need_to_move_par(bool n);
    bool get_need_to_move_par();
    bool get_is_alive_par();
    void set_is_alive_par(bool n);
    bool get_is_stunned_par();
    void set_is_stunned_par(bool n);
    /*зарождение будущей жизни
    void set_can_move_par(bool n);
    bool get_can_move_par();
    */
};


#endif