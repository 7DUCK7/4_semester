#include "parser.h"
#include "word.h"
#include <string.h>
#include <stdlib.h>

struct parser
{
    char buf_word[MAX_WORD_LENGTH];
    FILE* current_fd;
    char* current_file_name;
};

struct parser * parser_create()
{
    struct parser * buf = (struct parser *)calloc(1, sizeof(struct parser));
    return buf;
};

void parser_destroy(struct parser * pars)
{
    free(pars);
    return;
};

int parser_open(struct parser * pars, char * file_name)
{
    pars->current_file_name = file_name;
    if(file_name != NULL)
    {
        if((pars->current_fd = fopen(file_name, "r")) == NULL)
            return -1;
    }
    else
    {
        pars->current_fd = stdin;
    }   
    return 0;
}

char * parser_get_one_word(struct parser * pars)
{
    char* buf = NULL;
    buf = (char*)calloc(100, 1);

    char c = 1;
    int i = 0;
    while(!(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) && c > 0)
    {
        c = fgetc(pars->current_fd);
    }

    if(c <= 0)
    {
        free(buf);
        return NULL;
    }
    
    while((('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) && c > 0)
    {
        buf[i] = c;
        i++;
        if(i >= MAX_WORD_LENGTH)
        {
            printf("too long word found(max_word_size = %d)\n saved only part of it\n", MAX_WORD_LENGTH);
            buf[i] = 0;
            break;
        }
        c = fgetc(pars->current_fd);
    }

    buf[i + 1] = 0;
    return buf;
}

void parser_close(struct parser * pars)
{
    fclose(pars->current_fd);
    return;
}

char* parser_get_file_name(struct parser * pars)
{
    return pars->current_file_name;
}