#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#include "ui.h"
#include "constants.h"
#include "stats.h"
#include "platform.h"

void draw_border() {
    platform_set_color(COLOR_YELLOW);
    for (int i = 0; i < CONSOLE_WIDTH; i++) {
        platform_set_cursor_position(i, 0);
        platform_printf("=");
        platform_set_cursor_position(i, CONSOLE_HEIGHT - 2);
        platform_printf("=");
    }
    for (int i = 0; i < CONSOLE_HEIGHT - 1; i++) {
        platform_set_cursor_position(0, i);
        platform_printf("|");
        platform_set_cursor_position(CONSOLE_WIDTH - 1, i);
        platform_printf("|");
    }
}

void show_menu() {
    platform_clear_screen();
    draw_border();

    platform_set_cursor_position(CONSOLE_WIDTH/2 - 7, 5);
    platform_set_color(COLOR_CYAN);
    platform_printf("TYPING TRAINER");

    platform_set_cursor_position(CONSOLE_WIDTH/2 - 6, 8);
    platform_set_color(COLOR_WHITE);
    platform_printf("1. Start Game");
    platform_set_cursor_position(CONSOLE_WIDTH/2 - 6, 9);
    platform_printf("2. Settings");
    platform_set_cursor_position(CONSOLE_WIDTH/2 - 6, 10);
    platform_printf("3. Statistics");
    platform_set_cursor_position(CONSOLE_WIDTH/2 - 6, 11);
    platform_printf("4. Exit");

    platform_set_cursor_position(CONSOLE_WIDTH/2 - 10, 13);
    platform_set_color(COLOR_YELLOW);
    platform_printf("Select menu item: ");
}

void show_settings_menu(float* word_speed, int* lives, int* word_direction_mode) {
    while (1) {
        platform_clear_screen();
        draw_border();

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 4, 5);
        platform_set_color(COLOR_CYAN);
        platform_printf("SETTINGS");

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 12, 8);
        platform_set_color(COLOR_WHITE);
        platform_printf("1. Word speed (current: %.1f)", *word_speed);

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 12, 9);
        platform_printf("2. Lives count (current: %d)", *lives);

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 12, 10);
        platform_printf("3. Word direction (current: %s)", (*word_direction_mode == 0) ? "Mixed" : ((*word_direction_mode == 1) ? "Left-to-right" : "Right-to-left"));

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 12, 11);
        platform_printf("4. Back to main menu");

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 10, 12);
        platform_set_color(COLOR_YELLOW);
        platform_printf("Select option: ");

        int choice = platform_get_key();
        if (choice == 27) return; 
        choice -= '0';
        switch (choice) {
            case 1:
                platform_set_cursor_position(CONSOLE_WIDTH/2 - 20, 14);
                platform_printf("Enter new word speed (0.1-1.0): ");
                float new_speed;
                scanf("%f", &new_speed);
                if (new_speed >= 0.1f && new_speed <= 1.0f) *word_speed = new_speed;
                getchar();
                break;
            case 2:
                platform_set_cursor_position(CONSOLE_WIDTH/2 - 20, 14);
                platform_printf("Enter new lives count (1-5): ");
                int new_lives;
                scanf("%d", &new_lives);
                if (new_lives >= 1 && new_lives <= 5) *lives = new_lives;
                getchar();
                break;
            case 3:
                platform_set_cursor_position(CONSOLE_WIDTH/2 - 20, 14);
                platform_printf("Enter direction mode (0: Mixed, 1: LTR, -1: RTL): ");
                int new_direction;
                scanf("%d", &new_direction);
                if (new_direction >= -1 && new_direction <= 1) *word_direction_mode = new_direction;
                getchar(); // Consume newline
                break;
            case 4:
                return;
            default:
                platform_set_cursor_position(CONSOLE_WIDTH/2 - 15, 14);
                platform_printf("Invalid option! Press any key to continue...");
                platform_get_key();
        }
    }
}

void show_statistics(Stats stats, Stats best_stats) {
    while (1) {
        platform_clear_screen();
        draw_border();

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 5, 5);
        platform_set_color(COLOR_CYAN);
        platform_printf("STATISTICS");

        
        platform_set_cursor_position(CONSOLE_WIDTH/2 - 25, 8); 
        platform_set_color(COLOR_WHITE);
        platform_printf("--- Current Game ---");

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 25, 10);
        platform_printf("Correct words: %d", stats.correct_words);

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 25, 11);
        platform_printf("Start speed: %.1f", stats.start_speed);

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 25, 12);
        platform_printf("WPM: %d", stats.wpm);

        
        platform_set_cursor_position(CONSOLE_WIDTH/2 + 5, 8); 
        platform_set_color(COLOR_YELLOW);
        platform_printf("--- Best Result ---\n");

        platform_set_cursor_position(CONSOLE_WIDTH/2 + 5, 10);
        platform_printf("Correct words: %d", best_stats.correct_words);

        platform_set_cursor_position(CONSOLE_WIDTH/2 + 5, 11);
        platform_printf("Start speed: %.1f", best_stats.start_speed);

        platform_set_cursor_position(CONSOLE_WIDTH/2 + 5, 12);
        platform_printf("WPM: %d", best_stats.wpm);

        platform_set_cursor_position(CONSOLE_WIDTH/2 - 15, 15); 
        platform_set_color(COLOR_YELLOW);
        platform_printf("Press ESC to return to main menu...");

        if (platform_get_key() == 27) break;
    }
}
