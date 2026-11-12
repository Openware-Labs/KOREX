#include <korex/types.h>
#include <korex/string.h>
#include <asm/cpu.h>

#define CPUID_VENDOR       0x00000000
#define CPUID_EXT_MAX      0x80000000
#define CPUID_BRAND_FIRST  0x80000002
#define CPUID_BRAND_LEAVES 3

static inline void cpuid(u32 leaf, u32* eax, u32* ebx, u32* ecx, u32* edx) {
    __asm__ volatile ("cpuid"
    : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
    : "a"(leaf), "c"(0));
}

void cpu_get_vendor(char* buffer) {
    u32 eax, ebx, ecx, edx;

    cpuid(CPUID_VENDOR, &eax, &ebx, &ecx, &edx);

    memcpy(buffer + 0, &ebx, 4);
    memcpy(buffer + 4, &edx, 4);
    memcpy(buffer + 8, &ecx, 4);
    buffer[12] = '\0';
}

void cpu_get_brand(char* buffer) {
    u32 regs[4];
    u32 start = 0;

    cpuid(CPUID_EXT_MAX, &regs[0], &regs[1], &regs[2], &regs[3]);

    if (regs[0] < CPUID_BRAND_FIRST + CPUID_BRAND_LEAVES - 1) {
        memcpy(buffer, "Unknown CPU", sizeof("Unknown CPU"));
        return;
    }

    for (u32 i = 0; i < CPUID_BRAND_LEAVES; i++) {
        cpuid(CPUID_BRAND_FIRST + i, &regs[0], &regs[1], &regs[2], &regs[3]);
        memcpy(buffer + i * sizeof(regs), regs, sizeof(regs));
    }
    buffer[CPU_BRAND_LEN - 1] = '\0';

    while (buffer[start] == ' ') {
        start++;
    }
    if (start > 0) {
        memmove(buffer, buffer + start, CPU_BRAND_LEN - start);
    }
}
