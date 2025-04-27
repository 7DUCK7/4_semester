#ifndef MODEL_H
#define MODEL_H

#include "tank.h"
#include "block.h"
#include <vector>

class Model
{
    private:
    std::vector<Tank *> tanks;
    std::vector<Block *> walls; //blocks that restrict tank's movement
    std::vector<Block *> base;
    int block_size;
    int sub_block_size;
    int tank_size;
    public:
    Model();
    std::vector<Tank *>* get_tanks_vect_ptr();
    std::vector<Block *>* get_walls_vect_ptr();
    void add_tank_to_vect(Tank* t);
    void set_block_size(int n);
    void set_sub_block_size(int n);
    void set_tank_size(int n);
    int get_block_size();
    int get_sub_block_size();
    int get_tank_size();
    std::vector<Tank *> get_tanks_vect();
};


#endif 