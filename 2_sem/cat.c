#include "cat.h"
#include "animal.h"
#include <stdlib.h>
#include <stdio.h>

struct cat
{
    struct animal a;
};
struct animal_methods cat_methods = {(voice) cat_voice};


struct cat * cat_create()
{
    struct cat * buf = (struct cat *)calloc(1, sizeof(struct cat));
    ((struct animal *)buf)->m = &cat_methods;
    animal_setname(((struct animal *)buf));
    return buf;
}

void cat_destroy(struct cat * ct)
{
    free(ct);
    return;
}

void cat_voice(struct cat * ct)
{
    struct animal *a = (struct animal *)ct;
    const char * name = animal_getname(a);
    printf("cat %s says: meow!\n", name);
    return;
}
