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
