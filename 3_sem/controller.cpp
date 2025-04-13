#include "controller.h"
#include "model.h"

#include <poll.h>
#include <time.h>
#include <list>

Controller * Controller::getController(char cont_type)
{
    switch(cont_type)
    {
        case 'w':   return((Controller*) new WASD_Controller());
        case 'i':   return((Controller*) new IJKL_Controller());
        case 'd':   return((Controller*) new Dumb_Bot_Controller());
        case 's':   return((Controller*) new Smart_Bot_Controller());
        default:    return nullptr;
    }
}

WASD_Controller::WASD_Controller()
{
    
}

void WASD_Controller::process_input(char c, Model * my_model, std::pair<int, int> screen_size, int snake_ind)
{
    std::list<Snake> * current_snakes = my_model->get_snakes();
    auto first_player = current_snakes->begin();
    char current_direction = first_player->get_direction();
    switch (current_direction)
    {
    case 'l':
    case 'r':
        switch (c)
        {    
        case 'w':
            first_player->set_direction('f');
            break;

        case 's':
            first_player->set_direction('b');
            break;
        default:
            break;
        }
        break;
    
    case 'b':
    case 'f':
        switch (c)
        {
        case 'd':
            first_player->set_direction('r');
            break;
    
        case 'a':
            first_player->set_direction('l');
            break;

        default:
            break;
        }
        break;
    default:
        break;
    }

    return;
}

IJKL_Controller::IJKL_Controller()
{
    
}

void IJKL_Controller::process_input(char c, Model * my_model, std::pair<int, int> screen_size, int snake_ind)
{
    std::list<Snake> * current_snakes = my_model->get_snakes();
    auto second_player = current_snakes->begin();
    second_player++;
    char current_direction_of_2nd_player = second_player->get_direction();
    switch (current_direction_of_2nd_player)
    {
    case 'l':               //  i
    case 'r':               // jkl
        switch (c)
        {    
        case 'i':
            second_player->set_direction('f');
            break;

        case 'k':
            second_player->set_direction('b');
            break;
        default:
            break;
        }
        break;
    
    case 'b':
    case 'f':
        switch (c)
        {
        case 'l':
            second_player->set_direction('r');
            break;
    
        case 'j':
            second_player->set_direction('l');
            break;

        default:
            break;
        }
        break;
        default:
            break;            
    }
}

void Dumb_Bot_Controller::get_closest_apple(Model * my_model)
{
    std::list<Snake> * current_snakes_list = my_model->get_snakes();
    auto it = current_snakes_list->begin();
    std::advance(it, snake_number);     //считаем змей с нуля
    Snake * operating_snake = &(*it);
    if(operating_snake->is_alive() == 0)    //мёртвая змея
        return;

    std::list<Apple> * current_apples_list = my_model->get_apples();
    int min_distance = 1000000;
    Apple * closest_apple = nullptr;
    for(auto ap = current_apples_list->begin(); ap != current_apples_list->end(); ap++)
    {
        int x = ap->get_coords().first - operating_snake->get_segment(1)->get_coords().first;
        int y = ap->get_coords().second - operating_snake->get_segment(1)->get_coords().second;
        if(x*x + y*y < min_distance)
        {
            closest_apple = &(*ap);
            min_distance = x*x + y*y;
        }
    }
    current_closest_apple = closest_apple;
    
} 

