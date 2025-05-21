#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "ui.h"
#include "constants.h"
#include "words.h"
#include "stats.h"
#include "platform.h"

int main() {
    platform_init();
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    load_words_from_file("nouns.txt");
    load_adjectives_from_file("adjectives.txt");

    char mode_command[50];
    sprintf(mode_command, "mode con: cols=%d lines=%d", CONSOLE_WIDTH, CONSOLE_HEIGHT);
    system(mode_command);

    srand(time(NULL));

    Stats stats = {0};
    Stats best_stats = {0};
    float word_speed = WORD_SPEED;
    int lives = 3;
    int menu_choice = 0;
    int word_direction_mode = 0; // 0: Mixed, 1: LTR, -1: RTL

    load_stats(&best_stats, "stats.dat");

    while (1) {
        show_menu();
        int key = platform_get_key();
        if (key == 27) { 
            break;
        }
        menu_choice = key - '0';

        switch (menu_choice) {
            case 1:
                stats.total_words = 0;
                stats.correct_words = 0;
                stats.wpm = 0;
                stats.accuracy = 0;
                stats.start_speed = word_speed;
                run_game(&stats, word_speed, lives, word_direction_mode);
                if (stats.correct_words > best_stats.correct_words) {
                    best_stats = stats;
                }
                break;

            case 2:
                show_settings_menu(&word_speed, &lives, &word_direction_mode);
                break;

            case 3:
                show_statistics(stats, best_stats);
                break;

            case 4:
                break;

            default:
                platform_set_cursor_position(CONSOLE_WIDTH/2 - 15, 14);
                printf("Invalid option! Press any key to continue...");
                platform_get_key();
        }
        if (menu_choice == 4) break;
    }

    save_stats(&best_stats, "stats.dat");

    platform_cleanup();
    return 0;
}
