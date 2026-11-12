#include <korex/pcspkr.h>
#include <korex/pit.h>
#include <korex/timer.h>
#include <asm/io.h>

#define PCSPKR_CONTROL_PORT   0x61
#define PCSPKR_ENABLE_BITS    0x03
#define PCSPKR_BEEP_FREQUENCY 1000
#define PCSPKR_BEEP_MS        100

void pcspkr_stop(void) {
    outb(PCSPKR_CONTROL_PORT, (u8)(inb(PCSPKR_CONTROL_PORT) & ~PCSPKR_ENABLE_BITS));
}

void pcspkr_play(u32 frequency) {
    u32 divisor;

    if (frequency == 0) {
        pcspkr_stop();
        return;
    }

    divisor = PIT_BASE_FREQUENCY / frequency;

    outb(PIT_COMMAND_PORT, PIT_CMD_CHANNEL2_SQUARE_WAVE);
    outb(PIT_CHANNEL2_PORT, (u8)(divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (u8)((divisor >> 8) & 0xFF));

    outb(PCSPKR_CONTROL_PORT, (u8)(inb(PCSPKR_CONTROL_PORT) | PCSPKR_ENABLE_BITS));
}

void pcspkr_beep(void) {
    pcspkr_play(PCSPKR_BEEP_FREQUENCY);
    msleep(PCSPKR_BEEP_MS);
    pcspkr_stop();
}
