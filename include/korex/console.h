#ifndef KOREX_CONSOLE_H
#define KOREX_CONSOLE_H

#include <korex/types.h>

enum console_color {
    CONSOLE_COLOR_DEFAULT = 0x0F,
    CONSOLE_COLOR_INFO    = 0x0B,
    CONSOLE_COLOR_ERROR   = 0x0C,
};

struct console {
    const char* name;
    void (*putc)(char c, u8 color);
    void (*clear)(void);
};

void console_register(const struct console* console);
void console_putc(char c, u8 color);
void console_write(const char* str, u8 color);
void console_clear(void);

#endif
