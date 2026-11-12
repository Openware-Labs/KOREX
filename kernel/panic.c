#include <korex/kernel.h>
#include <korex/console.h>
#include <asm/irq.h>
#include <asm/cpu.h>

void panic(const char* fmt, ...) {
    va_list args;

    irq_disable();
    console_clear();

    printk_color(CONSOLE_COLOR_ERROR, "\nKERNEL PANIC: ");
    va_start(args, fmt);
    vprintk_color(CONSOLE_COLOR_ERROR, fmt, args);
    va_end(args);
    printk("\n\nSystem halted.");

    for (;;) {
        cpu_halt();
    }
}
