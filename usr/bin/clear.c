#include <korex/console.h>
#include <usr/bin.h>

int cmd_clear(int argc __unused, char** argv __unused) {
    console_clear();
    return 0;
}