Dumb_Bot_Controller::Dumb_Bot_Controller()
{
    
}
void Dumb_Bot_Controller::decide_where_to_go(Model * my_model, std::pair <int, int> screen_size)
{
    int nearest_walls[4] = {0, 0, 0, 0};
    std::list<Snake> * current_snakes_list = my_model->get_snakes();
    auto it = current_snakes_list->begin();   
    std::advance(it, snake_number);     //считаем змей с нуля
    Snake * operating_snake = &(*it);


    if(operating_snake->is_alive() == 0)    //мёртвая змея
        return;

    check_nearby_walls(nearest_walls, my_model, screen_size);
    //totally traped:
    if(nearest_walls[UP] == 1 && nearest_walls[LEFT] == 1 && nearest_walls[DOWN] == 1 && nearest_walls[RIGHT] == 1)
        return; //do nothing and just die
    //when there are some options:
    std::pair<int, int> head_coords= operating_snake->get_segment(1)->get_coords();
    char current_direction = operating_snake->get_direction();
    if(head_coords.first == current_closest_apple->get_coords().first) //если мы на одной вертикали с яблоком-целью
    {
        if(head_coords.second < current_closest_apple->get_coords().second)    //если мы ниже яблока-цели
        {
            if(current_direction == 'f' && !nearest_walls[UP])  {return;};  //если мы движемся наверх(вперёд) и нет впереди стены, то ничего не меняем 
            if(current_direction == 'f' && nearest_walls[UP] && !nearest_walls[RIGHT])
            //теперь нам мешает стена спереди, но справа нет стены, тогда просто пойдём вправо
            {
                operating_snake->set_direction('r');
            }
            if(current_direction == 'f' && nearest_walls[UP] && !nearest_walls[RIGHT])
            //теперь нам мешает стена спереди, но слева нет стены, тогда просто пойдём влево
            {
                operating_snake->set_direction('l');
            }
            if(current_direction == 'l' || current_direction == 'r')//надо повернуть на эту вертикаль
            {
                if(!nearest_walls[UP])
                    operating_snake->set_direction('f');    //иначе должны просто сместиться с этой вертикали
            }
            if(current_direction == 'b' && !nearest_walls[RIGHT])   {operating_snake->set_direction('r');}
            else if(current_direction == 'b' && !nearest_walls[LEFT])   {operating_snake->set_direction('l');};
        }
        else    //если мы выше яблока цели
        {
            if(current_direction == 'b' && !nearest_walls[DOWN])  {return;};  //если мы движемся вниз(назад) и нет стены, то ничего не меняем 
            if(current_direction == 'b' && nearest_walls[DOWN] && !nearest_walls[RIGHT])
            //теперь нам мешает стена, но справа нет стены, тогда просто пойдём вправо
            {
                operating_snake->set_direction('r');
            }
            if(current_direction == 'b' && nearest_walls[DOWN] && !nearest_walls[RIGHT])
            //теперь нам мешает стена, но слева нет стены, тогда просто пойдём влево
            {
                operating_snake->set_direction('l');
            }
            if(current_direction == 'l' || current_direction == 'r')//надо повернуть на эту вертикаль
            {
                if(!nearest_walls[DOWN])
                    operating_snake->set_direction('b');    //иначе должны просто сместиться с этой вертикали
            }
            if(current_direction == 'f' && !nearest_walls[RIGHT])   {operating_snake->set_direction('r');}
            else if(current_direction == 'f' && !nearest_walls[LEFT])   {operating_snake->set_direction('l');};
        }
    }
    else if(head_coords.second == current_closest_apple->get_coords().second)  //если мы с яблоком-целью на одной горизонтали
    {
        if(head_coords.first < current_closest_apple->get_coords().first)    //если мы левее яблока-цели
        {
            if(current_direction == 'r' && !nearest_walls[RIGHT])  {return;};  //если мы движемся вправо и нет стены, то ничего не меняем 
            if(current_direction == 'r' && nearest_walls[RIGHT] && !nearest_walls[UP])
            //теперь нам мешает стена, но сверху нет стены, тогда просто пойдём верх
            {
                operating_snake->set_direction('f');
            }
            if(current_direction == 'r' && nearest_walls[UP] && !nearest_walls[DOWN])
            //теперь нам мешает стена, но снизу нет стены, тогда просто пойдём вниз
            {
                operating_snake->set_direction('b');
            }
            if(current_direction == 'f' || current_direction == 'b')//надо повернуть на эту горизонталь
            {
                if(!nearest_walls[RIGHT])
                    operating_snake->set_direction('r');    //иначе должны просто сместиться с этой горизонтали
            }
            if(current_direction == 'l' && !nearest_walls[UP])   {operating_snake->set_direction('f');}
            else if(current_direction == 'l' && !nearest_walls[DOWN])   {operating_snake->set_direction('b');};
        }
        else    //если мы правее яблока цели
        {
            if(current_direction == 'l' && !nearest_walls[LEFT])  {return;};  //если мы движемся влево и нет стены, то ничего не меняем 
            if(current_direction == 'l' && nearest_walls[LEFT] && !nearest_walls[UP])
            //теперь нам мешает стена, но сверху нет стены, тогда просто пойдём вперёд(вверх)
            {
                operating_snake->set_direction('f');
            }
            if(current_direction == 'l' && nearest_walls[LEFT] && !nearest_walls[DOWN])
            //теперь нам мешает стена, но снизу нет стены, тогда просто пойдём вниз
            {
                operating_snake->set_direction('b');
            }
            if(current_direction == 'f' || current_direction == 'b')//надо повернуть на эту вертикаль
            {
                if(!nearest_walls[LEFT])
                    operating_snake->set_direction('l');    //иначе должны просто сместиться с этой вертикали
            }
            if(current_direction == 'r' && !nearest_walls[UP])   {operating_snake->set_direction('f');}
            else if(current_direction == 'r' && !nearest_walls[DOWN])   {operating_snake->set_direction('b');};
        }
    }
    else    //а что если мы не совпадаем с целью ни по вертикали ни по горизонтали
    {
        if((head_coords.first - current_closest_apple->get_coords().first)*(head_coords.first - current_closest_apple->get_coords().first) <
            (head_coords.second - current_closest_apple->get_coords().second)*(head_coords.second - current_closest_apple->get_coords().second))
        //если ближе вертикаль
        {
            if(head_coords.first > current_closest_apple->get_coords().first)  //если мы правее вертикали
            {
                if(current_direction == 'l' && !nearest_walls[LEFT])    {return;}   //путь свободен - идём по нему
                else if(current_direction == 'l' && nearest_walls[LEFT] && !nearest_walls[UP]) //доджим стену на пути
                {operating_snake->set_direction('f');}
                else if(current_direction == 'l' && nearest_walls[LEFT] && !nearest_walls[DOWN])
                {operating_snake->set_direction('b');}
                else if(current_direction == 'r') // на удачу повернем вверх(вперёд) или вниз
                {
                    if(rand() % 2)
                    {
                        operating_snake->set_direction('b');
                    }
                    else
                    {
                        operating_snake->set_direction('f');
                    }
                }
                else if(current_direction == 'f' || current_direction == 'b')
                {operating_snake->set_direction('l');}
            }
            else    //если мы левее вертикали
            {
                if(current_direction == 'r' && !nearest_walls[RIGHT])    {return;}   //путь свободен - идём по нему
                else if(current_direction == 'r' && nearest_walls[RIGHT] && !nearest_walls[UP]) //доджим стену на пути
                {operating_snake->set_direction('f');}
                else if(current_direction == 'r' && nearest_walls[RIGHT] && !nearest_walls[DOWN])
                {operating_snake->set_direction('b');}
                else if(current_direction == 'l') // на удачу повернем вверх(вперёд) или вниз
                {
                    if(rand() % 2)
                    {
                        operating_snake->set_direction('b');
                    }
                    else
                    {
                        operating_snake->set_direction('f');
                    }
                }
                else if(current_direction == 'f' || current_direction == 'b')
                {operating_snake->set_direction('r');}
            }
        }
        else    //если ближе горизонталь
        {
            if(head_coords.second > current_closest_apple->get_coords().second)  //если мы выше горизонтали
            {
                if(current_direction == 'b' && !nearest_walls[DOWN])    {return;}   //путь свободен - идём по нему
                else if(current_direction == 'b' && nearest_walls[DOWN] && !nearest_walls[LEFT]) //доджим стену на пути
                {operating_snake->set_direction('l');}
                else if(current_direction == 'b' && nearest_walls[DOWN] && !nearest_walls[RIGHT])
                {operating_snake->set_direction('r');}
                else if(current_direction == 'f') // на удачу повернем влево или вправо
                {
                    if(rand() % 2)
                    {
                        operating_snake->set_direction('l');
                    }
                    else
                    {
                        operating_snake->set_direction('r');
                    }
                }
                else if(current_direction == 'l' || current_direction == 'r')
                {operating_snake->set_direction('b');}
            }
            else    //если мы ниже горизонтали
            {
                if(current_direction == 'f' && !nearest_walls[UP])    {return;}   //путь свободен - идём по нему
                else if(current_direction == 'f' && nearest_walls[UP] && !nearest_walls[RIGHT]) //доджим стену на пути
                {operating_snake->set_direction('r');}
                else if(current_direction == 'f' && nearest_walls[UP] && !nearest_walls[LEFT])
                {operating_snake->set_direction('l');}
                else if(current_direction == 'b') // на удачу повернем вверх(вперёд) или вниз
                {
                    if(rand() % 2)
                    {
                        operating_snake->set_direction('l');
                    }
                    else
                    {
                        operating_snake->set_direction('r');
                    }
                }
                else if(current_direction == 'l' || current_direction == 'r')
                {operating_snake->set_direction('f');}
            }            
        }
    }
}

