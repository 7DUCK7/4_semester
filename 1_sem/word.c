#include "word.h"
#include <stdlib.h>
#include <string.h>

struct word
{
    char * str;
    int count;
};

struct word * word_create(char * wrd)
{
    struct word * buf;
    buf = (struct word *)calloc(1, sizeof(struct word));
    buf->count = 1;
    buf->str = wrd;
    return buf;
}

void word_destroy(struct word * wrd)
{
    free(wrd->str);
    free(wrd);
    return;
}

char * word_get_str(struct word * wrd)
{
    return wrd->str;
}

void word_inc_count(struct word * wrd)
{
    wrd->count++;
    return;
}

int word_get_count(struct word * wrd)
{
    return wrd->count;
}
 