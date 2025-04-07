#include "model.h"
#include "view.h"

#include <cstdlib>

Model::Model(int num_of_players, std::list<std::pair<int, int>> coords) //Пока что работаем с одной змеёй, а потому num_of_snakes == 1, и координаты передаём через переменную, а не массив и тп
{
    number_of_players = num_of_players;
    int color_ind = RED;
    auto it = coords.begin();
    for(int i = 0; i < coords.size(); i++)
    {
        std::pair<int, int> buf_pair = *it;
        snakes.push_back(Snake(buf_pair));
        snakes.back().set_color(color_ind);
        color_ind++;
        it++;
    }
    return;
}

void Model::update(std::pair <int, int> screen_size)
{
    for(auto it = snakes.begin(); it != snakes.end(); it++)
    {
        it->move();
    }
    manage_collision(screen_size);
    manage_apples();
    spawn_apples(screen_size);
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

int Model::is_apple_being_eaten(Apple * apple)
{
    std::pair apple_coords = apple->get_coords();
    for(auto it = snakes.begin(); it != snakes.end(); it++)
    {
        if(apple_coords == it->get_segment(1)->get_coords())
            return 1;
    }
    return 0;
}

void Model::spawn_apples(std::pair<int, int> screen_size)
{
    std::pair<int, int> p (0, 0);
    while(apples.size() < MAX_APPLES_NUM)
    {
        while(1)
        {
            p = {0, 0};
            while(p.first <= 1)
            {
                p.first = rand() % screen_size.first;
            }
            while(p.second <= 1)
            {
                p.second = rand() % screen_size.second;
            }
            if(check_if_coords_are_free(p))
                break;
        }
        apples.push_back(Apple(p));
    }
}

int Model::check_if_coords_are_free(std::pair<int, int> cords)
{
    Segment * buf_segment;
    for(auto sn = snakes.begin(); sn != snakes.end(); sn++)
    {
        int i = 1;
        do
        {
            if(cords == sn->get_segment(i)->get_coords())
                return 0;
            i++;
        } while (sn->get_segment(i - 1) != sn->get_segment(-1));
    }
    return 1;
}

void Model::manage_apples()
{
    for(auto ap = apples.begin(); ap != apples.end(); ap++)
    {
        //std::cout << "тут ещё не упал" << __LINE__;
        std::pair<int, int> current_apple_coords = ap->get_coords();
        for(auto sn = snakes.begin(); sn != snakes.end(); sn++)
        {
            if(((sn->get_segment(1))->get_coords()) == current_apple_coords)
            {
                //std::cout << "тут ещё не упал" << __LINE__;
                sn->tick_eaten_apple();
                //std::cout << "тут ещё не упал" << __LINE__;
                ap = apples.erase(ap);
                break;
            }
        }
    }
    return;
}

void Model::manage_collision(std::pair<int, int> screen_size)
{
    std::list <Snake*> dead_snakes; 
    std::pair<int ,int> current_snake_head_coords;
    for(auto sn = snakes.begin(); sn != snakes.end(); sn++)
    {
        current_snake_head_coords = (*sn->get_segment(1)).get_coords();
        //checking border collision
        if(current_snake_head_coords.first < 2 || current_snake_head_coords.second < 2 || 
            current_snake_head_coords.first > screen_size.first - 1 || current_snake_head_coords.second > screen_size.second - 1)
        {
            dead_snakes.push_front(&(*sn));
            continue;
        }
        //checking self and other snakes collision
        for(auto sn2 = snakes.begin(); sn2 != snakes.end(); sn2++)
        {
            //self collision
            int did_sn_die = 0;
            int i = 0;
            Segment * buf_segment;

            if(sn2 == sn)   //self collision
                i = 2;
            else    //collision with others
                i = 1;      

            do
            {
                buf_segment = sn2->get_segment(i);
                if(current_snake_head_coords == buf_segment->get_coords())
                {
                    dead_snakes.push_front(&(*sn));
                    did_sn_die = 1;
                    break;
                }
                i++;
            }   while(buf_segment != sn2->get_segment(-1));

            if(did_sn_die)
                break;
        }
    }      
    for(auto it = dead_snakes.begin(); it != dead_snakes.end(); it++)
        (*it)->death();
    return; 
}

int Model::get_number_of_players()
{
    return number_of_players;
}

//----------------------------------------------------------------------------------------------------------------------

Apple::Apple(std::pair<int, int> coord)
{
    coordinates = coord;
    return;
}

std::pair<int, int> Apple::get_coords()
{
    return coordinates;
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

    if(has_apple_been_eaten == 0)
    {
        add_segment_to_delete(*get_segment(-1));
        remove_tail();
    }
    else
    {
        has_apple_been_eaten = 0;
    }

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

void Snake::clear_to_do_lists(char c)
{
    switch (c)
    {
    case 'h':
        segments_to_set_head.clear();
        break;
    case 'b':
        segments_to_set_body.clear();
        break;    
    case 'd':
        segments_to_delete.clear();
        break;    
    case 'a':
        segments_to_delete.clear();
        segments_to_set_body.clear();
        segments_to_set_head.clear();  
        break;         
    default:
        break;
    }

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
    clear_to_do_lists('h');
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

void Snake::tick_eaten_apple()
{
    has_apple_been_eaten++;
    apple_score++;
    return;
}

int Snake::get_color()
{
    return snake_color;
}

void Snake::set_color(int color)
{
    snake_color = color;
    return;
}

int Snake::get_apple_score()
{
    return apple_score;
}