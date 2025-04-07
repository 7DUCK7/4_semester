#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <iostream>

#define MAX_APPLES_NUM 5

class Apple;
class Snake;

class Model
{
    private:
    std::list<Apple> apples;
    std::list<Snake> snakes;
    int number_of_players;

    public:
    Model(int num_of_players, std::list<std::pair<int, int>> coords);
    void update(std::pair <int, int> screen_size);
    std::list<Apple>* get_apples();
    std::list<Snake>* get_snakes();
    int is_apple_being_eaten(Apple * apple);
    void spawn_apples(std::pair<int, int> screen_size);
    int check_if_coords_are_free(std::pair<int, int> cords);
    void manage_apples();
    void manage_collision(std::pair<int, int> screen_size);
    int get_number_of_players();
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
    std::pair<int, int> coordinates;
    public:
    Segment(std::pair<int, int> coord)
    {
        coordinates.first = coord.first;
        coordinates.second = coord.second;
    }
    std::pair<int, int> get_coords();
    void set_coords(std::pair<int, int> coord);
    void delete_segment();
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
    public:
    Snake(std::pair<int, int> coord);

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
}; 


#endif