#include <stdio.h>
#include <windows.h>

#include "ui.h"
#include "utils.h"
#include "stats.h"

#ifdef _WIN32
#include <conio.h>
#endif


void draw_border() {
    set_color(COLOR_YELLOW);
    for (int i = 0; i < CONSOLE_WIDTH; i++) {
        set_cursor_position(i, 0);
        printf("=");
        set_cursor_position(i, CONSOLE_HEIGHT - 2);
        printf("=");
    }
    for (int i = 0; i < CONSOLE_HEIGHT - 1; i++) {
        set_cursor_position(0, i);
        printf("|");
        set_cursor_position(CONSOLE_WIDTH - 1, i);
        printf("|");
    }
}

void show_menu() {
    clear_screen();
    draw_border();

    set_cursor_position(CONSOLE_WIDTH/2 - 7, 5);
    set_color(COLOR_CYAN);
    printf("TYPING TRAINER");

    set_cursor_position(CONSOLE_WIDTH/2 - 6, 8);
    set_color(COLOR_WHITE);
    printf("1. Start Game");
    set_cursor_position(CONSOLE_WIDTH/2 - 6, 9);
    printf("2. Settings");
    set_cursor_position(CONSOLE_WIDTH/2 - 6, 10);
    printf("3. Statistics");
    set_cursor_position(CONSOLE_WIDTH/2 - 6, 11);
    printf("4. Exit");

    set_cursor_position(CONSOLE_WIDTH/2 - 10, 13);
    set_color(COLOR_YELLOW);
    printf("Select menu item: ");
}

void show_settings_menu(float* word_speed, int* lives) {
    while (1) {
        clear_screen();
        draw_border();

        set_cursor_position(CONSOLE_WIDTH/2 - 4, 5);
        set_color(COLOR_CYAN);
        printf("SETTINGS");

        set_cursor_position(CONSOLE_WIDTH/2 - 12, 8);
        set_color(COLOR_WHITE);
        printf("1. Word speed (current: %.1f)", *word_speed);

        set_cursor_position(CONSOLE_WIDTH/2 - 12, 9);
        printf("2. Lives count (current: %d)", *lives);

        set_cursor_position(CONSOLE_WIDTH/2 - 12, 10);
        printf("3. Back to main menu");

        set_cursor_position(CONSOLE_WIDTH/2 - 10, 12);
        set_color(COLOR_YELLOW);
        printf("Select option: ");

        int choice = _getch() - '0';
        switch (choice) {
            case 1:
                set_cursor_position(CONSOLE_WIDTH/2 - 20, 14);
                printf("Enter new word speed (0.1-2.0): ");
                float new_speed;
                scanf("%f", &new_speed);
                if (new_speed >= 0.1f && new_speed <= 2.0f) *word_speed = new_speed;
                getchar();
                break;
            case 2:
                set_cursor_position(CONSOLE_WIDTH/2 - 20, 14);
                printf("Enter new lives count (1-5): ");
                int new_lives;
                scanf("%d", &new_lives);
                if (new_lives >= 1 && new_lives <= 5) *lives = new_lives;
                getchar();
                break;
            case 3:
                return;
            default:
                set_cursor_position(CONSOLE_WIDTH/2 - 15, 14);
                printf("Invalid option! Press any key to continue...");
                _getch();
        }
    }
}

void show_statistics(Stats stats) {
    while (1) {
        clear_screen();
        draw_border();

        set_cursor_position(CONSOLE_WIDTH/2 - 5, 5);
        set_color(COLOR_CYAN);
        printf("STATISTICS");

        set_cursor_position(CONSOLE_WIDTH/2 - 12, 8);
        set_color(COLOR_WHITE);
        printf("Total words: %d", stats.total_words);

        set_cursor_position(CONSOLE_WIDTH/2 - 12, 9);
        printf("Correct: %d", stats.correct_words);

        set_cursor_position(CONSOLE_WIDTH/2 - 12, 10);
        printf("Accuracy: %d%%", stats.total_words > 0 ? (stats.correct_words * 100) / stats.total_words : 0);

        set_cursor_position(CONSOLE_WIDTH/2 - 15, 12);
        set_color(COLOR_YELLOW);
        printf("Press ESC to return to main menu...");

        if (_getch() == 27) break;
    }
}
