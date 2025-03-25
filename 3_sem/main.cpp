#include "view.h"
#include "model.h"
#include "controller.h"
#include <iostream>
int main(int ac, char* av[])
{
    View *v;
    if((ac <= 1) || ((v = View::getView((av[1])[0])) == NULL))
    {
        std::cout << "no interface was chosen(add flag t to use text interface)\n"; 
        return 0;
    }
    Model* m;
    std::pair<int, int> p(9, 9);
    m = new Model(1, p);

    Controller * c;
    c = new Controller();

    v->setmodel(m);
    v->setcontroller(c);
    v->run();
    return 0;
}