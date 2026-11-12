#include <korex/kernel.h>
#include <korex/console.h>
#include <korex/version.h>
#include <asm/cpu.h>
#include <usr/bin.h>

static const char* const logo[] = {
    "  _  __ ___  ____  _______  __\n",
    " | |/ // _ \\|  _ \\| ____\\ \\/ /\n",
    " | ' /| | | | |_) |  _|  \\  / \n",
    " | . \\| |_| |  _ <| |___ /  \\ \n",
    " |_|\\_\\\\___/|_| \\_\\_____/_/\\_\\\n",
};

#define SEPARATOR "------------------------------------\n"

int cmd_sysinfo(int argc __unused, char** argv __unused) {
    char vendor[CPU_VENDOR_LEN];
    char brand[CPU_BRAND_LEN];

    cpu_get_vendor(vendor);
    cpu_get_brand(brand);

    for (unsigned int i = 0; i < sizeof(logo) / sizeof(logo[0]); i++) {
        console_write(logo[i], CONSOLE_COLOR_INFO);
    }
    console_write(SEPARATOR, CONSOLE_COLOR_DEFAULT);

    printk("OS:         %s\n", korex_version.name);
    printk("Version:    v%s\n", korex_version.release);
    printk("Arch:       %s\n", korex_version.machine);
    printk("CPU Vendor: %s\n", vendor);
    printk("CPU Brand:  %s\n", brand);

    console_write(SEPARATOR, CONSOLE_COLOR_DEFAULT);
    return 0;
}
