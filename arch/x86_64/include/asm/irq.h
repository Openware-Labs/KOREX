#ifndef ASM_IRQ_H
#define ASM_IRQ_H

#include <korex/types.h>

#define NR_IRQS         16
#define IRQ_VECTOR_BASE 0x20

#define IRQ_TIMER    0
#define IRQ_KEYBOARD 1

typedef void (*irq_handler_t)(void);

int irq_register(unsigned int irq, irq_handler_t handler);
void irq_dispatch(unsigned int irq);

void pic_unmask(unsigned int irq);
void pic_send_eoi(unsigned int irq);
int pic_irq_is_spurious(unsigned int irq);

static inline void irq_enable(void) {
    __asm__ volatile ("sti" : : : "memory");
}

static inline void irq_disable(void) {
    __asm__ volatile ("cli" : : : "memory");
}

#endif
