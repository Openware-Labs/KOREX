#ifndef KOREX_COMPILER_H
#define KOREX_COMPILER_H

#define __noreturn         __attribute__((noreturn))
#define __packed           __attribute__((packed))
#define __unused           __attribute__((unused))
#define __aligned(n)       __attribute__((aligned(n)))
#define __section(name)    __attribute__((section(name)))
#define __printf(fmt, arg) __attribute__((format(printf, fmt, arg)))

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#endif
