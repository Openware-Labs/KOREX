#include <korex/kernel.h>

struct sink {
    char* buf;
    size_t cap;
    size_t len;
};

static void sink_put(struct sink* s, char c) {
    if (s->len + 1 < s->cap) {
        s->buf[s->len] = c;
    }
    s->len++;
}

static void sink_puts(struct sink* s, const char* str) {
    while (*str) {
        sink_put(s, *str++);
    }
}

static void sink_number(struct sink* s, u64 value, unsigned int base, int upper, int negative) {
    const char* alphabet = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    char digits[24];
    unsigned int count = 0;

    if (negative) {
        sink_put(s, '-');
    }
    do {
        digits[count++] = alphabet[value % base];
        value /= base;
    } while (value);

    while (count) {
        sink_put(s, digits[--count]);
    }
}

int vsnprintf(char* buf, size_t size, const char* fmt, va_list args) {
    struct sink out = { buf, size, 0 };

    for (; *fmt; fmt++) {
        int is_long = 0;

        if (*fmt != '%') {
            sink_put(&out, *fmt);
            continue;
        }

        fmt++;
        while (*fmt == 'l' || *fmt == 'z') {
            is_long = 1;
            fmt++;
        }

        switch (*fmt) {
            case 'c':
                sink_put(&out, (char)va_arg(args, int));
                break;
            case 's': {
                const char* str = va_arg(args, const char*);
                sink_puts(&out, str ? str : "(null)");
                break;
            }
            case 'd':
            case 'i': {
                s64 v = is_long ? va_arg(args, long) : va_arg(args, int);
                sink_number(&out, v < 0 ? -(u64)v : (u64)v, 10, 0, v < 0);
                break;
            }
            case 'u': {
                u64 v = is_long ? va_arg(args, unsigned long) : va_arg(args, unsigned int);
                sink_number(&out, v, 10, 0, 0);
                break;
            }
            case 'x':
            case 'X': {
                u64 v = is_long ? va_arg(args, unsigned long) : va_arg(args, unsigned int);
                sink_number(&out, v, 16, *fmt == 'X', 0);
                break;
            }
            case 'p':
                sink_puts(&out, "0x");
                sink_number(&out, (u64)(uintptr_t)va_arg(args, void*), 16, 0, 0);
                break;
            case '%':
                sink_put(&out, '%');
                break;
            case '\0':
                fmt--;
                break;
            default:
                sink_put(&out, '%');
                sink_put(&out, *fmt);
                break;
        }
    }

    if (size) {
        buf[out.len < size ? out.len : size - 1] = '\0';
    }
    return (int)out.len;
}

int snprintf(char* buf, size_t size, const char* fmt, ...) {
    va_list args;
    int len;

    va_start(args, fmt);
    len = vsnprintf(buf, size, fmt, args);
    va_end(args);
    return len;
}
