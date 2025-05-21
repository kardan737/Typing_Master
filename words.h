#ifndef WORD_H
#define WORD_H

#define MAX_WORD_LENGTH 50
#define MAX_NOUNS 350 
#define MAX_ADJECTIVES 150 

typedef struct {
    char text[MAX_WORD_LENGTH];
    float x;
    int y;
    int active;
    int typed;
    float prev_x;
    int word_count;
    int direction; // 1 for left-to-right, -1 for right-to-left
} Word;

const char* get_random_word();


void load_words_from_file(const char* filename);
void load_adjectives_from_file(const char* filename);


extern char loaded_words[MAX_NOUNS][MAX_WORD_LENGTH];
extern int noun_count;


extern char loaded_adjectives[MAX_ADJECTIVES][MAX_WORD_LENGTH];
extern int adjective_count;

#endif // WORD_H
