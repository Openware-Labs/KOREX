#include <korex/version.h>
#include <asm/setup.h>
#include <generated/version.h>

const struct korex_version korex_version = {
    .name    = KOREX_NAME,
    .release = KOREX_VERSION,
    .machine = ARCH_NAME,
};
