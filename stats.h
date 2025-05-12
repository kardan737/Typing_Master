#ifndef STATS_H
#define STATS_H

#include <time.h>

typedef struct {
    int total_words;
    int correct_words;
    int wpm;
    int accuracy;
} Stats;

int calculate_wpm(int score, time_t start_time);
void show_stats(int score, int lives, int wpm, int accuracy);

#endif // STATS_H
