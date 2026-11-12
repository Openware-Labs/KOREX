#include <korex/tty.h>
#include <korex/console.h>
#include <korex/types.h>

#define TTY_INPUT_RING_SIZE 128
#define TTY_LINE_SIZE       64

static volatile char input_ring[TTY_INPUT_RING_SIZE];
static volatile unsigned int ring_head;
static volatile unsigned int ring_tail;

static char line[TTY_LINE_SIZE];
static unsigned int line_len;

static tty_line_handler_t line_handler;
static tty_bell_handler_t bell_handler;

void tty_set_line_handler(tty_line_handler_t handler) {
    line_handler = handler;
}

void tty_set_bell_handler(tty_bell_handler_t handler) {
    bell_handler = handler;
}

void tty_bell(void) {
    if (bell_handler) {
        bell_handler();
    }
}

void tty_push(char c) {
    unsigned int next = (ring_head + 1) % TTY_INPUT_RING_SIZE;

    if (next == ring_tail) {
        return;
    }
    input_ring[ring_head] = c;
    ring_head = next;
}

int tty_pending(void) {
    return ring_head != ring_tail;
}

static void tty_receive(char c) {
    if (c == '\n') {
        console_putc('\n', CONSOLE_COLOR_DEFAULT);
        line[line_len] = '\0';
        line_len = 0;
        if (line_handler) {
            line_handler(line);
        }
    } else if (c == '\b') {
        if (line_len > 0) {
            line_len--;
            console_putc('\b', CONSOLE_COLOR_DEFAULT);
        } else {
            tty_bell();
        }
    } else if (c >= ' ' && c <= '~') {
        if (line_len < TTY_LINE_SIZE - 1) {
            line[line_len++] = c;
            console_putc(c, CONSOLE_COLOR_DEFAULT);
        }
    }
}

void tty_poll(void) {
    while (ring_tail != ring_head) {
        char c = input_ring[ring_tail];

        ring_tail = (ring_tail + 1) % TTY_INPUT_RING_SIZE;
        tty_receive(c);
    }
}
