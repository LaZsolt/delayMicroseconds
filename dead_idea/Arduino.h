#define clockCyclesPerMicrosecond()  ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
#define microsecondsToClockCycles(a) ( ((a) * (F_CPU / 1000L)) / 1000L )

...

#define delayMicroseconds(us)       \
    if (__builtin_constant_p(us)) { \
        if (us) delayMicroseconds_c(us);    \
    } else {                        \
        delayMicroseconds_v(us);    \
    }
void delayMicroseconds_c(unsigned int us);
void delayMicroseconds_v(unsigned int us) __attribute__ ((noinline));
