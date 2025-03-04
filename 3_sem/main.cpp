#include "view.h"

int main(int ac, char* av[])
{
    View *v = View::getView((av[1])[0]);
    v->draw();
    return 0;
}