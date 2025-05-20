#ifndef STATS_H
#define STATS_H

#include <time.h>

typedef struct {
    int total_words;
    int correct_words;
    int wpm;
    int accuracy;
    float start_speed;
} Stats;

int calculate_wpm(int score, time_t start_time);
void show_stats(int score, int lives, int wpm, int accuracy);
void save_stats(const Stats* stats, const char* filename);
void load_stats(Stats* stats, const char* filename);

#endif // STATS_H
