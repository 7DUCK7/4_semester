#include "model.h"

Model::Model(int num_of_snakes, std::pair<int, int> coords) //Пока что работаем с одной змеёй, а потому num_of_snakes == 1, и координаты передаём через переменную, а не массив и тп
{
    for(int i = 0; i < num_of_snakes; i++)
    {
        snakes.push_back(Snake(coords));
    }
    return;
}

void Model::update()
{
    for(auto it = snakes.begin(); it != snakes.end(); it++)
    {
        it->move();
    }
    return;
}

std::list<Apple>* Model::get_apples()
{
    return &apples;
}

std::list<Snake>* Model::get_snakes()
{
    return &snakes;
}

//----------------------------------------------------------------------------------------------------------------------

std::pair<int, int> Apple::get_coords()
{
    return coordinates;
}

void Apple::set_coords(std::pair<int, int> coord)
{
    coordinates = coord;
    return;
}

void Apple::delete_apple()
{

    return;
}

//----------------------------------------------------------------------------------------------------------------------

std::pair<int, int> Segment::get_coords()
{
    return coordinates;
}

void Segment::set_coords(std::pair<int, int> coord)
{
    coordinates = coord;
    return;
}

void Segment::delete_segment()
{

    return;
}

//----------------------------------------------------------------------------------------------------------------------

Snake::Snake(std::pair<int, int> coord)
{
    Segment buf_segment = Segment(coord);
    add_segment_to_set_head(buf_segment);
    body.push_back(buf_segment);
    coord.first -= 1;
    buf_segment = Segment(coord);
    add_segment_to_set_body(buf_segment);
    body.push_back(buf_segment);
    coord.first -= 1;
    buf_segment = Segment(coord);
    add_segment_to_set_body(buf_segment);
    body.push_back(buf_segment);
}

Segment* Snake::get_segment(int i)
{
    if(i == -1)
        return &(*(--body.end()));
    if(i == 1)
        return &(*body.begin());
    if(i > body.size())
    {
        std::cout << "went out of list, empty Segment(-1, -1) returned\n";
        std::pair<int, int> error_coords(-1, -1);
        return (new Segment(error_coords));
    }
    auto it = body.begin();
    std::advance(it, i - 1);
    return &(*it);
}

int Snake::get_size()
{
    return body.size();
}

void Snake::add_new_head(Segment seg)
{
    body.push_front(seg);
    return;
}

void Snake::remove_tail()
{
    body.pop_back();
    return;
}

void Snake::move()
{
    if(alive == 0)
        return;

    add_segment_to_delete(*get_segment(-1));
    remove_tail();
    Segment* old_head = get_segment(1);
    std::pair <int, int> new_coords = old_head->get_coords();


    switch (direction)
    {
    case 'r':
        new_coords.first += 1;
        break;

    case 'f':
        new_coords.second += 1;
        break;   

    case 'l':
        new_coords.first -= 1;
        break;

    case 'b':
        new_coords.second -= 1;
        break; 

    default:
        break;
    }

    Segment new_head(new_coords);
    add_segment_to_set_body(*old_head);
    add_new_head(new_head);
    add_segment_to_set_head(new_head);
  
    return;
}

void Snake::add_segment_to_delete(Segment s)
{
    segments_to_delete.push_back(s);
    return;
}

void Snake::add_segment_to_set_head(Segment s)
{
    segments_to_set_head.push_back(s);
    return;
}

void Snake::add_segment_to_set_body(Segment s)
{
    segments_to_set_body.push_back(s);
    return;
}

void Snake::clear_to_do_lists()
{
    segments_to_delete.clear();
    segments_to_set_body.clear();
    segments_to_set_head.clear();
    return;
}

std::list<Segment>* Snake::get_segments_to_delete()
{
    return &segments_to_delete; 
}

std::list<Segment>* Snake::get_segments_to_set_head()
{
    return &segments_to_set_head;
}

std::list<Segment>* Snake::get_segments_to_set_body()
{
    return &segments_to_set_body;
}

char Snake::get_direction()
{
    return direction;
}

void Snake::death()
{
    for(auto it = body.begin(); it != body.end(); it++)
    {
        add_segment_to_delete(*it);
    }
    body.clear();
    alive = 0;
    return;
}

int Snake::is_alive()
{
    return alive;
}

void Snake::set_direction(char c)
{
    direction = c;
    return;
}