#include <korex/init.h>
#include <korex/kernel.h>
#include <korex/console.h>
#include <korex/version.h>
#include <korex/vga.h>
#include <korex/timer.h>
#include <korex/keyboard.h>
#include <korex/pcspkr.h>
#include <korex/tty.h>
#include <usr/shell.h>
#include <asm/setup.h>
#include <asm/irq.h>
#include <asm/cpu.h>

static void __noreturn kernel_idle_loop(void) {
    for (;;) {
        tty_poll();

        irq_disable();
        if (tty_pending()) {
            irq_enable();
        } else {
            cpu_idle();
        }
    }
}

void start_kernel(void) {
    vga_init();
    printk_color(CONSOLE_COLOR_INFO, "%s v%s\n", korex_version.name, korex_version.release);

    setup_arch();
    timer_init();
    keyboard_init();

    tty_set_bell_handler(pcspkr_beep);
    shell_init();

    irq_enable();
    kernel_idle_loop();
}
