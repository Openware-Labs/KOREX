#include <korex/tty.h>
#include <usr/bin.h>

int cmd_beep(int argc __unused, char** argv __unused) {
    tty_bell();
    return 0;
}