void Dumb_Bot_Controller::check_nearby_walls(int * walls, Model * my_model, std::pair <int, int> screen_size)
{
    std::list<Snake> * current_snakes_list = my_model->get_snakes();
    auto it = current_snakes_list->begin();   
    std::advance(it, snake_number);     //считаем змей с нуля
    Snake * operating_snake = &(*it); 
    Segment * head = operating_snake->get_segment(1);
    std::pair<int, int> head_coords = head->get_coords();
    //checking boreders
    if(head_coords.first >= screen_size.first - 1)  {walls[RIGHT] = 1;};
    if(head_coords.first <= 2)  {walls[LEFT] = 1;};
    if(head_coords.second >= screen_size.second - 1)  {walls[UP] = 1;};
    if(head_coords.second <= 2)  {walls[DOWN] = 1;};
    //checking other snakes and selves body
    std::pair <int, int> up_coords (head_coords.first, head_coords.second + 1);
    std::pair <int, int> left_coords (head_coords.first - 1, head_coords.second);
    std::pair <int, int> down_coords (head_coords.first, head_coords.second - 1);
    std::pair <int, int> right_coords (head_coords.first + 1, head_coords.second);
    for(auto sn = current_snakes_list->begin(); sn != current_snakes_list->end(); sn++)
    {
        Segment * buf_segment;
        int i = 1;
        do
        {
            buf_segment = sn->get_segment(i);
            if(up_coords == buf_segment->get_coords())  {walls[UP] = 1;};
            if(left_coords == buf_segment->get_coords())  {walls[LEFT] = 1;};
            if(down_coords == buf_segment->get_coords())  {walls[DOWN] = 1;};
            if(right_coords == buf_segment->get_coords())  {walls[RIGHT] = 1;};
            i++;
        } while (buf_segment != sn->get_segment(-1));
    }
    //checking moving walls
    std::list<Wall> * current_walls_list = my_model->get_walls();
    for(auto w = current_walls_list->begin(); w != current_walls_list->end(); w++)
    {
        Segment * buf_block;
        int i = 1;
        do
        {
            buf_block = w->get_block(i);
            if(up_coords == buf_block->get_coords())  {walls[UP] = 1;};
            if(left_coords == buf_block->get_coords())  {walls[LEFT] = 1;};
            if(down_coords == buf_block->get_coords())  {walls[DOWN] = 1;};
            if(right_coords == buf_block->get_coords())  {walls[RIGHT] = 1;};
            i++;            
        } while (buf_block != w->get_block(-1));
    }
    return;
}

