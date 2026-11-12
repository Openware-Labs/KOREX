#include <asm/irq.h>

static irq_handler_t irq_handlers[NR_IRQS];

int irq_register(unsigned int irq, irq_handler_t handler) {
    if (irq >= NR_IRQS || !handler) {
        return -1;
    }

    irq_handlers[irq] = handler;
    pic_unmask(irq);
    return 0;
}

void irq_dispatch(unsigned int irq) {
    if (irq >= NR_IRQS) {
        return;
    }
    if (pic_irq_is_spurious(irq)) {
        return;
    }

    if (irq_handlers[irq]) {
        irq_handlers[irq]();
    }
    pic_send_eoi(irq);
}
