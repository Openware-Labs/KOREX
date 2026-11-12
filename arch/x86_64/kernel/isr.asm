[bits 64]
[extern irq_dispatch]
[extern exception_handler]

[global isr_default]
[global isr_irq_table]

%macro SAVE_REGS 0
    push rax
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
%endmacro

%macro RESTORE_REGS 0
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax
%endmacro

%macro IRQ_STUB 1
isr_irq%1:
    SAVE_REGS
    mov edi, %1
    call irq_dispatch
    RESTORE_REGS
    iretq
%endmacro

isr_default:
    SAVE_REGS
    call exception_handler
    RESTORE_REGS
    iretq

IRQ_STUB 0
IRQ_STUB 1
IRQ_STUB 2
IRQ_STUB 3
IRQ_STUB 4
IRQ_STUB 5
IRQ_STUB 6
IRQ_STUB 7
IRQ_STUB 8
IRQ_STUB 9
IRQ_STUB 10
IRQ_STUB 11
IRQ_STUB 12
IRQ_STUB 13
IRQ_STUB 14
IRQ_STUB 15

section .rodata
align 8
isr_irq_table:
    dq isr_irq0, isr_irq1, isr_irq2, isr_irq3
    dq isr_irq4, isr_irq5, isr_irq6, isr_irq7
    dq isr_irq8, isr_irq9, isr_irq10, isr_irq11
    dq isr_irq12, isr_irq13, isr_irq14, isr_irq15
