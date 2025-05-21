#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
//#include <ncurses.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
   
   
#include "game.h"
#include "ui.h"
#include "constants.h"
#include "words.h"
#include "stats.h"
#include "platform.h"


void get_random_word_or_pair(char* buffer, size_t buffer_size, int* word_count) {
    int choice = rand() % 5; 

    if (choice == 4 && adjective_count > 0 && noun_count > 0) {
        const char* adj = loaded_adjectives[rand() % adjective_count];
        const char* noun = loaded_words[rand() % noun_count];
        snprintf(buffer, buffer_size, "%s %s", adj, noun);
        *word_count = 2;
    } else {
        const char* word = loaded_words[rand() % noun_count];
        strncpy(buffer, word, buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
        *word_count = 1;
    }
}

void run_game(Stats* stats, float word_speed, int lives, int word_direction_mode) {
    Word active_words[MAX_WORDS] = {0};
    int score = 0;
    int word_spawn_timer = 0;
    char input[100] = {0};
    int input_pos = 0;
    int i;
    int input_line_y = CONSOLE_HEIGHT - 3;
    time_t game_start_time = time(NULL);
    int words_since_speedup = 0;
    int max_active_words = (int)(2 + word_speed * 4);
    if (max_active_words > MAX_WORDS) max_active_words = MAX_WORDS;
    int active_count = 0;
    int words_generated_since_pair = 0; 
    for (i = 0; i < MAX_WORDS; i++) {
        if (active_words[i].active) active_count++;
    }

    platform_clear_screen();
    draw_border();

    while (lives > 0) {
        for (i = 0; i < MAX_WORDS; i++) {
            if (active_words[i].active) {
                int clear_start_x = (int)active_words[i].prev_x;
                int clear_end_x = clear_start_x + strlen(active_words[i].text);
                int clear_y = active_words[i].y;

                if (clear_start_x < 1) clear_start_x = 1;
                if (clear_end_x > CONSOLE_WIDTH - 1) clear_end_x = CONSOLE_WIDTH - 1; 
                
                if (clear_start_x < clear_end_x) {
                    platform_set_cursor_position(clear_start_x, clear_y);
                    for (int j = 0; j < clear_end_x - clear_start_x; j++) {
                        platform_printf(" ");
                    }
                }
            }
        }

        for (i = 0; i < MAX_WORDS; i++) {
            if (active_words[i].active) {
                active_words[i].prev_x = active_words[i].x;
                active_words[i].x += word_speed * active_words[i].direction;

                if ((active_words[i].direction == 1 && active_words[i].x >= CONSOLE_WIDTH - strlen(active_words[i].text) - 1) ||
                    (active_words[i].direction == -1 && active_words[i].x < 1)) {
                    platform_set_cursor_position((int)active_words[i].prev_x, active_words[i].y);
                    for (int j = 0; j < strlen(active_words[i].text); j++) {
                        platform_printf(" ");
                    }

                    if (!active_words[i].typed) {
                        lives--;
                        platform_beep();
                        platform_flash_screen();
                    }

                    active_words[i] = (Word){0};
                }

            }
        }

        if (word_spawn_timer <= 0) {
            if (active_count < max_active_words) {
                for (i = 0; i < MAX_WORDS; i++) {
                    if (!active_words[i].active) {
                        int y, conflict, attempt = 0;
                        do {
                            y = rand() % (CONSOLE_HEIGHT - 4) + 1;
                            conflict = 0;
                            for (int k = 0; k < MAX_WORDS; k++) {
                                if (active_words[k].active && active_words[k].y == y &&
                                    active_words[k].x < CONSOLE_WIDTH - 2) {
                                    conflict = 1;
                                    break;
                                }
                            }
                            attempt++;
                            if (attempt > 20) break;
                        } while (conflict);
                        get_random_word_or_pair(active_words[i].text, sizeof(active_words[i].text), &active_words[i].word_count);
                        if (word_direction_mode == 0) {
                            active_words[i].direction = (rand() % 2 == 0) ? 1 : -1;
                        } else {
                            active_words[i].direction = word_direction_mode;
                        }
                        if (active_words[i].direction == 1) {
                            active_words[i].x = 1;
                        } else {
                            active_words[i].x = CONSOLE_WIDTH - strlen(active_words[i].text) - 1;
                        }
                        active_words[i].prev_x = active_words[i].x;
                        active_words[i].y = y;
                        active_words[i].active = 1;
                        active_words[i].typed = 0;
                        word_spawn_timer = (int)(BASE_SPAWN_INTERVAL / word_speed);
                        if (word_spawn_timer < 3) word_spawn_timer = 3;
                        break;
                    }
                }
            }
        }
        word_spawn_timer--;

        for (i = 0; i < MAX_WORDS; i++) {
            if (active_words[i].active) {
                platform_set_cursor_position((int)active_words[i].x, active_words[i].y);
                platform_set_color(active_words[i].typed ? COLOR_GREEN : COLOR_WHITE);
                platform_printf("%s", active_words[i].text);
            }
        }

        platform_set_cursor_position(1, input_line_y);
        platform_set_color(COLOR_WHITE);
        platform_printf("Input: ");
        platform_set_color(COLOR_RED);
        platform_printf("%s_", input);

        stats->wpm = calculate_wpm(score, game_start_time);
        stats->accuracy = stats->total_words > 0 ? (stats->correct_words * 100) / stats->total_words : 0;
        show_stats(score, lives, stats->wpm, stats->accuracy);

        if (platform_key_pressed()) {
            char c = platform_get_key();
            if (c == 27) { 
                break; 
            } else if (c == '\b') { 
                if (input_pos > 0) {
                    input[--input_pos] = '\0';
                }
            } else if (c == '\r' || c == '\n') { 
                input[input_pos] = '\0';
                int word_found = 0;
                for (i = 0; i < MAX_WORDS; i++) {
                    if (active_words[i].active && !active_words[i].typed && strcmp(input, active_words[i].text) == 0) {
                        active_words[i].typed = 1;
                        bool is_word_pair = (strchr(active_words[i].text, ' ') != NULL);
                        if (is_word_pair) {
                            score += 2;
                            stats->correct_words += 2;
                        } else {
                            score++;
                            stats->correct_words++;
                        }

                        word_found = 1;
                        words_since_speedup += (is_word_pair ? 2 : 1);
                        if (words_since_speedup >= 5) {
                            platform_set_cursor_position(CONSOLE_WIDTH/2 - 12, CONSOLE_HEIGHT/2);
                            platform_set_color(COLOR_YELLOW);
                            platform_printf("Скорость увеличится!");
                            #ifdef _WIN32
                            Sleep(1000);
                            #else
                            usleep(1000000); 
                            #endif
                            
                            platform_set_cursor_position(CONSOLE_WIDTH/2 - 12, CONSOLE_HEIGHT/2);
                            for (int k = 0; k < 25; k++) platform_printf(" "); 
                            platform_set_color(COLOR_WHITE);
                            word_speed += 0.1f;
                            if (word_speed > 2.0f) word_speed = 2.0f;
                            words_since_speedup = 0;
                        }
                        break;
                    }
                }

                if (!word_found) {
                    platform_beep();
                    platform_flash_screen();
                }

                stats->total_words += (word_found) ? (strchr(input, ' ') != NULL ? 2 : 1) : 1; 

                input_pos = 0;
                memset(input, 0, sizeof(input));

                platform_set_cursor_position(8, input_line_y);
                for (i = 0; i < CONSOLE_WIDTH - 9; i++) platform_printf(" ");

            } else if ((c >= 'a' && c <= 'z') || c == ' ') { 
                if (input_pos < sizeof(input) - 1) {
                    input[input_pos++] = c;
                    input[input_pos] = '\0';
                }
            }
        }

        #ifdef _WIN32
        Sleep(50);
        #else
        usleep(50000); 
        #endif
    }

    platform_clear_screen();
    platform_set_color(COLOR_YELLOW);
    platform_printf("Game Over!\nFinal Score: %d\nWPM: %d\nAccuracy: %d%%\n\nPress any key to continue...", score, stats->wpm, stats->accuracy);
    platform_get_key();
}
