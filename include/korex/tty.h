#ifndef KOREX_TTY_H
#define KOREX_TTY_H

typedef void (*tty_line_handler_t)(char* line);
typedef void (*tty_bell_handler_t)(void);

void tty_set_line_handler(tty_line_handler_t handler);
void tty_set_bell_handler(tty_bell_handler_t handler);

void tty_push(char c);
int tty_pending(void);
void tty_poll(void);
void tty_bell(void);

#endif
