[org 0x7c00]
[bits 16]

STAGE2_OFFSET  equ 0x1000
STAGE2_SECTORS equ 4

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    mov [boot_drive], dl

    mov si, msg_loading
    call print

    xor ah, ah
    mov dl, [boot_drive]
    int 0x13
    jc disk_error

    mov ah, 0x02
    mov al, STAGE2_SECTORS
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]
    mov bx, STAGE2_OFFSET
    int 0x13
    jc disk_error

    jmp STAGE2_OFFSET

disk_error:
    mov si, msg_disk_err
    call print
    jmp $

print:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp print
.done:
    ret

msg_loading  db "Loading stage 2...", 13, 10, 0
msg_disk_err db "Disk error!", 13, 10, 0
boot_drive   db 0

times 510-($-$$) db 0
dw 0xaa55
