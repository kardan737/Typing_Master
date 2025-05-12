#ifndef WORD_H
#define WORD_H

#define MAX_WORD_LENGTH 50

typedef struct {
    char text[MAX_WORD_LENGTH];
    float x;
    int y;
    int active;
    int typed;
    float prev_x;
} Word;

const char* get_random_word();

#endif // WORD_H
