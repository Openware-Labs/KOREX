#include <korex/compiler.h>
#include <usr/shell.h>
#include <usr/bin.h>

const struct shell_command shell_commands[] = {
    { "help",    "show this list",          cmd_help,    0 },
    { "clear",   "clear the screen",        cmd_clear,   0 },
    { "beep",    "play a beep",             cmd_beep,    0 },
    { "sysinfo", "show system information", cmd_sysinfo, 0 },
    { "anime",   "",                        cmd_anime,   1 },
};

const unsigned int shell_command_count = ARRAY_SIZE(shell_commands);
