#ifndef ASM_CPU_H
#define ASM_CPU_H

#define CPU_VENDOR_LEN 13
#define CPU_BRAND_LEN  49

void cpu_get_vendor(char* buffer);
void cpu_get_brand(char* buffer);

static inline void cpu_idle(void) {
    __asm__ volatile ("sti; hlt" : : : "memory");
}

static inline void cpu_halt(void) {
    __asm__ volatile ("hlt");
}

#endif
