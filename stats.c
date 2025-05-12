#include <time.h>
#include "stats.h"
#include "utils.h"
#include <stdio.h>

int calculate_wpm(int score, time_t start_time) {
    time_t current_time = time(NULL);
    double minutes = difftime(current_time, start_time) / 60.0;
    return minutes > 0 ? (int)(score / minutes) : 0;
}

void show_stats(int score, int lives, int wpm, int accuracy) {
    set_cursor_position(0, CONSOLE_HEIGHT - 1);
    set_color(COLOR_YELLOW);
    printf("Score: %d | Lives: %d | WPM: %d | Accuracy: %d%%", score, lives, wpm, accuracy);
}
