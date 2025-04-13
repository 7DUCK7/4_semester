#include "model.h"
#include "view.h"

#include <cstdlib>

Model::Model(int num_of_players, int num_of_dumb_bots, int num_of_smart_bots, int num_of_walls)
{
    number_of_players = num_of_players;
    number_of_dumb_bots = num_of_dumb_bots;
    number_of_smart_bots = num_of_smart_bots;
    number_of_walls = num_of_walls;
}

void Model::update(std::pair <int, int> screen_size)
{
    manage_walls_movement(screen_size);
    for(auto it = walls.begin(); it != walls.end(); it++)
    {
        it->move();
    }
    for(auto it = snakes.begin(); it != snakes.end(); it++)
    {
        it->move();
    }
    manage_collision(screen_size);
    manage_apples();
    spawn_apples(screen_size);
    max_apple_num = (number_of_players + number_of_smart_bots + number_of_dumb_bots) * 2;
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

std::list<Wall>* Model::get_walls()
{
    return &walls;
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

void Model::spawn_players(std::pair<int, int> screen_size)
{
    switch (number_of_players)
    {
    case 1:
        snakes.push_back(Snake({4, screen_size.second / 2}, 'w'));
        snakes.back().set_color(RED);
        break;
    case 2:
        snakes.push_back(Snake({4, screen_size.second / 3}, 'w'));
        snakes.back().set_color(RED);
        snakes.push_back(Snake({4, screen_size.second / 3 * 2}, 'i'));
        snakes.back().set_color(GREEN);
        break;
    
    default:
        break;
    }
    return;
}

void Model::spawn_apples(std::pair<int, int> screen_size)
{
    std::pair<int, int> p (0, 0);
    while(apples.size() < max_apple_num)
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

void Model::spawn_walls(std::pair<int, int> screen_size)
{
    switch (number_of_walls)
    {
    case 1:
        walls.push_back(Wall({screen_size.first / 3, screen_size.second / 2}, screen_size, 'f', 100, 2));
        break;
    case 2:
        walls.push_back(Wall({screen_size.first / 3, screen_size.second / 2}, screen_size, 'f', 100, 2));
        walls.push_back(Wall({screen_size.first / 3 * 2, screen_size.second / 2}, screen_size, 'f', 100, 1));
    default:
        break;
    }
    return;
}

void Model::spawn_bots(std::pair<int, int> screen_size)
{
    int i = 1;
    for(i = 1; i <= number_of_dumb_bots; i++)
    {
        snakes.push_back(Snake({4, (screen_size.second - 2) / (number_of_dumb_bots + number_of_smart_bots) * i}, 'd'));
        snakes.back().set_color(PURPLE);
    }
    for(i; i <= number_of_dumb_bots + number_of_smart_bots; i++)
    {
        snakes.push_back(Snake({4, (screen_size.second - 2) / (number_of_dumb_bots + number_of_smart_bots) * i}, 's'));
        snakes.back().set_color(BLUE);
    }
}

int Model::check_if_coords_are_free(std::pair<int, int> cords)
{
    Segment * buf_segment;
    //не спавнимся на змеях
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
    //не спавнимся на стенах
    for(auto w = walls.begin(); w != walls.end(); w++)
    {
        int i = 1;
        do
        {
            if(cords == w->get_block(i)->get_coords())
                return 0;
            i++;
        } while (w->get_block(i - 1) != w->get_block(-1));   
    }
    return 1;
}

void Model::manage_apples()
{
    for(auto ap = apples.begin(); ap != apples.end(); ap++)
    {
        
        std::pair<int, int> current_apple_coords = ap->get_coords();
        for(auto sn = snakes.begin(); sn != snakes.end(); sn++)
        {
            if(((sn->get_segment(1))->get_coords()) == current_apple_coords)
            {
                sn->tick_eaten_apple();
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
    
    //manage wall collision
    for(auto sn = snakes.begin(); sn != snakes.end(); sn++)
    {
        int is_sn_dead = 0;
        for(auto w = walls.begin(); w != walls.end(); w++)
        {
            int i = 1;
            Segment* buf_snake_seg;
            Segment* buf_block;
            do
            {
                buf_snake_seg = sn->get_segment(i);
                int j = 1;
                do
                {
                    buf_block = w->get_block(j);
                    if(buf_block->get_coords() == buf_snake_seg->get_coords())
                    {
                        buf_snake_seg->do_not_delete_segment();
                        dead_snakes.push_front(&(*sn));
                        is_sn_dead = 1;
                        break;
                    }
                    j++;
                } while (buf_block != w->get_block(-1));
                if(is_sn_dead)
                    break;
                i++;
            } while (buf_snake_seg != sn->get_segment(-1));
            if(is_sn_dead)
                break;
        }
    }
    //manage apple-wall collision
    for(auto ap = apples.begin(); ap != apples.end(); ap++)
    {
        for(auto w = walls.begin(); w != walls.end(); w++)
        {
            if(w->get_block(1)->get_coords() == ap->get_coords())
            ap = apples.erase(ap);
        }
    }

    for(auto it = dead_snakes.begin(); it != dead_snakes.end(); it++)
        (*it)->death();
    return; 
}

void Model::manage_walls_movement(std::pair<int, int> screen_size)
{
    for(auto w = walls.begin(); w != walls.end(); w++)
    {
        std::pair<int, int> current_front_block_coords = w->get_block(1)->get_coords();
        char current_wall_direction = w->get_direction();
        switch (current_wall_direction)
        {
        case 'f':
            if(current_front_block_coords.second >= screen_size.second - 2)
            {
                w->change_direction('b');
                w->reverse_wall();
            }
            break;

        case 'b':
            if(current_front_block_coords.second <= 3)
            {
                w->change_direction('f');
                w->reverse_wall();
            }
            break;   

        case 'r':
            if(current_front_block_coords.first >= screen_size.first - 2)
            {
                w->change_direction('l');
                w->reverse_wall();
            }
            break;

        case 'l':
            if(current_front_block_coords.first <= 3)
            {
                w->change_direction('r');
                w->reverse_wall();
            }
            break;
        default:
            break;
        }
    }
}

int Model::get_number_of_players()
{
    return number_of_players;
}

int Model::get_number_of_bots()
{
    return number_of_smart_bots + number_of_dumb_bots;
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

void Segment::do_not_delete_segment()
{
    do_not_delete = 1;
    return;
}

int Segment::check_do_not_delete_flag()
{
    return do_not_delete;
}
//----------------------------------------------------------------------------------------------------------------------

Snake::Snake(std::pair<int, int> coord, char contr_type)
{
    controller_type = contr_type;
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
        std::cout << "went out of list, empty nullptr returned\n";
        return nullptr;
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
    for(auto it = ++body.begin(); it != body.end(); it++)
    {
        if(it->check_do_not_delete_flag())
            continue;
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

char Snake::get_controller_type()
{
    return controller_type;
}

Wall::Wall(std::pair<int, int> coords, std::pair<int, int> size_of_screen, char direct, int movement_range, int set_delay)
{
    range_of_movement = movement_range;
    direction = direct;
    delay = set_delay;
    std::pair<int, int> current_coords = coords;
    std::pair<int, int> changer;
    int wall_length = 0;
    switch (direction)
    {
        case 'f':   changer = {0, -1};  wall_length = size_of_screen.second / 4;    break;
        case 'b':   changer = {0, 1};   wall_length = size_of_screen.second / 4;    break;
        case 'l':   changer = {1, 0};   wall_length = size_of_screen.first / 4;     break;
        case 'r':   changer = {-1, 0};  wall_length = size_of_screen.first / 4;     break;
    default:
        break;
    }

    for(int i = 0; i < wall_length; i++)
    {
        wall_blocks.push_back(Segment(current_coords));
        add_segment_to_set_block(*(--wall_blocks.end()));
        current_coords.first += changer.first;
        current_coords.second += changer.second;
    }
    
}

void Wall::change_direction(char c)
{
    direction = c;
    return;
}

char Wall::get_direction()
{
    return direction;
}
void Wall::add_front_block(Segment b)
{
    wall_blocks.push_front(b);
    return;
}

void Wall::delete_last_block()
{
    wall_blocks.pop_back();
    return;
}

Segment * Wall::get_block(int n)
{
    if(n == 1)  return &(*wall_blocks.begin());
    if(n == -1) return &(*(--(wall_blocks.end())));
    if(n > wall_blocks.size())  return nullptr;
    auto it = wall_blocks.begin();
    std::advance(it, n - 1);
    return &(*it);
}

void Wall::move()
{
    if(check_delay() == 0)
        return;
    std::pair<int, int> changer;
    switch (direction)
    {
        case 'f':   changer = {0, 1};  break;
        case 'b':   changer = {0, -1};   break;
        case 'l':   changer = {-1, 0};   break;
        case 'r':   changer = {1, 0};  break;
    default:
        break;
    }

    std::pair<int, int> old_cords;
    for(auto b = wall_blocks.begin(); b != wall_blocks.end(); b++)
    {
        if(b == (--wall_blocks.end()))
            add_block_to_delete(*b);  
        old_cords = b->get_coords();
        old_cords.first += changer.first;
        old_cords.second += changer.second;
        b->set_coords(old_cords);  
        if(b == wall_blocks.begin())
            add_segment_to_set_block(*b); 
    }
}

void Wall::add_block_to_delete(Segment b)
{
    blocks_to_delete.push_back(b);
    return;
}

void Wall::add_segment_to_set_block(Segment b)
{
    segments_to_set_block.push_back(b);
    return;
}

void Wall::clear_all_to_do_lists()
{
    blocks_to_delete.clear();
    segments_to_set_block.clear();
    return;
}

void Wall::reverse_wall()
{
    std::reverse(wall_blocks.begin(), wall_blocks.end());
    return;
}

std::list<Segment>* Wall::get_blocks_to_delete()
{
    return &blocks_to_delete;
}

std::list<Segment>* Wall::get_segments_to_set_blocks()
{
    return &segments_to_set_block;
}

int Wall::check_delay()
{
    if(delay_counter < delay)
        delay_counter++;
    else
    {
        delay_counter = 0;
        return 1;
    }
    return 0;
}

int Wall::get_size()
{
    return wall_blocks.size();
}