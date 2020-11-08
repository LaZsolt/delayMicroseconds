/* Delay for the given number of microseconds.  Assumes a 1, 2, 4, 8, 12, 16, 20, 24, 25 or 32 MHz clocks. */

#define _NOP1() do { __asm__ __volatile__ ("nop");} while (0)
#define _NOP2() do { __asm__ __volatile__ ("nop      \n\t nop"); } while (0)
#define _NOP3() do { __asm__ __volatile__ ("brcc .+0 \n\t brcs .+0"); } while (0)
#define _NOP4() do { __asm__ __volatile__ ("brcc .+0 \n\t brcs .+0 \n\t nop"); } while (0)
#define _NOP5() do { __asm__ __volatile__ ("brcc .+0 \n\t brcs .+0 \n\t nop      \n\t nop"); } while (0)
#define _NOP6() do { __asm__ __volatile__ ("brcc .+0 \n\t brcs .+0 \n\t brcc .+0 \n\t brcs .+0"); } while (0)
#define _MORENOP_ ""

/* Link time optimization (LTO for short) has been supported by the IDE since v1.6.11.
 * In Arduino IDE 1.6.11 and newer LTO is enabled by default.  The LTO optimizes the code
 * at link time, making the code (often) significantly smaller without making it "slower"
 * and sometimes destroy acccurate software timings like delayMicroseconds() with lower values.
 * To avoid LTO optimization, the line of delayMicrosecons() definition in arduino.h must be replace to this:
 * void delayMicroseconds(unsigned int) __attribute__ ((noinline)) ;
 * Any other optimization options like this line below no effet on LTO.
 * void __attribute__((optimize("O1"))) foo(unsigned char data) {
 *   // unmodifiable compiler code
 * }
 */
