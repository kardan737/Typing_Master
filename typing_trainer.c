#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25
#define MAX_WORDS 10
#define WORD_SPEED 0.5
#define BEEP_FREQUENCY 750
#define BEEP_DURATION 200

// Цвета для консоли
#define COLOR_BLACK 0
#define COLOR_WHITE 15
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_BLUE 9
#define COLOR_CYAN 11
#define COLOR_INVERTED 112  // Белый текст на черном фоне
#define BACKGROUND_RED 64   // Красный фон

//Хранение слова
typedef struct {
    char text[50];
    float x;
    int y;
    int active;
    int typed;
    float prev_x;  //предыдущая позиция
} Word;

// Структура статистики
typedef struct {
    int total_words;
    int correct_words;
    int wpm;
    int accuracy;
} Stats;

// Установка цвета текста
void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Установка позиции курсора
void set_cursor_position(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Очистка экрана
void clear_screen() {
    system("cls");
}

// Отображение рамки
void draw_border() {
    int i;
    set_color(COLOR_YELLOW);
    for (i = 0; i < CONSOLE_WIDTH; i++) {
        set_cursor_position(i, 0);
        printf("=");
        set_cursor_position(i, CONSOLE_HEIGHT - 2);
        printf("=");
    }
    for (i = 0; i < CONSOLE_HEIGHT - 1; i++) {
        set_cursor_position(0, i);
        printf("|");
        set_cursor_position(CONSOLE_WIDTH - 1, i);
        printf("|");
    }
}

// Воспроизведение звука ошибки
void play_error_sound() {
    Beep(BEEP_FREQUENCY, BEEP_DURATION);
}

// Получение случайного слова
const char* get_random_word() {
    static const char* words[] = {
        "computer", "program", "keyboard", "monitor", "mouse",
        "screen", "memory", "processor", "network", "system",
        "software", "hardware", "internet", "browser", "server",
        "database", "algorithm", "function", "variable", "string"
    };
    return words[rand() % 20];
}

// Отображение статистики
void show_stats(int score, int lives, int wpm, int accuracy) {
    set_cursor_position(0, CONSOLE_HEIGHT - 1);
    set_color(COLOR_YELLOW);
    printf("Score: %d | Lives: %d | WPM: %d | Accuracy: %d%%", score, lives, wpm, accuracy);
}

// Отображение меню
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

// Отображение настроек
void show_settings(float word_speed, int lives) {
    clear_screen();
    draw_border();
    
    set_cursor_position(CONSOLE_WIDTH/2 - 4, 5);
    set_color(COLOR_CYAN);
    printf("SETTINGS");

    set_cursor_position(CONSOLE_WIDTH/2 - 12, 8);
    set_color(COLOR_WHITE);
    printf("1. Word speed (current: %.1f)", word_speed);
    
    set_cursor_position(CONSOLE_WIDTH/2 - 12, 9);
    printf("2. Lives count (current: %d)", lives);
    
    set_cursor_position(CONSOLE_WIDTH/2 - 12, 10);
    printf("3. Back to main menu");

    set_cursor_position(CONSOLE_WIDTH/2 - 10, 12);
    set_color(COLOR_YELLOW);
    printf("Select option: ");
}

// Расчёт WPM
int calculate_wpm(int score, time_t start_time) {
    time_t current_time = time(NULL);
    double minutes = difftime(current_time, start_time) / 60.0;
    return minutes > 0 ? (int)(score / minutes) : 0;
}

// Мигание игрового поля красным, если ошибка ввода
void flash_screen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    int x, y;

    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | COLOR_WHITE);
    for (y = 1; y < CONSOLE_HEIGHT - 2; y++) { // Пропускаем верхнюю и нижнюю границы
        for (x = 1; x < CONSOLE_WIDTH - 1; x++) { // Пропускаем левую и правую границы
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

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    char mode_command[50]; // Размер консоли
    sprintf(mode_command, "mode con: cols=%d lines=%d", CONSOLE_WIDTH, CONSOLE_HEIGHT);
    system(mode_command);
    
    srand(time(NULL));
    
    Word active_words[MAX_WORDS] = {0};
    Stats stats = {0};
    int score = 0;
    int lives = 3;
    int word_spawn_timer = 0;
    char input[100] = {0};
    int input_pos = 0;
    int i;
    int input_line_y = CONSOLE_HEIGHT - 3;
    int menu_choice = 0;
    
    while (1) {
        show_menu();
        menu_choice = _getch() - '0';
        
        switch (menu_choice) {
            case 1: // Начать игру
                memset(active_words, 0, sizeof(active_words));
                stats = (Stats){0};
                score = 0;
                lives = 3;
                word_spawn_timer = 0;
                memset(input, 0, sizeof(input));
                input_pos = 0;
                
                clear_screen();
                draw_border();
                
                while (lives > 0) {
                    for (i = 0; i < MAX_WORDS; i++) {
                        if (active_words[i].active) {
                            set_cursor_position((int)active_words[i].prev_x, active_words[i].y);
                            printf(" ");
                        }
                    }
                    
                    static time_t game_start_time = 0;
                    if (game_start_time == 0) {
                        game_start_time = time(NULL);
                    }
                    
                    // Обновление позиций слов
                    for (i = 0; i < MAX_WORDS; i++) {
                        if (active_words[i].active) {
                            active_words[i].prev_x = active_words[i].x;
                            active_words[i].x += WORD_SPEED;
                            
                            if (active_words[i].x >= CONSOLE_WIDTH - strlen(active_words[i].text) - 1) {
                                if (!active_words[i].typed) {
                                    lives--;
                                    play_error_sound();
                                    flash_screen();
                                }
                                // Очищаем слово с экрана
                                set_cursor_position((int)active_words[i].x, active_words[i].y);
                                for (int j = 0; j < strlen(active_words[i].text); j++) {
                                    printf(" ");
                                }
                                active_words[i].active = 0;
                                active_words[i].x = 0;
                                active_words[i].y = 0;
                                memset(active_words[i].text, 0, sizeof(active_words[i].text));
                            }
                        }
                    }
                    
                    // Спавн новых слов
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
                    
                    // Отрисовка слов на новых позициях
                    for (i = 0; i < MAX_WORDS; i++) {
                        if (active_words[i].active) {
                            set_cursor_position((int)active_words[i].x, active_words[i].y);
                            if (active_words[i].typed) {
                                set_color(COLOR_GREEN);
                            } else {
                                set_color(COLOR_WHITE);
                            }
                            printf("%s", active_words[i].text);
                        }
                    }
                    
                    // Отрисовка строки ввода
                    set_cursor_position(1, input_line_y);
                    set_color(COLOR_WHITE);
                    printf("Input: ");
                    set_color(COLOR_RED);
                    printf("%s", input);
                    printf("_");
                    
                    // Обновление статистики
                    stats.wpm = calculate_wpm(score, game_start_time);
                    stats.accuracy = stats.total_words > 0 ? (stats.correct_words * 100) / stats.total_words : 0;
                    show_stats(score, lives, stats.wpm, stats.accuracy);
                    
                    if (_kbhit()) {
                        char c = _getch();
                        if (c == 27) {
                            break;
                        } else if (c == '\b') {
                            if (input_pos > 0) {
                                input[--input_pos] = '\0';
                            }
                        } else if (c == ' ') {
                            if (input_pos > 0) {
                                input[input_pos] = '\0';
                                int word_found = 0;
                                for (i = 0; i < MAX_WORDS; i++) {
                                    if (active_words[i].active && !active_words[i].typed &&
                                        strcmp(input, active_words[i].text) == 0) {
                                        active_words[i].typed = 1;
                                        score++;
                                        stats.correct_words++;
                                        word_found = 1;
                                        break;
                                    }
                                }
                                
                                // Если слово не найдено, считаем его ошибкой
                                if (!word_found) {
                                    play_error_sound();
                                    flash_screen();
                                }
                                
                                stats.total_words++;
                                input_pos = 0;
                                memset(input, 0, sizeof(input));
                                
                                // Очищаем строку ввода на экране
                                set_cursor_position(8, input_line_y);
                                for (i = 0; i < CONSOLE_WIDTH - 9; i++) {
                                    printf(" ");
                                }
                            }
                        } else if (c >= 'a' && c <= 'z') { // Буквы
                            if (input_pos < sizeof(input) - 1) {
                                input[input_pos++] = c;
                                input[input_pos] = '\0';
                            }
                        }
                    }
                    
                    Sleep(50);
                }
                
                // Игра окончена
                clear_screen();
                set_color(COLOR_YELLOW);
                printf("Game Over!\n");
                printf("Final Score: %d\n", score);
                printf("WPM: %d\n", stats.wpm);
                printf("Accuracy: %d%%\n", stats.accuracy);
                printf("\nPress any key to continue...");
                _getch();
                break;
                
            case 2: // Настройки
                while (1) {
                    show_settings(WORD_SPEED, lives);
                    int settings_choice = _getch() - '0';
                    
                    switch (settings_choice) {
                        case 1:
                            set_cursor_position(CONSOLE_WIDTH/2 - 20, 14);
                            printf("Enter new word speed (0.1-2.0): ");
                            float new_speed;
                            scanf("%f", &new_speed);
                            if (new_speed >= 0.1 && new_speed <= 2.0) {
                                // WORD_SPEED = new_speed;
                            }
                            getchar(); 
                            break;
                            
                        case 2:
                            set_cursor_position(CONSOLE_WIDTH/2 - 20, 14);
                            printf("Enter new lives count (1-5): ");
                            int new_lives;
                            scanf("%d", &new_lives);
                            if (new_lives >= 1 && new_lives <= 5) {
                                lives = new_lives;
                            }
                            getchar(); 
                            break;
                            
                        case 3:
                            goto settings_exit; 
                            
                        default:
                            set_cursor_position(CONSOLE_WIDTH/2 - 15, 14);
                            printf("Invalid option! Press any key to continue...");
                            _getch();
                    }
                }
                settings_exit:
                break;
                
            case 3: // Статистика
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
                    
                    if (_getch() == 27) { // ESC
                        break;
                    }
                }
                break;
                
            case 4: // Выход
                return 0;
                
            default:
                set_cursor_position(CONSOLE_WIDTH/2 - 15, 14);
                printf("Invalid option! Press any key to continue...");
                _getch();
        }
    }
    
    return 0;
}