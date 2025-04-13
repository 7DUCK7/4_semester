#include "view.h"
#include "model.h"
#include "controller.h"
#include <iostream>

void how_to_quit_midgame();
int ask_players_number();
int ask_dumb_bots_number();
int ask_smart_bots_number();
int ask_walls();
void set_controllers(int number_of_players, int number_of_dumb_bots, int number_of_smart_bots, View * v);
int main(int ac, char* av[])
{
    srand(time(0));
    View *v;
    if((ac <= 1) || ((v = View::getView((av[1])[0])) == NULL))
    {
        std::cout << "no interface was chosen(add flag t to use text interface)\n"; 
        return 0;
    }
    how_to_quit_midgame();
    int number_of_players = ask_players_number(), number_of_dumb_bots = ask_dumb_bots_number(), number_of_smart_bots = ask_smart_bots_number();
    Model* m;
    m = new Model(number_of_players, number_of_dumb_bots, number_of_smart_bots, ask_walls());

    set_controllers(number_of_players, number_of_dumb_bots, number_of_smart_bots, v);

    v->setmodel(m);
    
    v->run();

    
    return 0;
}

void how_to_quit_midgame()
{
    std::cout << "To leave midgame press q \n\r";
}
int ask_players_number()
{
    std::cout << "Please enter, how many players will be playing(0,1 or 2 players) \r\n";
    int buf;
    std::cin >> buf;
    while(buf > 2 || buf < 0)
    {
        std::cout << "wrong number\n";
        std::cin >> buf;
    }

    return buf;
}

int ask_dumb_bots_number()
{
    std::cout << "Please enter, how many DUMB bots there will be playing(0 - 4 bots) \r\n";
    int buf;
    std::cin >> buf;
    while(buf > 4 || buf < 0)
    {
        std::cout << "wrong number\n";
        std::cin >> buf;
    }
    return buf;
}

int ask_smart_bots_number()
{
    std::cout << "Please enter, how many SMART bots there will be playing(0 - 4 bots) \r\n";
    int buf;
    std::cin >> buf;
    while(buf > 4 || buf < 0)
    {
        std::cout << "wrong number\n";
        std::cin >> buf;
    }
    return buf;
}

int ask_walls()
{
    std::cout << "Please enter, how many WALLS there will be (0 - 2 walls) \r\n";
    int buf;
    std::cin >> buf;
    while(buf > 2 || buf < 0)
    {
        std::cout << "wrong number\n";
        std::cin >> buf;
    }
    return buf; 
}

void set_controllers(int number_of_players, int number_of_dumb_bots, int number_of_smart_bots, View * v)
{
    switch (number_of_players)
    {
        case 1:
            v->set_first_player_controller(Controller::getController('w'));
            break;
        case 2:
            v->set_first_player_controller(Controller::getController('w'));
            v->set_second_player_controller(Controller::getController('i'));
            break;    
        default:
            break;
    }
    if(number_of_dumb_bots > 0)
        v->set_dumb_bot_controller(Controller::getController('d'));
    if(number_of_smart_bots > 0)
        v->set_smart_bot_controller(Controller::getController('s'));
}

