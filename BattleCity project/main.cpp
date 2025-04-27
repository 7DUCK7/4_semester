#include "map.h"
#include "GUI.h"
#include "sprites.h"
#include "model.h"
#include "controller.h"
int main()
{
    Sprites my_sprites = Sprites();
    //std::cout << "I'm alive " << __FILE__ << ' ' << __LINE__ << std::endl;
    
    Map my_map = Map();
    Model my_model = Model();
    GUI my_GUI = GUI(&my_map, &my_model, &my_sprites);
    my_GUI.set_first_player_controller(Controller::get_controller('1'));
    my_GUI.run();
    return 0;
}