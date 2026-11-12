#include <asm/setup.h>

void setup_arch(void) {
    idt_init();
    pic_init();
}
