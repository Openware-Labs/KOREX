#ifndef KOREX_KERNEL_H
#define KOREX_KERNEL_H

#include <korex/types.h>
#include <korex/stdarg.h>

int vsnprintf(char* buf, size_t size, const char* fmt, va_list args);
int snprintf(char* buf, size_t size, const char* fmt, ...) __printf(3, 4);

int vprintk_color(u8 color, const char* fmt, va_list args);
int printk_color(u8 color, const char* fmt, ...) __printf(2, 3);
int printk(const char* fmt, ...) __printf(1, 2);

void panic(const char* fmt, ...) __noreturn __printf(1, 2);

#endif
