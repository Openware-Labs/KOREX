#include <korex/vga.h>
#include <korex/console.h>
#include <asm/io.h>

#define VGA_MEMORY           0xB8000
#define VGA_COLUMNS          80
#define VGA_ROWS             25
#define VGA_BLANK_ATTR       0x0F

#define VGA_CRTC_INDEX       0x3D4
#define VGA_CRTC_DATA        0x3D5
#define VGA_CRTC_CURSOR_HIGH 0x0E
#define VGA_CRTC_CURSOR_LOW  0x0F

static volatile u16* const vga_buffer = (volatile u16*)VGA_MEMORY;
static unsigned int cursor_row;
static unsigned int cursor_col;

static inline u16 vga_cell(char c, u8 attr) {
    return (u16)(((u16)attr << 8) | (u8)c);
}

static void vga_update_cursor(void) {
    u16 pos = (u16)(cursor_row * VGA_COLUMNS + cursor_col);

    outb(VGA_CRTC_INDEX, VGA_CRTC_CURSOR_LOW);
    outb(VGA_CRTC_DATA, (u8)(pos & 0xFF));
    outb(VGA_CRTC_INDEX, VGA_CRTC_CURSOR_HIGH);
    outb(VGA_CRTC_DATA, (u8)((pos >> 8) & 0xFF));
}

static void vga_scroll(void) {
    for (unsigned int i = 0; i < (VGA_ROWS - 1) * VGA_COLUMNS; i++) {
        vga_buffer[i] = vga_buffer[i + VGA_COLUMNS];
    }
    for (unsigned int i = 0; i < VGA_COLUMNS; i++) {
        vga_buffer[(VGA_ROWS - 1) * VGA_COLUMNS + i] = vga_cell(' ', VGA_BLANK_ATTR);
    }
    cursor_row = VGA_ROWS - 1;
    cursor_col = 0;
}

static void vga_clear(void) {
    for (unsigned int i = 0; i < VGA_ROWS * VGA_COLUMNS; i++) {
        vga_buffer[i] = vga_cell(' ', VGA_BLANK_ATTR);
    }
    cursor_row = 0;
    cursor_col = 0;
    vga_update_cursor();
}

static void vga_putc(char c, u8 color) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            vga_buffer[cursor_row * VGA_COLUMNS + cursor_col] = vga_cell(' ', VGA_BLANK_ATTR);
        }
    } else {
        vga_buffer[cursor_row * VGA_COLUMNS + cursor_col] = vga_cell(c, color);
        cursor_col++;
        if (cursor_col >= VGA_COLUMNS) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= VGA_ROWS) {
        vga_scroll();
    }

    vga_update_cursor();
}

static const struct console vga_console = {
    .name  = "vga",
    .putc  = vga_putc,
    .clear = vga_clear,
};

void vga_init(void) {
    console_register(&vga_console);
    vga_clear();
}
