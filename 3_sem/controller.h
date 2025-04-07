#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <termios.h>
#include <unistd.h>
#include <poll.h>
#define TIMEOUT 2*100000000
class Model;

class Controller
{
    private:
    struct termios original;
    struct termios raw;
    struct pollfd fd_in = {  .fd = STDIN_FILENO, .events = POLLIN};
    public:
    Controller();
    void disable_custom_controller();
    void process_input(char c, Model * my_model);
    long wait_input(Model * my_model);
};

#endif