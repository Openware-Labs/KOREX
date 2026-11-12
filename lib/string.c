#include <korex/string.h>

void* memcpy(void* dest, const void* src, size_t n) {
    u8* d = dest;
    const u8* s = src;

    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void* memmove(void* dest, const void* src, size_t n) {
    u8* d = dest;
    const u8* s = src;

    if (d < s) {
        while (n--) {
            *d++ = *s++;
        }
    } else if (d > s) {
        d += n;
        s += n;
        while (n--) {
            *--d = *--s;
        }
    }
    return dest;
}

void* memset(void* s, int c, size_t n) {
    u8* p = s;

    while (n--) {
        *p++ = (u8)c;
    }
    return s;
}

int memcmp(const void* a, const void* b, size_t n) {
    const u8* x = a;
    const u8* y = b;

    while (n--) {
        if (*x != *y) {
            return *x - *y;
        }
        x++;
        y++;
    }
    return 0;
}

size_t strlen(const char* s) {
    const char* p = s;

    while (*p) {
        p++;
    }
    return (size_t)(p - s);
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const u8*)s1 - *(const u8*)s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) {
        return 0;
    }
    return *(const u8*)s1 - *(const u8*)s2;
}
