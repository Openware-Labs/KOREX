#include <korex/kernel.h>
#include <korex/string.h>
#include <usr/bin.h>
#include <usr/shell.h>

#define HELP_NAME_COLUMN 10

int cmd_help(int argc __unused, char** argv __unused) {
    printk("Commands:\n");

    for (unsigned int i = 0; i < shell_command_count; i++) {
        const struct shell_command* cmd = &shell_commands[i];

        if (cmd->hidden) {
            continue;
        }

        printk("  %s", cmd->name);
        for (size_t pad = strlen(cmd->name); pad < HELP_NAME_COLUMN; pad++) {
            printk(" ");
        }
        printk("%s\n", cmd->summary);
    }
    return 0;
}
