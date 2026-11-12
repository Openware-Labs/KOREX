#include <korex/kernel.h>
#include <korex/console.h>

#define PRINTK_BUFFER_SIZE 256

int vprintk_color(u8 color, const char* fmt, va_list args) {
    char buffer[PRINTK_BUFFER_SIZE];
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);

    console_write(buffer, color);
    return len;
}

int printk_color(u8 color, const char* fmt, ...) {
    va_list args;
    int len;

    va_start(args, fmt);
    len = vprintk_color(color, fmt, args);
    va_end(args);
    return len;
}

int printk(const char* fmt, ...) {
    va_list args;
    int len;

    va_start(args, fmt);
    len = vprintk_color(CONSOLE_COLOR_DEFAULT, fmt, args);
    va_end(args);
    return len;
}
