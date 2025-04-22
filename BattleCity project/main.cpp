#include "map.h"
#include "GUI.h"
#include "sprites.h"

int main()
{
    Sprites my_sprites = Sprites();
    //std::cout << "I'm alive " << __FILE__ << ' ' << __LINE__ << std::endl;
    
    Map my_map = Map(&my_sprites);
    
    GUI my_GUI = GUI(&my_map);
    
    my_GUI.run();
    return 0;
}