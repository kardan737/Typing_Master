#include <windows.h>
#include <conio.h>

#include <stdio.h>
#include "platform.h"
#include "constants.h"
#include <stdarg.h>

static HANDLE hConsole = NULL;
static WORD saved_attributes = 0;

void platform_init() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;
}

void platform_cleanup() {
    if (hConsole) SetConsoleTextAttribute(hConsole, saved_attributes);
}

void platform_set_color(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void platform_set_cursor_position(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(hConsole, coord);
}

void platform_clear_screen() {
    system("cls");
}

int platform_get_key() {
    return _getch();
}

int platform_key_pressed() {
    return _kbhit();
}

void platform_beep() {
    Beep(BEEP_FREQUENCY, BEEP_DURATION);
}

void platform_flash_screen() {
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    int x, y;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD orig_attr = consoleInfo.wAttributes;
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | COLOR_WHITE);
    for (y = 1; y < CONSOLE_HEIGHT - 2; y++) {
        for (x = 1; x < CONSOLE_WIDTH - 1; x++) {
            platform_set_cursor_position(x, y);
            printf(" ");
        }
    }
    Sleep(100);
    SetConsoleTextAttribute(hConsole, orig_attr);
    for (y = 1; y < CONSOLE_HEIGHT - 2; y++) {
        for (x = 1; x < CONSOLE_WIDTH - 1; x++) {
            platform_set_cursor_position(x, y);
            printf(" ");
        }
    }
}

void platform_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
} 