void Dumb_Bot_Controller::process_input(char c, Model * my_model, std::pair<int, int> screen_size, int snake_ind)
{
    snake_number = snake_ind;
    get_closest_apple(my_model);
    decide_where_to_go(my_model, screen_size);
}

Smart_Bot_Controller::Smart_Bot_Controller()
{

}

void Smart_Bot_Controller::get_closest_apple(Model * my_model)
{
    std::list<Snake> * current_snakes_list = my_model->get_snakes();
    auto it = current_snakes_list->begin();
    std::advance(it, snake_number);     //считаем змей с нуля
    Snake * operating_snake = &(*it);
    if(operating_snake->is_alive() == 0)    //мёртвая змея
        return;

    std::list<Apple> * current_apples_list = my_model->get_apples();
    int min_distance = 1000000;
    Apple * closest_apple = nullptr;
    for(auto ap = current_apples_list->begin(); ap != current_apples_list->end(); ap++)
    {
        int x = ap->get_coords().first - operating_snake->get_segment(1)->get_coords().first;
        int y = ap->get_coords().second - operating_snake->get_segment(1)->get_coords().second;
        if(x*x + y*y < min_distance)
        {
            closest_apple = &(*ap);
            min_distance = x*x + y*y;
        }
    }
    current_closest_apple = closest_apple;
} 

