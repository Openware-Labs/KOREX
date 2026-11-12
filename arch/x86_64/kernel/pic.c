#include <korex/types.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/setup.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define PIC_EOI        0x20
#define PIC_READ_ISR   0x0B
#define PIC_CASCADE_IRQ 2

#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10
#define ICW4_8086 0x01

void pic_init(void) {
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, IRQ_VECTOR_BASE);
    io_wait();
    outb(PIC2_DATA, IRQ_VECTOR_BASE + 8);
    io_wait();

    outb(PIC1_DATA, 1 << PIC_CASCADE_IRQ);
    io_wait();
    outb(PIC2_DATA, PIC_CASCADE_IRQ);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void pic_unmask(unsigned int irq) {
    u16 port = irq < 8 ? PIC1_DATA : PIC2_DATA;
    u8 line = (u8)(irq & 7);

    outb(port, (u8)(inb(port) & ~(1u << line)));

    if (irq >= 8) {
        outb(PIC1_DATA, (u8)(inb(PIC1_DATA) & ~(1u << PIC_CASCADE_IRQ)));
    }
}

void pic_send_eoi(unsigned int irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}

static u16 pic_read_isr(void) {
    outb(PIC1_COMMAND, PIC_READ_ISR);
    outb(PIC2_COMMAND, PIC_READ_ISR);
    return (u16)((inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND));
}

int pic_irq_is_spurious(unsigned int irq) {
    u16 isr;

    if (irq != 7 && irq != 15) {
        return 0;
    }

    isr = pic_read_isr();

    if (irq == 7) {
        return !(isr & 0x0080);
    }
    if (isr & 0x8000) {
        return 0;
    }

    outb(PIC1_COMMAND, PIC_EOI);
    return 1;
}
