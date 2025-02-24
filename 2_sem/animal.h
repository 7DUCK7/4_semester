#define MAX_ANIMAL_NAME_SIZE 64

struct animal
{
    char * name;
    struct animal_methods * m;
};

struct animal_methods
{
    void (*voice)(struct animal *);
};

typedef void (*voice)(struct animal *);

void animal_setname(struct animal * a);
char * animal_getname(struct animal * a);