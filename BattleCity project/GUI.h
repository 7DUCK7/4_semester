#ifndef GUI_H
#define GUI_H

#include <SFML/Window.hpp>

#include "map.h"
#include "controller.h"
#include "model.h"

#define FRAME_RATE 90
class Map;
class Controller;
class Model;

class GUI
{
    private:
    int screen_height;
    int screen_width;

    Map * my_map;
    Model * my_model;
    Sprites * my_sprites;
    Controller * first_player_controller;
    Controller * second_player_controller;
    Controller * enemy_1_bot_controller;
    Controller * enemy_2_bot_controller;
    Controller * enemy_3_bot_controller;
    std::pair <int, int> window_size;
    int finish = 0;
    void prepare_tanks();
    void get_window_size_and_manage_sprites_sizes();
    void set_tanks_and_bullets_speeds();
    void draw_bullets(sf::RenderWindow* my_window);
    public:
    GUI(Map * map, Model * model, Sprites * sprites);
    ~GUI()  {};
    void run();
    void set_first_player_controller(Controller * c);
    void set_second_player_controller(Controller * c);
    void set_enemy_1_bot_controller(Controller * c);
    void set_enemy_2_bot_controller(Controller * c);
    void set_enemy_3_bot_controller(Controller * c);
    void drawDebugBounds(sf::RenderWindow& window, const sf::Sprite& sprite);
    void prepare_players_tanks();
    void all_controllers_process_input();
    void draw_all_tanks(sf::RenderWindow * my_window);
    void prepare_bot_tanks();
    void finish_lost_game(sf::RenderWindow * my_window);
    void move_window(sf::RenderWindow * my_window, int desktop_width);
    void finish_won_game(sf::RenderWindow * my_window);
};


#endif 