#ifndef MODEL_H
#define MODEL_H

#include <list>
#include <iostream>

class Apple;
class Snake;

class Model
{
    private:
    std::list<Apple> apples;
    std::list<Snake> snakes;

    public:
    Model(int num_of_snakes, std::pair<int, int> coords);
    void update();
    std::list<Apple>* get_apples();
    std::list<Snake>* get_snakes();
};

class Apple
{
    private:
    std::pair<int, int> coordinates;
    public:
    std::pair<int, int> get_coords();
    void set_coords(std::pair<int, int> coord);
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
    void clear_to_do_lists();
    std::list<Segment>* get_segments_to_delete();
    std::list<Segment>* get_segments_to_set_head();
    std::list<Segment>* get_segments_to_set_body();
    char get_direction();
    void death();
    int is_alive();
    void set_direction(char c);
}; 


#endif