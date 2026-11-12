#ifndef KOREX_VERSION_H
#define KOREX_VERSION_H

struct korex_version {
    const char* name;
    const char* release;
    const char* machine;
};

extern const struct korex_version korex_version;

#endif
