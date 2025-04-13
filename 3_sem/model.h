#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <iostream>
#include <algorithm> 



class Apple;
class Snake;
class Wall;

class Model
{
    private:
    std::list<Apple> apples;
    std::list<Snake> snakes;
    std::list<Wall> walls;
    int number_of_players;
    int number_of_dumb_bots;
    int number_of_smart_bots;
    int number_of_walls;
    int max_apple_num;

    public:
    Model(int num_of_players, int num_of_dumb_bots, int num_of_smart_bots, int num_of_walls);
    void update(std::pair <int, int> screen_size);
    std::list<Apple>* get_apples();
    std::list<Snake>* get_snakes();
    std::list<Wall>* get_walls();
    int is_apple_being_eaten(Apple * apple);
    void spawn_apples(std::pair<int, int> screen_size);
    void spawn_players(std::pair<int, int> screen_size);
    void spawn_walls(std::pair<int, int> screen_size);
    void spawn_bots(std::pair<int, int> screen_size);
    int check_if_coords_are_free(std::pair<int, int> cords);
    void manage_apples();
    void manage_collision(std::pair<int, int> screen_size);
    void manage_walls_movement(std::pair<int, int> screen_size);
    int get_number_of_players();
    int get_number_of_bots();
};

class Apple
{
    private:
    std::pair<int, int> coordinates;
    public:
    Apple(std::pair<int, int> coord);
    std::pair<int, int> get_coords();
    void delete_apple();
};

class Segment
{
    private:
    int do_not_delete = 0;
    std::pair<int, int> coordinates;
    public:
    Segment(std::pair<int, int> coord)
    {
        coordinates.first = coord.first;
        coordinates.second = coord.second;
    }
    std::pair<int, int> get_coords();
    void set_coords(std::pair<int, int> coord);
    void do_not_delete_segment();
    int check_do_not_delete_flag();
};

class Snake
{
    private:
    std::list<Segment> body;
    std::list<Segment> segments_to_delete;
    std::list<Segment> segments_to_set_head;
    std::list<Segment> segments_to_set_body;
    char direction = 'r';
    int alive = 1;
    int has_apple_been_eaten = 0;
    int apple_score = 0;
    int snake_color;
    char controller_type;
    public:
    Snake(std::pair<int, int> coord, char contr_type);

    Segment* get_segment(int i);          // -1 = tail; 1 = head; 2,3,4,5,6... middle body segments
    int get_size();
    void add_new_head(Segment seg);
    void remove_tail();
    void move();
    void add_segment_to_delete(Segment s);
    void add_segment_to_set_head(Segment s);
    void add_segment_to_set_body(Segment s);
    void clear_to_do_lists(char c);
    std::list<Segment>* get_segments_to_delete();
    std::list<Segment>* get_segments_to_set_head();
    std::list<Segment>* get_segments_to_set_body();
    char get_direction();
    void death();
    int is_alive();
    void set_direction(char c);
    void tick_eaten_apple();
    int get_color();
    void set_color(int color);
    int get_apple_score();
    char get_controller_type();
}; 

class Wall
{
    private:
    std::list<Segment> wall_blocks;
    std::list<Segment> blocks_to_delete;
    std::list<Segment> segments_to_set_block;
    char direction;
    int color;
    int range_of_movement;
    int delay = 2;  //количество шагов на которое стенка будет меньше чем змея
    int delay_counter = 0;
    public:
    Wall(std::pair<int, int> coords, std::pair<int, int> size_of_screen, char direct, int movement_range, int set_delay);
    void change_direction(char c);
    char get_direction();
    void add_front_block(Segment b);
    void delete_last_block();
    Segment * get_block(int n); //n = -1 - last, 1 - first, 2, 3, 4, ... - other blocks
    void move();
    void add_block_to_delete(Segment b);
    void add_segment_to_set_block(Segment b);
    void clear_all_to_do_lists();
    void reverse_wall();
    std::list<Segment>* get_blocks_to_delete();
    std::list<Segment>* get_segments_to_set_blocks();
    int check_delay();
    int get_size();
};
#endif