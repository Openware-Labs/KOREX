#ifndef USR_SHELL_H
#define USR_SHELL_H

#define SHELL_PROMPT   "Korex> "
#define SHELL_MAX_ARGS 8

struct shell_command {
    const char* name;
    const char* summary;
    int (*main)(int argc, char** argv);
    int hidden;
};

extern const struct shell_command shell_commands[];
extern const unsigned int shell_command_count;

void shell_init(void);
void shell_execute(char* line);

#endif
