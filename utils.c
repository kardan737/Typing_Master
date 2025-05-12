#include <windows.h>
#include <stdio.h>

#include "utils.h"

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void set_cursor_position(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clear_screen() {
    system("cls");
}

void play_error_sound() {
    Beep(BEEP_FREQUENCY, BEEP_DURATION);
}

void flash_screen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    int x, y;

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | COLOR_WHITE);
    for (y = 1; y < CONSOLE_HEIGHT - 2; y++) {
        for (x = 1; x < CONSOLE_WIDTH - 1; x++) {
            set_cursor_position(x, y);
            printf(" ");
        }
    }

    Sleep(100); 

    SetConsoleTextAttribute(hConsole, saved_attributes);
    for (y = 1; y < CONSOLE_HEIGHT - 2; y++) {
        for (x = 1; x < CONSOLE_WIDTH - 1; x++) {
            set_cursor_position(x, y);
            printf(" ");
        }
    }

}

