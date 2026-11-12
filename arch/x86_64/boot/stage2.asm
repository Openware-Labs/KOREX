[org 0x1000]
[bits 16]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    mov si, msg_loading
    call print_16

    mov ax, 0x1000
    mov es, ax
    xor bx, bx

    mov ah, 0x02
    mov al, 64
    mov ch, 0
    mov cl, 10
    mov dh, 0
    mov dl, 0x80
    int 0x13
    jc disk_error

    in al, 0x92
    or al, 2
    out 0x92, al

    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG_32:protected_mode

disk_error:
    mov si, msg_err
    call print_16
    jmp $

print_16:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp print_16
.done:
    ret

[bits 32]
protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    mov esi, 0x10000
    mov edi, 0x100000
    mov ecx, 32768 / 4
    rep movsd

    call setup_paging
    call enable_long_mode

    jmp CODE_SEG_64:long_mode_entry

setup_paging:
    mov edi, 0x7000
    mov cr3, edi
    xor eax, eax
    mov ecx, 4096
    rep stosd
    mov edi, cr3

    mov dword [edi], 0x8003
    add edi, 0x1000
    mov dword [edi], 0x9003
    add edi, 0x1000

    mov ebx, 0x00000083
    mov ecx, 512
.loop:
    mov dword [edi], ebx
    add ebx, 0x200000
    add edi, 8
    loop .loop
    ret

enable_long_mode:
    mov eax, cr4
    or eax, 1 << 5
    or eax, 1 << 9
    or eax, 1 << 10
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax
    ret

[bits 64]
long_mode_entry:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rdi, 0xB8000
    mov word [rdi], 0x0A58

    mov rsp, 0x90000
    and rsp, -16
    push 0
    mov rax, 0x100000
    jmp rax

msg_loading db "Loading kernel from disk...", 13, 10, 0
msg_err     db "Disk read error!", 13, 10, 0

gdt_start:
    dq 0x0
gdt_code_32:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt_code_64:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 10101111b
    db 0x0
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG_32 equ gdt_code_32 - gdt_start
CODE_SEG_64 equ gdt_code_64 - gdt_start
DATA_SEG equ gdt_data - gdt_start
