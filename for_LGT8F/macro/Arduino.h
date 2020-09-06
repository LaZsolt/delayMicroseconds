#define delayMicroseconds(us)      \
(__extension__({                   \
  uint16_t _cyn__ = ( ((us) * (F_CPU / 1000L)) / 3000L ); \
  if ( _cyn__ ) {                  \
    __asm__ __volatile__ (         \
      "usL_%=:" "sbiw %0,1" "\n\t" \
                "brne usL_%="      \
               : /* no outputs */  \
               :"w" ( _cyn__ )     \
    );                             \
  }                                \
})) ;
