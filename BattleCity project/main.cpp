#include "map.h"
#include "GUI.h"
#include "sprites.h"
#include "model.h"
#include "controller.h"
#include <string.h>

int main(int argc, char* argv[])
{
    srand(time(0));
    Sprites my_sprites = Sprites();
    //std::cout << "I'm alive " << __FILE__ << ' ' << __LINE__ << std::endl;
    if(argc < 2 || argv[1][0] > '2' || argv[1][0] < '1')
    {
        std::cout << "Please choose how many players will be playing (1 or 2) and write this number as the first cmd argument" << std::endl;
        return -1;
    }
    Map my_map = Map();
    Model my_model = Model(((int)argv[1][0]) - 48);
    GUI my_GUI = GUI(&my_map, &my_model, &my_sprites);
    my_GUI.set_first_player_controller(Controller::get_controller('1'));
    my_GUI.set_second_player_controller(Controller::get_controller('2'));
    my_GUI.set_enemy_1_bot_controller(Controller::get_controller(1));
    my_GUI.run();
    return 0;
}