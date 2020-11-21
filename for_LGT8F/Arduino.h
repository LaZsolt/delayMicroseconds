#define delayMicroseconds(us) do { if (__builtin_constant_p(us)) { delayMicroseconds_c(us); } else { delayMicroseconds_v(us); } } while (0)
#define delayMicroseconds_c(us) _lgt8fx_delay_cycles((uint32_t)(  (F_CPU/1e6)*((double)us)  )) // for constant case
 void   delayMicroseconds_v(unsigned int us) __attribute__ ((noinline));                       // for variable case

__inline__ void _lgt8fx_delay_cycles(const uint32_t cticks) __attribute__((always_inline));
