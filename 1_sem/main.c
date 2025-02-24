#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "word.h"
#include "dictionary.h"

void parse(struct parser * my_parser, struct dictionary * my_dictionary);

int main(int argc, char * argv[])
{
    struct parser * my_parser;
    my_parser = parser_create();

    struct dictionary * my_dictionary = NULL;
    my_dictionary = dictionary_create();

    if(argc == 1)
    {
        parser_open(my_parser, NULL);
        printf("To stop input write this word: stopp\n");
        printf("P.S. This word won't be counted\n");
        parse(my_parser, my_dictionary);
        parser_close(my_parser);
    }
    else
    {
        for(int i = 1; i < argc; i++)
        {
            if(parser_open(my_parser, argv[i]) < 0)
            {
                printf("coudn't open this file: %s\n", argv[i]);
                return -1;
            }
            parse(my_parser, my_dictionary);
            parser_close(my_parser);
        }
    }

    dictionary_sort_by_frequency(my_dictionary);
    dictionary_display(my_dictionary);
    
    parser_destroy(my_parser);
    dictionary_destroy(my_dictionary);
    return 0;
}

void parse(struct parser * my_parser, struct dictionary * my_dictionary)
{
    char * buf = NULL;
    char stopp_word[6] = "stopp";
    int buf_int = 0;
    if(parser_get_file_name(my_parser) != NULL)
        stopp_word[0] = 0;

    while(((buf = parser_get_one_word(my_parser)) != NULL) && strcmp(buf, stopp_word))
    {
        if((buf_int = dictionary_check(my_dictionary, buf)) < 0)
        {
            dictionary_add_new_word(my_dictionary, buf);
        }
        else
        {
            dictionary_add_existing_word(my_dictionary, buf_int);
        }
    }
    return;
}