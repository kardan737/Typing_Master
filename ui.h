#ifndef UI_H
#define UI_H

#include "stats.h"

void draw_border();
void show_menu();
void show_settings_menu(float* word_speed, int* lives);
void show_statistics(Stats stats);

#endif // UI_H
