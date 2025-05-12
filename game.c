#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
   

   
#include "game.h"
#include "ui.h"
#include "utils.h"
#include "words.h"
#include "stats.h"

void run_game(Stats* stats, float word_speed, int lives) {
    Word active_words[MAX_WORDS] = {0};
    int score = 0;
    int word_spawn_timer = 0;
    char input[100] = {0};
    int input_pos = 0;
    int i;
    int input_line_y = CONSOLE_HEIGHT - 3;
    time_t game_start_time = time(NULL);

    clear_screen();
    draw_border();

    while (lives > 0) {
        for (i = 0; i < MAX_WORDS; i++) {
            if (active_words[i].active) {
                set_cursor_position((int)active_words[i].prev_x, active_words[i].y);
                printf(" ");
            }
        }

        for (i = 0; i < MAX_WORDS; i++) {
            if (active_words[i].active) {
                active_words[i].prev_x = active_words[i].x;
                active_words[i].x += word_speed;

                if (active_words[i].x >= CONSOLE_WIDTH - strlen(active_words[i].text) - 1) {
                // Очистить слово с экрана
                    set_cursor_position((int)active_words[i].x, active_words[i].y);
                    for (int j = 0; j < strlen(active_words[i].text); j++) {
                        printf(" ");
                    }

                    if (!active_words[i].typed) {
                        lives--;
                        play_error_sound();
                        flash_screen();
                    }

                    active_words[i] = (Word){0};
                }

            }
        }

        if (word_spawn_timer <= 0) {
            for (i = 0; i < MAX_WORDS; i++) {
                if (!active_words[i].active) {
                    strcpy(active_words[i].text, get_random_word());
                    active_words[i].x = 1;
                    active_words[i].prev_x = 1;
                    active_words[i].y = rand() % (CONSOLE_HEIGHT - 4) + 1;
                    active_words[i].active = 1;
                    active_words[i].typed = 0;
                    word_spawn_timer = 20;
                    break;
                }
            }
        }
        word_spawn_timer--;

        for (i = 0; i < MAX_WORDS; i++) {
            if (active_words[i].active) {
                set_cursor_position((int)active_words[i].x, active_words[i].y);
                set_color(active_words[i].typed ? COLOR_GREEN : COLOR_WHITE);
                printf("%s", active_words[i].text);
            }
        }

        set_cursor_position(1, input_line_y);
        set_color(COLOR_WHITE);
        printf("Input: ");
        set_color(COLOR_RED);
        printf("%s_", input);

        stats->wpm = calculate_wpm(score, game_start_time);
        stats->accuracy = stats->total_words > 0 ? (stats->correct_words * 100) / stats->total_words : 0;
        show_stats(score, lives, stats->wpm, stats->accuracy);

        if (_kbhit()) {
            char c = _getch();
            if (c == 27) break;
            else if (c == '\b') {
                if (input_pos > 0) input[--input_pos] = '\0';
            } else if (c == ' ') {
                input[input_pos] = '\0';
                int word_found = 0;
                for (i = 0; i < MAX_WORDS; i++) {
                    if (active_words[i].active && !active_words[i].typed && strcmp(input, active_words[i].text) == 0) {
                        active_words[i].typed = 1;
                        score++;
                        stats->correct_words++;
                        word_found = 1;
                        break;
                    }
                }
                if (!word_found) {
                    play_error_sound();
                    flash_screen();
                }
                stats->total_words++;
                input_pos = 0;
                memset(input, 0, sizeof(input));

                set_cursor_position(8, input_line_y);
                for (i = 0; i < CONSOLE_WIDTH - 9; i++) printf(" ");

            } else if (c >= 'a' && c <= 'z') {
                if (input_pos < sizeof(input) - 1) {
                    input[input_pos++] = c;
                    input[input_pos] = '\0';
                }
            }
        }

        Sleep(50);
    }

    clear_screen();
    set_color(COLOR_YELLOW);
    printf("Game Over!\nFinal Score: %d\nWPM: %d\nAccuracy: %d%%\n\nPress any key to continue...", score, stats->wpm, stats->accuracy);
    _getch();
}
