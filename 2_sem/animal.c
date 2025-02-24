#include "animal.h"
#include <stdlib.h>
#include <stdio.h>

void animal_setname(struct animal * a)
{
    char * buf = calloc(1, MAX_ANIMAL_NAME_SIZE);
    printf("Please, enter animal's name:\n");
    scanf("%s", buf);
    a->name = buf;
}

char * animal_getname(struct animal * a)
{
    return a->name;
}