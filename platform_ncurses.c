#include "platform.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

static int color_pairs_initialized = 0;

static int map_color(int color) {
    switch (color) {
        case 15: return COLOR_WHITE;
        case 12: return COLOR_RED;
        case 10: return COLOR_GREEN;
        case 14: return COLOR_YELLOW;
        case 9:  return COLOR_BLUE;
        case 11: return COLOR_CYAN;
        case 0:  return COLOR_BLACK;
        default: return COLOR_WHITE;
    }
}

void platform_init() {
    initscr();
    start_color();
    use_default_colors();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, FALSE);
    curs_set(1);
    if (!color_pairs_initialized) {
        init_pair(1, COLOR_WHITE, -1);
        init_pair(2, COLOR_RED, -1);
        init_pair(3, COLOR_GREEN, -1);
        init_pair(4, COLOR_YELLOW, -1);
        init_pair(5, COLOR_BLUE, -1);
        init_pair(6, COLOR_CYAN, -1);
        init_pair(7, COLOR_BLACK, -1);
        color_pairs_initialized = 1;
    }
}

void platform_cleanup() {
    endwin();
}

void platform_set_color(int color) {
    int nc = map_color(color);
    int pair = 1;
    switch (nc) {
        case COLOR_WHITE: pair = 1; break;
        case COLOR_RED: pair = 2; break;
        case COLOR_GREEN: pair = 3; break;
        case COLOR_YELLOW: pair = 4; break;
        case COLOR_BLUE: pair = 5; break;
        case COLOR_CYAN: pair = 6; break;
        case COLOR_BLACK: pair = 7; break;
        default: pair = 1; break;
    }
    attron(COLOR_PAIR(pair));
}

void platform_set_cursor_position(int x, int y) {
    move(y, x);
}

void platform_clear_screen() {
    clear();
    refresh();
}

int platform_get_key() {
    nodelay(stdscr, FALSE);
    int ch = getch();

    if (ch == KEY_BACKSPACE) {
        return '\b';
    }
    return ch;
}

int platform_key_pressed() {
    nodelay(stdscr, TRUE);
    int ch = getch();
    if (ch == ERR) {
        nodelay(stdscr, FALSE);
        return 0;
    } else {
        ungetch(ch);
        nodelay(stdscr, FALSE);
        return 1;
    }
}

void platform_beep() {
    beep();
}

void platform_flash_screen() {
    flash();
}

void platform_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vw_printw(stdscr, fmt, args);
    va_end(args);
    refresh();
} 