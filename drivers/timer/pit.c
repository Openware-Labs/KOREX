#include <korex/timer.h>
#include <korex/pit.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/cpu.h>

static volatile u64 ticks;

static void pit_irq(void) {
    ticks++;
}

void timer_init(void) {
    u32 divisor = PIT_BASE_FREQUENCY / HZ;

    outb(PIT_COMMAND_PORT, PIT_CMD_CHANNEL0_RATE_GENERATOR);
    outb(PIT_CHANNEL0_PORT, (u8)(divisor & 0xFF));
    outb(PIT_CHANNEL0_PORT, (u8)((divisor >> 8) & 0xFF));

    irq_register(IRQ_TIMER, pit_irq);
}

u64 timer_ticks(void) {
    return ticks;
}

void msleep(unsigned int ms) {
    u64 wait = ((u64)ms * HZ + 999) / 1000 + 1;
    u64 start = ticks;

    while (ticks - start < wait) {
        cpu_idle();
    }
}