void Smart_Bot_Controller::decide_where_to_go(Model * my_model, std::pair <int, int> screen_size)
{
    int nearest_walls[4] = {0, 0, 0, 0};
    std::list<Snake> * current_snakes_list = my_model->get_snakes();
    auto it = current_snakes_list->begin();   
    std::advance(it, snake_number);     //считаем змей с нуля
    Snake * operating_snake = &(*it);

    if(operating_snake->is_alive() == 0)    //мёртвая змея
        return;

    check_nearby_walls(nearest_walls, my_model, screen_size);
    //totally traped:
    if(nearest_walls[UP] == 1 && nearest_walls[LEFT] == 1 && nearest_walls[DOWN] == 1 && nearest_walls[RIGHT] == 1)
        return; //do nothing and just die
    //очевидные ситуации с единственным выходом:
    if(nearest_walls[UP] && nearest_walls[RIGHT] && nearest_walls[DOWN] && !nearest_walls[LEFT])
    {
        operating_snake->set_direction('l');
        return;
    }
    if(nearest_walls[UP] && nearest_walls[RIGHT] && !nearest_walls[DOWN] && nearest_walls[LEFT])
    {
        operating_snake->set_direction('b');
        return;
    }
    if(nearest_walls[UP] && !nearest_walls[RIGHT] && nearest_walls[DOWN] && nearest_walls[LEFT])
    {
        operating_snake->set_direction('r');
        return;
    }
    if(!nearest_walls[UP] && nearest_walls[RIGHT] && nearest_walls[DOWN] && !nearest_walls[LEFT])
    {
        operating_snake->set_direction('f');
        return;
    }
    //when there are some options:
    std::pair<int, int> head_coords= operating_snake->get_segment(1)->get_coords();
    char current_direction = operating_snake->get_direction();
    if(head_coords.first == current_closest_apple->get_coords().first) //если мы на одной вертикали с яблоком-целью
    {
        if(head_coords.second < current_closest_apple->get_coords().second)    //если мы ниже яблока-цели
        {
            if(current_direction == 'f' && !nearest_walls[UP])  {return;};  //если мы движемся наверх(вперёд) и нет впереди стены, то ничего не меняем 
            if(current_direction == 'f' && nearest_walls[UP] && !nearest_walls[RIGHT])
            //теперь нам мешает стена спереди, но справа нет стены, тогда просто пойдём вправо
            {
                operating_snake->set_direction('r');
            }
            if(current_direction == 'f' && nearest_walls[UP] && !nearest_walls[RIGHT])
            //теперь нам мешает стена спереди, но слева нет стены, тогда просто пойдём влево
            {
                operating_snake->set_direction('l');
            }
            if(current_direction == 'l' || current_direction == 'r')//надо повернуть на эту вертикаль
            {
                if(!nearest_walls[UP])
                    operating_snake->set_direction('f');    //иначе должны просто сместиться с этой вертикали
                else if(!nearest_walls[DOWN] && (nearest_walls[LEFT] || nearest_walls[RIGHT]))
                    operating_snake->set_direction('b');
            }
            if(current_direction == 'b' && !nearest_walls[RIGHT])   {operating_snake->set_direction('r');}
            else if(current_direction == 'b' && !nearest_walls[LEFT])   {operating_snake->set_direction('l');};
        }
        else    //если мы выше яблока цели
        {
            if(current_direction == 'b' && !nearest_walls[DOWN])  {return;};  //если мы движемся вниз(назад) и нет стены, то ничего не меняем 
            if(current_direction == 'b' && nearest_walls[DOWN] && !nearest_walls[RIGHT])
            //теперь нам мешает стена, но справа нет стены, тогда просто пойдём вправо
            {
                operating_snake->set_direction('r');
            }
            if(current_direction == 'b' && nearest_walls[DOWN] && !nearest_walls[RIGHT])
            //теперь нам мешает стена, но слева нет стены, тогда просто пойдём влево
            {
                operating_snake->set_direction('l');
            }
            if(current_direction == 'l' || current_direction == 'r')//надо повернуть на эту вертикаль
            {
                if(!nearest_walls[DOWN])
                    operating_snake->set_direction('b');    //иначе должны просто сместиться с этой вертикали
                else if(!nearest_walls[UP] && (nearest_walls[LEFT] || nearest_walls[RIGHT]))
                    operating_snake->set_direction('f');
            }
            if(current_direction == 'f' && !nearest_walls[RIGHT])   {operating_snake->set_direction('r');}
            else if(current_direction == 'f' && !nearest_walls[LEFT])   {operating_snake->set_direction('l');};
        }
    }
    else if(head_coords.second == current_closest_apple->get_coords().second)  //если мы с яблоком-целью на одной горизонтали
    {
        if(head_coords.first < current_closest_apple->get_coords().first)    //если мы левее яблока-цели
        {
            if(current_direction == 'r' && !nearest_walls[RIGHT])  {return;};  //если мы движемся вправо и нет стены, то ничего не меняем 
            if(current_direction == 'r' && nearest_walls[RIGHT] && !nearest_walls[UP])
            //теперь нам мешает стена, но сверху нет стены, тогда просто пойдём верх
            {
                operating_snake->set_direction('f');
            }
            if(current_direction == 'r' && nearest_walls[UP] && !nearest_walls[DOWN])
            //теперь нам мешает стена, но снизу нет стены, тогда просто пойдём вниз
            {
                operating_snake->set_direction('b');
            }
            if(current_direction == 'f' || current_direction == 'b')//надо повернуть на эту горизонталь
            {
                if(!nearest_walls[RIGHT])
                    operating_snake->set_direction('r');    //иначе должны просто сместиться с этой горизонтали
                else if(!nearest_walls[LEFT] && (nearest_walls[UP] || nearest_walls[DOWN]))
                    operating_snake->set_direction('l');

            }
            if(current_direction == 'l' && !nearest_walls[UP])   {operating_snake->set_direction('f');}
            else if(current_direction == 'l' && !nearest_walls[DOWN])   {operating_snake->set_direction('b');};
        }
        else    //если мы правее яблока цели
        {
            if(current_direction == 'l' && !nearest_walls[LEFT])  {return;};  //если мы движемся влево и нет стены, то ничего не меняем 
            if(current_direction == 'l' && nearest_walls[LEFT] && !nearest_walls[UP])
            //теперь нам мешает стена, но сверху нет стены, тогда просто пойдём вперёд(вверх)
            {
                operating_snake->set_direction('f');
            }
            if(current_direction == 'l' && nearest_walls[LEFT] && !nearest_walls[DOWN])
            //теперь нам мешает стена, но снизу нет стены, тогда просто пойдём вниз
            {
                operating_snake->set_direction('b');
            }
            if(current_direction == 'f' || current_direction == 'b')//надо повернуть на эту вертикаль
            {
                if(!nearest_walls[LEFT])
                    operating_snake->set_direction('l');    //иначе должны просто сместиться с этой вертикали
                else if(!nearest_walls[RIGHT] && (nearest_walls[UP] || nearest_walls[DOWN]))
                    operating_snake->set_direction('l');
            }
            if(current_direction == 'r' && !nearest_walls[UP])   {operating_snake->set_direction('f');}
            else if(current_direction == 'r' && !nearest_walls[DOWN])   {operating_snake->set_direction('b');};
        }
    }
    else    //а что если мы не совпадаем с целью ни по вертикали ни по горизонтали
    {
        if((head_coords.first - current_closest_apple->get_coords().first)*(head_coords.first - current_closest_apple->get_coords().first) <
            (head_coords.second - current_closest_apple->get_coords().second)*(head_coords.second - current_closest_apple->get_coords().second))
        //если ближе вертикаль
        {
            if(head_coords.first > current_closest_apple->get_coords().first)  //если мы правее вертикали
            {
                if(current_direction == 'l' && !nearest_walls[LEFT])    {return;}   //путь свободен - идём по нему
                else if(current_direction == 'l' && nearest_walls[LEFT] && !nearest_walls[UP]) //доджим стену на пути
                {operating_snake->set_direction('f');}
                else if(current_direction == 'l' && nearest_walls[LEFT] && !nearest_walls[DOWN])
                {operating_snake->set_direction('b');}
                else if(current_direction == 'r') // повернём уже здраво в отличие от глупого бота
                {
                    if(!nearest_walls[DOWN])
                    {
                        operating_snake->set_direction('b');
                    }
                    else
                    {
                        operating_snake->set_direction('f');
                    }
                }
                else if(current_direction == 'f' || current_direction == 'b')   //тут тоже оценим здраво в отличии от глупого бота
                {
                    if(!nearest_walls[LEFT])
                        operating_snake->set_direction('l');
                }
            }
            else    //если мы левее вертикали
            {
                if(current_direction == 'r' && !nearest_walls[RIGHT])    {return;}   //путь свободен - идём по нему
                else if(current_direction == 'r' && nearest_walls[RIGHT] && !nearest_walls[UP]) //доджим стену на пути
                {operating_snake->set_direction('f');}
                else if(current_direction == 'r' && nearest_walls[RIGHT] && !nearest_walls[DOWN])
                {operating_snake->set_direction('b');}
                else if(current_direction == 'l') // здраво повернём
                {
                    if(!nearest_walls[DOWN])
                    {
                        operating_snake->set_direction('b');
                    }
                    else
                    {
                        operating_snake->set_direction('f');
                    }
                }
                else if(current_direction == 'f' || current_direction == 'b')
                {
                    if(!nearest_walls[RIGHT])
                        operating_snake->set_direction('r');
                }
            }
        }
        else    //если ближе горизонталь
        {
            if(head_coords.second > current_closest_apple->get_coords().second)  //если мы выше горизонтали
            {
                if(current_direction == 'b' && !nearest_walls[DOWN])    {return;}   //путь свободен - идём по нему
                else if(current_direction == 'b' && nearest_walls[DOWN] && !nearest_walls[LEFT]) //доджим стену на пути
                {operating_snake->set_direction('l');}
                else if(current_direction == 'b' && nearest_walls[DOWN] && !nearest_walls[RIGHT])
                {operating_snake->set_direction('r');}
                else if(current_direction == 'f') // здраво повернем влево или вправо
                {
                    if(!nearest_walls[LEFT])
                    {
                        operating_snake->set_direction('l');
                    }
                    else
                    {
                        operating_snake->set_direction('r');
                    }
                }
                else if(current_direction == 'l' || current_direction == 'r')   //более здоровая оценка
                {
                    if(!nearest_walls[DOWN])
                        operating_snake->set_direction('b');
                }
            }
            else    //если мы ниже горизонтали
            {
                if(current_direction == 'f' && !nearest_walls[UP])    {return;}   //путь свободен - идём по нему
                else if(current_direction == 'f' && nearest_walls[UP] && !nearest_walls[RIGHT]) //доджим стену на пути
                {operating_snake->set_direction('r');}
                else if(current_direction == 'f' && nearest_walls[UP] && !nearest_walls[LEFT])
                {operating_snake->set_direction('l');}
                else if(current_direction == 'b') // здраво повернем вверх(вперёд) или вниз
                {
                    if(!nearest_walls[LEFT])
                    {
                        operating_snake->set_direction('l');
                    }
                    else
                    {
                        operating_snake->set_direction('r');
                    }
                }
                else if(current_direction == 'l' || current_direction == 'r')   //более здоровая оценка
                {
                    if(!nearest_walls[UP])
                        operating_snake->set_direction('f');
                }
            }            
        }
    }
}

