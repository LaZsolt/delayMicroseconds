#define delayMicroseconds(us)       \
    if (__builtin_constant_p(us)) { \
        delayMicroseconds_c(us);    \
    } else {                        \
        delayMicroseconds_v(us);    \
    }
#define delayMicroseconds_c(_us) _lgt8fx_delay_cycles((double)(_us)*(double)(F_CPU)/1000000)    // for constant case
 void   delayMicroseconds_v(unsigned int us) __attribute__ ((noinline));         // for variable case which is same as earlier

__inline__ void _lgt8fx_delay_cycles(const double) __attribute__((always_inline));
