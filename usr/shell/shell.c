#include <korex/kernel.h>
#include <korex/string.h>
#include <korex/tty.h>
#include <usr/shell.h>

static const struct shell_command* shell_find(const char* name) {
    for (unsigned int i = 0; i < shell_command_count; i++) {
        if (strcmp(shell_commands[i].name, name) == 0) {
            return &shell_commands[i];
        }
    }
    return NULL;
}

static int shell_tokenize(char* line, char** argv, int max_args) {
    int argc = 0;

    while (*line) {
        while (*line == ' ') {
            *line++ = '\0';
        }
        if (*line == '\0' || argc == max_args) {
            break;
        }
        argv[argc++] = line;
        while (*line && *line != ' ') {
            line++;
        }
    }
    return argc;
}

static void shell_prompt(void) {
    printk("%s", SHELL_PROMPT);
}

void shell_execute(char* line) {
    char* argv[SHELL_MAX_ARGS];
    int argc = shell_tokenize(line, argv, SHELL_MAX_ARGS);

    if (argc > 0) {
        const struct shell_command* cmd = shell_find(argv[0]);

        if (cmd) {
            cmd->main(argc, argv);
        } else {
            printk("Unknown command.\n");
            tty_bell();
        }
    }

    shell_prompt();
}

void shell_init(void) {
    tty_set_line_handler(shell_execute);
    shell_prompt();
}
