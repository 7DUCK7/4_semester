#include <stdio.h>

struct parser;

struct parser * parser_create();
void parser_destroy(struct parser * pars);
int parser_open(struct parser * pars, char * file_name);
char * parser_get_one_word(struct parser * pars);
void parser_close(struct parser * pars);
char* parser_get_file_name(struct parser * pars);