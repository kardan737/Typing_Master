#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdarg.h>

void platform_init();
void platform_cleanup();
void platform_set_color(int color);
void platform_set_cursor_position(int x, int y);
void platform_clear_screen();
int  platform_get_key(); 
int  platform_key_pressed();
void platform_beep();
void platform_flash_screen();
void platform_printf(const char* fmt, ...);

#endif // PLATFORM_H 