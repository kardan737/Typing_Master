#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#include "game.h"
#include "ui.h"
#include "utils.h"
#include "words.h"
#include "stats.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    char mode_command[50];
    sprintf(mode_command, "mode con: cols=%d lines=%d", CONSOLE_WIDTH, CONSOLE_HEIGHT);
    system(mode_command);

    srand(time(NULL));

    Stats stats = {0};
    float word_speed = WORD_SPEED;
    int lives = 3;
    int menu_choice = 0;

    while (1) {
        show_menu();
        menu_choice = _getch() - '0';

        switch (menu_choice) {
            case 1:
                run_game(&stats, word_speed, lives);
                break;

            case 2:
                show_settings_menu(&word_speed, &lives);
                break;

            case 3:
                show_statistics(stats);
                break;

            case 4:
                return 0;

            default:
                set_cursor_position(CONSOLE_WIDTH/2 - 15, 14);
                printf("Invalid option! Press any key to continue...");
                _getch();
        }
    }

    return 0;
}
