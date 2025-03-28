#include "controller.h"
#include "model.h"

#include <poll.h>
#include <time.h>
#include <list>

Controller::Controller()
{
    tcgetattr(STDIN_FILENO, &original);
    raw = original;
    cfmakeraw(&raw);    //переводим терминал в raw режим для посимвольного чтения ввода
    raw.c_lflag &= ~ECHO;   //отключаем ECHO, чтобы ввод в терминал не выводился на экран
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void Controller::disable_custom_controller()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original);
    return;
}

void Controller::process_input(char c, Model * my_model)
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
    }
    return;
}

long Controller::wait_input(Model * my_model)
{
    char read_symbol;
    struct timespec start, end;
    long time_wasted = 5*100000000;
    clock_gettime(CLOCK_MONOTONIC, &start);
    int n = poll(&fd_in, 1, 500);
    clock_gettime(CLOCK_MONOTONIC, &end);
    if(n == 1)
    {
        read(0, &read_symbol, 1);
        process_input(read_symbol, my_model);
        time_wasted = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    }
    tcflush(STDIN_FILENO, TCIFLUSH);
    return time_wasted;  // наносекунды
}