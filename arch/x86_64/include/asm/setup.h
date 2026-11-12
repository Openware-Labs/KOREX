#ifndef ASM_SETUP_H
#define ASM_SETUP_H

#define ARCH_NAME            "x86_64 (Long Mode)"
#define KERNEL_CODE_SELECTOR 0x10

void setup_arch(void);
void idt_init(void);
void pic_init(void);

#endif
