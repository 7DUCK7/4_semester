#define MAX_WORD_LENGTH 256

struct word;

struct word * word_create(char * wrd);
void word_destroy(struct word * wrd);
char * word_get_str(struct word * wrd);
void word_inc_count(struct word * wrd);
int word_get_count(struct word * wrd);
 