void delayMicroseconds_v(unsigned int us)
{
    // call = 2 cycles + 2 to 4 cycles to init us (2 for constant delay, 1 or 4 for variable)
    // at the end of this function, return = 2 cycles in LGT MCU

#if F_CPU >= 32000000L
    // for the 32 MHz clock e.g. LGT8Fx Arduino compatibile boards
    // no zero delay fix

    // the following loop takes 1/4 of a microsecond (8 cycles with nops)
    // per iteration, so execute it four times for each microsecond of
    // delay requested.
    us <<= 2; // x4 us, = 4 cycles

    // 32 MHz is two times faster than 16 MHz so need more nop in the
    // wait cycle for keep the same delay capability than 16 MHz
#undef  _MORENOP_
#define _MORENOP_ " brne .+0 \n\t  breq .+0 \n\t  nop \n\t"

    // account for the time taken in the preceeding commands.
    // LGT burned 16 (18) cycles above, remove 2, (2*8=16)
    // us is at least 4 so we can substract 2
    us -= 2; // = 1 cycle
    _NOP6(); // tuning

#elif F_CPU >= 25000000L
    // the following loop takes a 1/5 of a microsecond (5 cycles)
    // per iteration, so execute it six times for each microsecond of
    // delay requested.
    us = (us << 2) + us; // x5 us, = 7 cycles

    // 25 MHz is faster so need 1 more nop in the wait cycle
#undef  _MORENOP_
#define _MORENOP_ " nop \n\t"

    // account for the time taken in the preceeding commands.
    // we just burned 14 (16) cycles above, remove 3, (3*5=15)
    // us is at least 5 so we can substract 3
    us -= 3; // = 1 cycle
    _NOP1(); // tuning
 
#elif F_CPU >= 24000000L
    // for the 24 MHz external clock if somebody is working with USB

    // is there any reason for zero delay fix?
    //if (!us) return; // = 3 cycles, (4 when true) (LGT same)

    // the following loop takes a 1/6 of a microsecond (4 cycles)
    // per iteration, so execute it six times for each microsecond of
    // delay requested.
    us *= 6; // x6 us, = 9 cycles [{ us = (us<<2)+(us<<1); = 9 cycles too }]
             // ( x6 us, = 7 cycles in LGT )
    // account for the time taken in the preceeding commands.
#if defined(__LGT8F__)
    _NOP(); //just waiting 1 cycle
    // LGT burned 15 (17) cycles above, remove 4, (4*4=16)
    // us is at least 6 so we can substract 4
    us -= 4; // = 1 cycle
#else
    // we just burned 21 (23) cycles above, remove 5, (5*4=20)
    // us is at least 6 so we can substract 5
    us -= 5; // = 2 cycles
#endif

#elif F_CPU >= 20000000L
    // for the 20 MHz clock on rare Arduino boards

#if defined(__LGT8F__)
    __asm__ __volatile__ (
        "nop" "\n\t"
        "nop"); //just waiting 2 cycles
    // no zero delay fix
#else
    // for a one-microsecond delay, simply return.  the overhead
    // of the function call takes 19 (21) cycles, which is 1us
    __asm__ __volatile__ (
        "brne .+0 \n\t"
        "breq .+0 \n\t"
        "nop      \n\t"
        "nop"); //just waiting 5 cycles
    if (us <= 1) return; // = 3 cycles, (4 when true)
#endif

    // the following loop takes a 1/5 of a microsecond (4 cycles)
    // per iteration, so execute it five times for each microsecond of
    // delay requested.
    us = (us << 2) + us; // x5 us, = 7 cycles

    // account for the time taken in the preceeding commands.
#if defined(__LGT8F__)
    // we just burned 16 (18) cycles above, remove 4, (4*4=16)
    // us is at least 5 so we can substract 4
    us -= 4; // = 1 cycle
#else
    // we just burned 27 (29) cycles above, remove 7, (7*4=28)
    // to 2nd us is at least 10 so we can substract 7
    us -= 7; // = 2 cycles
#endif

#elif F_CPU >= 16000000L
    // for the 16 MHz clock on most Arduino boards

    // the following loop takes 1/4 of a microsecond (4 cycles)
    // per iteration, so execute it four times for each microsecond of
    // delay requested.
    us <<= 2; // x4 us, = 4 cycles

    // account for the time taken in the preceeding commands.
    // we just burned 10-13 cycles above, remove 3, (3*4-1=11)
    // to us is at least 4 so we can substract 3
    us -= 3; // = 1 cycle
    _NOP2(); //tuning

#elif F_CPU >= 12000000L
    // for the 12 MHz clock if somebody is working with USB

#if defined(__LGT8F__)
    __asm__ __volatile__ (
        "brne .+0 \n\t"
        "breq .+0 \n\t"
        "nop"); // waiting 4 missing cycles in LGT
#endif
    // for a 1 microsecond delay, simply return.  the overhead
    // of the function call takes 14 (16) cycles, which is 1.2us
    if (us <= 1) return; // = 3 cycles, (4 when true) (LGT too)

    // the following loop takes 1/3 of a microsecond (4 cycles)
    // per iteration, so execute it three times for each microsecond of
    // delay requested.
    us = (us << 1) + us; // x3 us, = 5 cycles

    // account for the time taken in the preceeding commands.
    // we just burned 20 (22) cycles above, remove 5, (5*4=20)
    // to 2nd us is at least 6 so we can substract 5
#if defined(__LGT8F__)
    _NOP(); //compensate the missing cycle below
#endif
    us -= 5; //2 cycles

#elif F_CPU >= 8000000L
    // for the 8 MHz clock

    // for a 1 microsecond delay, simply return.  the overhead
    // of the function call takes 9-12 cycles, which is ~1 us
    if (us <= 1) return; // = 3 cycles, (4 when true)

    // the following loop takes 1/2 of a microsecond (4 cycles)
    // per iteration, so execute it twice for each microsecond of
    // delay requested.
    us <<= 1; //x2 us, = 2 cycles

    // account for the time taken in the preceeding commands.
    // we just burned 11-14 cycles above, remove 3, (3*4-1=11)
    // to 2rd us is at least 4 so we can substract 3
    us -= 3; // = 1 cycle
    _NOP1(); // tuning

#elif F_CPU >= 4000000L
    // for the 4 MHz clock

    __asm__ __volatile__ ("nop"); //just waiting 1 cycle
#if defined(__LGT8F__)
    // The overhead of the function call is 11 (13) cycles which is 3 us
    if (us <= 3) return;

    // Subtract microseconds that were wasted in this function
    _NOP(); //compensate the missing cycle below
    us -= 3;
#else
    // The overhead of the function call is 15 (17) cycles which is 4 us
    if (us <= 4) return;

    // Subtract microseconds that were wasted in this function
  us -= 4;
#endif
    // We don't need to multiply here because one request microsecond is exactly one loop cycle

#elif F_CPU >= 2000000L
    // for the 2 MHz clock

#if defined(__LGT8F__)
    __asm__ __volatile__ (
        "brne .+0 \n\t"
        "breq .+0 \n\t"
        "nop"); // waiting 4 missing cycles in LGT
#endif
  // the overhead of the function call is 14 (16) cycles which is ~7 us
  // if someone request less than 11 us it will delay ~7 microseconds
  if (us <= 10) return;

  // Subtract microseconds (9) that were wasted in this function
  // so we can substract 9 ( every 2nd delay accurate 11 us and above)
#if defined(__LGT8F__)
    _NOP(); //compensate the missing cycle below
#endif
  us -= 9;  // = 2 cycles
  us >>= 1; // division by 2 = 2 cycles
  _NOP;     // waiting 1 cycle to adjust time
  
#else
    // for the 1 MHz internal clock (default settings for common Atmega microcontrollers)

    // the overhead of the function calls is 14 (16) cycles
    if (us <= 16) return; //= 3 cycles, (4 when true)
    if (us <= 25) return; //= 3 cycles, (4 when true), (must be at least 25 if we want to substract 22)

    // compensate for the time taken by the preceeding and next commands (about 22 cycles)
    us -= 22; // = 22 cycles
    // the following loop takes 4 microseconds (4 cycles)
    // per iteration, so execute it us/4 times
    // us is at least 4, divided by 4 gives us 1 (no zero delay bug)
    us >>= 2; // us div 4, = 4 cycles

#endif

    // busy wait
    __asm__ __volatile__ (
        "1: sbiw %0,1   \n\t"            // 1 cycle in LGT
        "   nop         \n\t"            // 1 cycle
            _MORENOP_                    // 4 cycles if 32 MHz or 1 cycle if 25 MHz
        "   brne 1b"                     // 2 cycles ( 1 cycle when counter became 0 )
        : "=w" (us)                      // No outputs, but it is inform the compiler about modified register
        : "0"  (us)
    );
    // return = 2 cycles in LGT MCU
}

