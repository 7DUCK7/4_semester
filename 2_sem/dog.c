#include "dog.h"
#include "animal.h"
#include <stdlib.h>
#include <stdio.h>

struct dog
{
    struct animal a;
};
struct animal_methods dog_methods = {(voice) dog_voice};


struct dog * dog_create()
{
    struct dog * buf = (struct dog *)calloc(1, sizeof(struct dog));
    ((struct animal *)buf)->m = &dog_methods;
    animal_setname(((struct animal *)buf));
    return buf;
}

void dog_destroy(struct dog * dg)
{
    free(dg);
    return;
}

void dog_voice(struct dog * dg)
{
    struct animal *a = (struct animal *)dg;
    const char * name = animal_getname(a);
    printf("dog %s says: gav!\n", name);
    return;
}