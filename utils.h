#ifndef UTILS_H
#define UTILS_H

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25
#define MAX_WORDS 10
#define WORD_SPEED 0.5f
#define BEEP_FREQUENCY 750
#define BEEP_DURATION 200

#define COLOR_BLACK 0
#define COLOR_WHITE 15
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_BLUE 9
#define COLOR_CYAN 11
#define COLOR_INVERTED 112
#define BACKGROUND_RED 64

void set_color(int color);
void set_cursor_position(int x, int y);
void clear_screen();
void play_error_sound();
void flash_screen();

#endif // UTILS_H