// AVR-GCC Inline Assembler Cookbook : https://www.nongnu.org/avr-libc/user-manual/inline_asm.html

static __inline__ void _lgt8fx_dloop_1(uint8_t  ct) __attribute__((always_inline));
static __inline__ void _lgt8fx_dloop_2(uint16_t ct) __attribute__((always_inline));
static __inline__ void _lgt8fx_dloop_3(uint32_t ct) __attribute__((always_inline));

static __inline__ void _lgt8fx_dloop_1(uint8_t ct)
{
  __asm__ __volatile__ (            // 1 clockticks to init counter
    "1: subi %0,1            \n\t"  // 1 clocktick in LGT
    "   brcc 1b"                    // 2 clockticks ( 1 when counter became -1 )
    : "=r" (ct)                     // No outputs, but it is inform the compiler about modified register
    : "0"  (ct)
  );                                // Sum: 1 + (ct+1) * 3 - 1
}

static __inline__ void _lgt8fx_dloop_2(uint16_t ct)
{
  __asm__ __volatile__ (            // 2 clockticks to init counter
    "1: sbiw %0,1            \n\t"  // 1 clocktick in LGT
    "   brcc 1b"                    // 2 clockticks ( 1 when counter became -1 )
    : "=w" (ct)                     // No outputs, but it is inform the compiler about modified register
    : "0"  (ct)
  );                                // Sum: 2 + (ct+1) * 3 - 1
}

static __inline__ void _lgt8fx_dloop_3(uint32_t ct)
{
  __asm__ __volatile__ (            // 4 clockticks to init counter ( one more instruction than necessary )
    "1: sbiw %A0,1            \n\t" // 1 clocktick in LGT
    "   sbc  %C0,__zero_reg__ \n\t" // 1 clocktick
    "   brcc 1b"                    // 2 clockticks ( 1 when counter became -1 )
    : "=r" (ct)                     // No outputs, but it is inform the compiler about modified register
    : "0"  (ct)
  );                                // Sum: 4 + (ct+1) * 4 - 1
}

