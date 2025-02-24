#include "dictionary.h"
#include "word.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct dictionary
{
    struct word * words[MAX_DICTIONARY_SIZE];
    int num_of_words;
};

struct dictionary * dictionary_create()
{
    struct dictionary * buf;
    buf = (struct dictionary *)calloc(1, sizeof(struct dictionary));
    return buf;
};

void dictionary_add_new_word(struct dictionary * dic, char * wrd)
{
    dic->num_of_words++;
    dic->words[dic->num_of_words - 1] = word_create(wrd);
    return;
};

void dictionary_add_existing_word(struct dictionary * dic, int ind)
{
    word_inc_count((dic->words)[ind]);
    return;
}

int dictionary_check(struct dictionary * dic, char * wrd)
{
    for(int i = 0; i < dic->num_of_words; i++)
    {
        if(strcmp(word_get_str((dic->words)[i]), wrd) == 0)
        return i;
    }
    return -1;
};

void dictionary_destroy(struct dictionary * dic)
{
    for(int i = 0; i < dic->num_of_words; i++)
    {
        word_destroy((dic->words)[i]);
    }
    free(dic);
    return;
}

void dictionary_sort_by_frequency(struct dictionary * dic)
{
    //printf("\ndic = %d\n", dic);
    int n = dic->num_of_words;
    //printf("\ndo I even get here?\n");
    //printf("dic->num_of_words = %d", n);
    for(int i = 1; i < n; i++)
    {
        int is_sorted = 1;
        for(int j = 0; j < n - i; j++)
        {
            if(word_get_count(dic->words[j]) > word_get_count(dic->words[j + 1]))
            {
                struct word * tmp = dic->words[j];
                dic->words[j] = dic->words[j + 1];
                dic->words[j + 1] = tmp;
                is_sorted = 0;
                //printf("1 sorting step passed\n");
            }
        }
        if(is_sorted)
            break;
    }
    return;
}

void dictionary_display(struct dictionary * dic)
{
    int n = dic->num_of_words;
    for(int i = 0; i < n; i++)
    {
        printf("%-30s%d\n", word_get_str((dic->words)[i]), word_get_count((dic->words)[i]));
    }
    return;
}
