#include <korex/init.h>
#include <korex/string.h>

extern char __bss_start[];
extern char __bss_end[];

__section(".text.start") __noreturn void _start(void) {
    memset(__bss_start, 0, (size_t)(__bss_end - __bss_start));
    start_kernel();
}