__inline__ void _lgt8fx_delay_cycles(const uint32_t cticks)
{
  uint32_t cticks_left;
  boolean CarryIsSet;

  cticks_left = cticks;
  CarryIsSet  = false;
  // AtMega loopcounter = 0        -> 8  clockticks    ( clockticks used = 4 + (loopcounter+1) * 5 - 1 )
  // AtMega loopcounter = 1        -> 13 clockticks
  // AtMega loopcounter = 16777215 -> 83886083 clockcycles
  // AtMega if (( cticks_left >= 262146 ) and ( cticks_left <= 83886083 ))
  
  // loopcounter = 0               -> 7  clockticks    ( clockticks used = 4 + (loopcounter+1) * 4 - 1 )
  // loopcounter = 1               -> 11 clockticks
  // loopcounter = 16777215        -> 67108867 clocktics
  if (( cticks_left >= 196610L ) and ( cticks_left <= 67108867L ))
    {
      uint32_t lcount;
      lcount  = ( (cticks_left - 3) / 4);                        // AtMega: ( (cticks_left - 3) / 5)
      _lgt8fx_dloop_3(lcount-1);
      cticks_left -= ( lcount * 4 ) + 3;                         // AtMega: ( (lcount+1) * 5) + 3
      CarryIsSet = true;
    }

  // AtMega loopcounter = 0          -> 5 clocktics    ( clockticks used = 2 + (loopcounter+1) * 4 - 1 )
  // AtMega loopcounter = 1          -> 9 clocktics
  // AtMega loopcounter = 65535      -> 262145 clocktics
  // AtMega if (( cticks_left >= 769 ) and ( cticks_left <= 262145 ))
  
  // loopcounter = 0                 -> 4 clocktics    ( clockticks used = 2 + (loopcounter+1) * 3 - 1 )
  // loopcounter = 1                 -> 7 clocktics
  // loopcounter = 65535             -> 196609 clocktics
  if (( cticks_left >= 769L ) and ( cticks_left <= 196609L ))
    {
      uint32_t lcount;
      lcount = ( (cticks_left - 1) / 3);                         // AtMega: (cticks_left - 1) / 4
      _lgt8fx_dloop_2((uint16_t)(lcount-1));
      cticks_left -= ( (lcount) * 3) + 1;                        // AtMega: ( (lcount+1) * 4) + 1
      CarryIsSet = true;
    }

  // AtMega loopcounter = 0          -> 3 clocktics    ( clockticks used = 1 + (loopcounter+1) * 3 - 1 )
  // AtMega loopcounter = 1          -> 6 clocktics
  // AtMega loopcounter = 255        -> 768 clocktics
  // AtMega if (( cticks_left >= 5 ) and ( cticks_left <= 768 ))
  
  // loopcounter = 0                 -> 3 clocktics    ( clockticks used = 1 + (loopcounter+1) * 3 - 1 )
  // loopcounter = 1                 -> 6 clocktics
  // loopcounter = 255               -> 768 clocktics
  if (( cticks_left >= 5L ) and ( cticks_left <= 768L ))
    {
      uint32_t lcount;
      lcount = (cticks_left / 3);                                // AtMega: ( cticks_left / 3 )
      _lgt8fx_dloop_1((uint8_t)(lcount-1));
      cticks_left -= (lcount) * 3;                               // AtMega: (lcount+1) * 3
      CarryIsSet = true;
    }
                                    // 6 clockticks:  asm( "breq  6f \n\t 6: brne  7f \n\t 7: breq  8f \n\t 8: brne  9f \n\t 9: \n\t" );
                                    // 6 clockticks:  asm( "brcs  7f \n\t 7: brcs  8f \n\t 8: brcs  9f \n\t 9: \n\t" );
                                    // 6 clockticks:  asm( "brcs .+0 \n\t    brcs .+0 \n\t    brcs .+0 \n\t" );
  if ((cticks_left == 4) && (CarryIsSet))
    {
      __asm__ __volatile__ (        // Carry is set after counting to -1
        "brcs .+0 \n\t"
        "brcs .+0"
      ); 
      cticks_left -= 4;
    }

  while (cticks_left >= 3)
    {
      __asm__ __volatile__ (        // 2 + 1 clock cycles or 1 + 2 clock cycles
        "brcs .+0 \n\t"
        "brcc .+0 \n\t"
        : :
      );
      cticks_left -= 3;
    }

  if (cticks_left == 2)
    {
      if (CarryIsSet)
      {
        __asm__ __volatile__ ("brcs .+0"); // Carry is set after counting to -1
      }
      else 
      {
        __asm__ __volatile__ ("nop \n\t nop");
      }
      cticks_left -= 2;
    }

  if (cticks_left == 1)
    {
      __asm__ __volatile__ ("nop");
      cticks_left -= 1;
    }
}
 
