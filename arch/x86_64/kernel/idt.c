#include <korex/types.h>
#include <asm/irq.h>
#include <asm/setup.h>

#define IDT_ENTRIES        256
#define IDT_GATE_INTERRUPT 0x8E

struct idt_entry {
    u16 base_low;
    u16 selector;
    u8  ist;
    u8  flags;
    u16 base_mid;
    u32 base_high;
    u32 reserved;
} __packed;

struct idt_ptr {
    u16 limit;
    u64 base;
} __packed;

extern void isr_default(void);
extern const u64 isr_irq_table[NR_IRQS];

static struct idt_entry idt[IDT_ENTRIES] __aligned(16);
static struct idt_ptr idtp;

static void idt_set_gate(unsigned int vector, u64 handler) {
    idt[vector].base_low  = (u16)(handler & 0xFFFF);
    idt[vector].selector  = KERNEL_CODE_SELECTOR;
    idt[vector].ist       = 0;
    idt[vector].flags     = IDT_GATE_INTERRUPT;
    idt[vector].base_mid  = (u16)((handler >> 16) & 0xFFFF);
    idt[vector].base_high = (u32)((handler >> 32) & 0xFFFFFFFF);
    idt[vector].reserved  = 0;
}

void idt_init(void) {
    idtp.limit = (u16)(sizeof(idt) - 1);
    idtp.base  = (u64)(uintptr_t)idt;

    for (unsigned int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, (u64)(uintptr_t)isr_default);
    }
    for (unsigned int irq = 0; irq < NR_IRQS; irq++) {
        idt_set_gate(IRQ_VECTOR_BASE + irq, isr_irq_table[irq]);
    }

    __asm__ volatile ("lidt %0" : : "m"(idtp));
}
