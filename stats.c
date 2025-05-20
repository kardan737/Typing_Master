#include <time.h>
#include "stats.h"
#include "constants.h"
#include <stdio.h>
#include "platform.h"

int calculate_wpm(int score, time_t start_time) {
    time_t current_time = time(NULL);
    double minutes = difftime(current_time, start_time) / 60.0;
    return minutes > 0 ? (int)(score / minutes) : 0;
}

void show_stats(int score, int lives, int wpm, int accuracy) {
    platform_set_cursor_position(0, CONSOLE_HEIGHT - 1);
    platform_set_color(COLOR_YELLOW);
    platform_printf("Score: %d | Lives: %d | WPM: %d | Accuracy: %d%%", score, lives, wpm, accuracy);
}

void save_stats(const Stats* stats, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (f) {
        fprintf(f, "%d %d %d %d %.2f\n", stats->total_words, stats->correct_words, stats->wpm, stats->accuracy, stats->start_speed);
        fclose(f);
    }
}

void load_stats(Stats* stats, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f) {
        fscanf(f, "%d %d %d %d %f", &stats->total_words, &stats->correct_words, &stats->wpm, &stats->accuracy, &stats->start_speed);
        fclose(f);
    } else {
        stats->total_words = 0;
        stats->correct_words = 0;
        stats->wpm = 0;
        stats->accuracy = 0;
        stats->start_speed = 0.0f;
    }
}
