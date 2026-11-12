#include <korex/console.h>

static const struct console* active_console;

void console_register(const struct console* console) {
    active_console = console;
}

void console_putc(char c, u8 color) {
    if (active_console && active_console->putc) {
        active_console->putc(c, color);
    }
}

void console_write(const char* str, u8 color) {
    while (*str) {
        console_putc(*str++, color);
    }
}

void console_clear(void) {
    if (active_console && active_console->clear) {
        active_console->clear();
    }
}
