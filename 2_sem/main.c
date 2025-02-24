#include <stdio.h>
#include "cat.h"
#include "dog.h"
#include "animal.h"

int main()
{
    struct animal * ct = (struct animal *)cat_create("Murka");
    //struct animal * dg = cat_create("Bob");
    ct->m->voice(ct);

    struct animal * dg = (struct animal *)dog_create("Baks");
    dg->m->voice(dg);
    return 0;
}