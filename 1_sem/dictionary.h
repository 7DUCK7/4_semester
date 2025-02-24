#define MAX_DICTIONARY_SIZE 1024

struct dictionary;

struct dictionary * dictionary_create();
void dictionary_add_new_word(struct dictionary * dic, char * wrd);
void dictionary_add_existing_word(struct dictionary * dic, int ind);
int dictionary_check(struct dictionary * dic, char * wrd);
void dictionary_destroy(struct dictionary * dic);
void dictionary_sort_by_frequency(struct dictionary * dic);
void dictionary_display(struct dictionary * dic);