void Smart_Bot_Controller::check_nearby_walls(int * walls, Model * my_model, std::pair <int, int> screen_size)
{
    std::list<Snake> * current_snakes_list = my_model->get_snakes();
    auto it = current_snakes_list->begin();   
    std::advance(it, snake_number);     //считаем змей с нуля
    Snake * operating_snake = &(*it); 
    Segment * head = operating_snake->get_segment(1);
    std::pair<int, int> head_coords = head->get_coords();
    //checking boreders
    if(head_coords.first >= screen_size.first - 1)  {walls[RIGHT] = 1;};
    if(head_coords.first <= 2)  {walls[LEFT] = 1;};
    if(head_coords.second >= screen_size.second - 1)  {walls[UP] = 1;};
    if(head_coords.second <= 2)  {walls[DOWN] = 1;};
    //checking other snakes and selves body
    std::pair <int, int> up_coords (head_coords.first, head_coords.second + 1);
    std::pair <int, int> left_coords (head_coords.first - 1, head_coords.second);
    std::pair <int, int> down_coords (head_coords.first, head_coords.second - 1);
    std::pair <int, int> right_coords (head_coords.first + 1, head_coords.second);
    for(auto sn = current_snakes_list->begin(); sn != current_snakes_list->end(); sn++)
    {
        Segment * buf_segment;
        int i = 1;
        do
        {
            buf_segment = sn->get_segment(i);
            if(up_coords == buf_segment->get_coords())  {walls[UP] = 1;};
            if(left_coords == buf_segment->get_coords())  {walls[LEFT] = 1;};
            if(down_coords == buf_segment->get_coords())  {walls[DOWN] = 1;};
            if(right_coords == buf_segment->get_coords())  {walls[RIGHT] = 1;};
            i++;
        } while (buf_segment != sn->get_segment(-1));
    }
    //checking moving walls
    std::list<Wall> * current_walls_list = my_model->get_walls();
    for(auto w = current_walls_list->begin(); w != current_walls_list->end(); w++)
    {
        Segment * buf_block;
        int i = 1;
        do
        {
            buf_block = w->get_block(i);
            if(up_coords == buf_block->get_coords())  {walls[UP] = 1;};
            if(left_coords == buf_block->get_coords())  {walls[LEFT] = 1;};
            if(down_coords == buf_block->get_coords())  {walls[DOWN] = 1;};
            if(right_coords == buf_block->get_coords())  {walls[RIGHT] = 1;};
            i++;            
        } while (buf_block != w->get_block(-1));
    }
    return;
}

void Smart_Bot_Controller::process_input(char c, Model * my_model, std::pair<int, int> screen_size, int snake_ind)
{
    snake_number = snake_ind;
    get_closest_apple(my_model);
    decide_where_to_go(my_model, screen_size);
}