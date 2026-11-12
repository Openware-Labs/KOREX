#include <korex/keyboard.h>
#include <korex/tty.h>
#include <asm/io.h>
#include <asm/irq.h>

#define KBD_DATA_PORT    0x60
#define KBD_RELEASE_MASK 0x80

static const char scancode_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

static void keyboard_irq(void) {
    u8 scancode = inb(KBD_DATA_PORT);
    char c;

    if (scancode & KBD_RELEASE_MASK) {
        return;
    }

    c = scancode_map[scancode];
    if (c) {
        tty_push(c);
    }
}

void keyboard_init(void) {
    irq_register(IRQ_KEYBOARD, keyboard_irq);
}
