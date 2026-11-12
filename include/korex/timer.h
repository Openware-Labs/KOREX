#ifndef KOREX_TIMER_H
#define KOREX_TIMER_H

#include <korex/types.h>

#define HZ 100

void timer_init(void);
u64 timer_ticks(void);
void msleep(unsigned int ms);

#endif
