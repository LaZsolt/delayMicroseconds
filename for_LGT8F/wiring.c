/* Delay for the given number of microseconds.  Assumes a 1, 2, 4, 8, 12, 16, 20, 24, 25 or 32 MHz clock. */

#define _NOP0() do {;} while (0)
#define _NOP1() do { __asm__ __volatile__ ("nop");} while (0)
#define _NOP2() do { __asm__ __volatile__ ("nop      \n\t nop"); } while (0)
#define _NOP3() do { __asm__ __volatile__ ("brcc .+0 \n\t brcs .+0"); } while (0)
#define _NOP4() do { __asm__ __volatile__ ("brcc .+0 \n\t brcs .+0 \n\t nop"); } while (0)
#define _NOP5() do { __asm__ __volatile__ ("brcc .+0 \n\t brcs .+0 \n\t nop      \n\t nop"); } while (0)
#define _NOP6() do { __asm__ __volatile__ ("brcc .+0 \n\t brcs .+0 \n\t brcc .+0 \n\t brcs .+0"); } while (0)
#ifdef _MORENOP_
  #undef _MORENOP_
#endif  

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

// Uncomment this line if you want to know your timing are clocktick accurate or even almost clocktick accurate.
// Uncomment this line if you want to know this "less acccurate" function is called or not
#pragma message "delayMicroseconds() called with variable."
  
    // call = 2 cycles + 2 to 4 cycles to init us (2 for constant delay, 1 or 4 for variable)
    // at the end of this function, return = 2 cycles in LGT MCU

#if F_CPU >= 32000000L
    // for the 32 MHz clock e.g. LGT8Fx Arduino compatibile boards
    // no zero delay fix

    // 32 MHz is two times faster than 16 MHz so need a lot more slower
    // wait cycle for keep the same delay capability than 16 MHz
    #define _MORENOP_ " brne .+0 \n\t  breq .+0 \n\t  nop \n\t"

    _NOP6(); // tuning

    // the following loop takes 1/4 of a microsecond (8 cycles with nops)
    // per iteration, so execute it four times for each microsecond of
    // delay requested.
    us <<= 2; // x4 us, = 4 cycles

    // account for the time taken in the preceeding commands.
    // LGT burned 16-19 cycles above, remove 2, (2*8=16)
    // us is at least 4 so we can substract 2
    us -= 2; // = 1 cycle

#elif F_CPU >= 25000000L
    // Ethernet crystal freq.

    // 25 MHz is faster so need 1 more nop in the wait cycle
    #define _MORENOP_ "nop \n\t"

    _NOP2(); // tuning

    // the following loop takes a 1/5 of a microsecond (5 cycles)
    // per iteration, so execute it six times for each microsecond of
    // delay requested.
    us = (us << 2) + us; // x5 us, = 7 cycles

    // account for the time taken in the preceeding commands.
    // we just burned 15 (17) cycles above, remove 3, (3*5=15)
    // us is at least 5 so we can substract 3
    us -= 3; // = 1 cycle
 
#elif F_CPU >= 24000000L
    // for the 24 MHz external clock (Maybe useful when working with USB)

    // Faster than 16 MHz so need some nop in the wait cycle
    // for keep the same delay capability than 16 MHz
    #define _MORENOP_ "nop \n\t  nop \n\t"

    _NOP2(); // tuning

    // the following loop takes a 1/4 of a microsecond (6 cycles)
    // per iteration, so execute it six times for each microsecond of
    // delay requested.
    us <<= 2; // x4 us, = 4 cycles

    // account for the time taken in the preceeding commands.
    // LGT burned 15 (17) cycles above, remove 4, (3*6=18)
    // us is at least 6 so we can substract 4
    us -= 2; // = 1 cycle

#elif F_CPU >= 20000000L
    // for the 20 MHz clock on rare Arduino boards

    // Need 1 nop in the wait cycle for keep 
    // the same delay capability than 16 MHz
    #define _MORENOP_ "nop \n\t"

    _NOP0(); // tuning

    // the following loop takes a 1/4 of a microsecond (5 cycles)
    // per iteration, so execute it five times for each microsecond of
    // delay requested.
    us <<= 2; // x4 us, = 4 cycles

    // account for the time taken in the preceeding commands.
    // we just burned 16-18 cycles above, remove 4, (4*4=16)
    // us is at least 5 so we can substract 4
    us -= 2; // = 1 cycle

#elif F_CPU >= 16000000L
    // for the 16 MHz clock on most Arduino boards

    _NOP2(); // tuning

    // the following loop takes 1/4 of a microsecond (4 cycles)
    // per iteration, so execute it four times for each microsecond of
    // delay requested.
    us <<= 2; // x4 us, = 4 cycles

    // account for the time taken in the preceeding commands.
    // we just burned 10-13 cycles above, remove 3, (3*4=12)
    // to 2nd us is at least 4 so we can substract 3
    us -= 3; // = 1 cycle

#elif F_CPU >= 12000000L
    // for the 12 MHz clock if somebody is working with USB

    _NOP4(); // tuning

    // for a 1 microsecond delay, simply return.  the overhead
    // of the function call takes 14 (16) cycles, which is 1.2us
    if (us <= 1) return; // = 3 cycles, (4 when true) (LGT too)

    _NOP1(); // tuning

    // the following loop takes 1/3 of a microsecond (4 cycles)
    // per iteration, so execute it three times for each microsecond of
    // delay requested.
    us = (us << 1) + us; // x3 us, = 5 cycles

    // account for the time taken in the preceeding commands.
    // we just burned 20 (22) cycles above, remove 5, (5*4=20)
    // to 2nd us is at least 8 so we can substract 5
    us -= 5; // = 1 cycle

#elif F_CPU >= 8000000L
    // for the 8 MHz clock

    // for a 1 microsecond delay, simply return.  the overhead
    // of the function call takes 9-12 cycles, which is ~1 us
    if (us <= 1) return; // = 3 cycles, (4 when true)

    _NOP1(); // tuning

    // the following loop takes 1/2 of a microsecond (4 cycles)
    // per iteration, so execute it twice for each microsecond of
    // delay requested.
    us <<= 1; //x2 us, = 2 cycles

    // account for the time taken in the preceeding commands.
    // we just burned 11-14 cycles above, remove 3, (3*4-1=11)
    // to 2rd us is at least 4 so we can substract 3
    us -= 3; // = 1 cycle

#elif F_CPU >= 4000000L
    // for the 4 MHz clock

    _NOP0(); // tuning

    // The overhead of the function call is 11 (13) cycles which is 3 us
    if (us <= 3) return;

    _NOP3(); // tuning

    // We don't need to multiply here because one request microsecond is exactly one loop cycle

    // Subtract microseconds that were wasted in this function
    us -= 3;

#elif F_CPU >= 2000000L
    // for the 2 MHz clock

    us >>= 1; // division by 2 = 2 cycles

    if (us <= 3) return;

    _NOP3(); // tuning

    // Subtract microseconds (7) that were wasted in this function
    us -= 3; // = 1 cycle

  #else
    // for the 1 MHz internal clock (default settings for common Atmega microcontrollers)

    if (us <= 10) return; //= 3 cycles, (4 when true)
 
    us >>= 2; // us div 4, = 4 cycles

    if (us <= 4) return;  //= 3 cycles, (4 when true)

    us -= 4; // = 1 cycle
    
    // the following loop takes 4 microseconds (4 cycles)
    // per iteration, so execute it us/4 times
    // us is at least 4, divided by 4 gives us 1 (no zero delay bug)

#endif

#ifndef _MORENOP_
  #define _MORENOP_
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

__inline__ void _lgt8fx_delay_cycles(const uint32_t cticks)
{
  uint32_t cticks_left = cticks;

  // loopcounter = 1               -> 7  clockticks    ( clockticks used = 3 + (loopcounter) * 5 - 1 )
  // loopcounter = 2               -> 12 clockticks
  // loopcounter = 16777215        -> 67108862 clocktics
  if (( cticks_left >= 262146L ) and ( cticks_left <= 83886077L ))
    {
      uint32_t lcount;
      lcount  = ( (cticks_left - 2) / 5);
      __builtin_avr_delay_cycles(lcount * 5 + 2);
      cticks_left -= ( lcount * 5 ) + 2;
    }

  // loopcounter = 1                 -> 4 clocktics    ( clockticks used = 2 + loopcounter * 3 - 1 )
  // loopcounter = 2                 -> 7 clocktics
  // loopcounter = 65535             -> 196606 clocktics
  if (( cticks_left >= 196607L ) and ( cticks_left <= 262145L ))
    {
      uint32_t lcount;
      lcount = ((cticks_left - 196605L) / 3);
      __builtin_avr_delay_cycles(lcount * 4 + 1);
      cticks_left -= lcount * 3 + 1;
    }
  if (( cticks_left >= 768L ) and ( cticks_left <= 196606L ))
    {
      uint32_t lcount;
      lcount = ((cticks_left - 1) / 3);
      __builtin_avr_delay_cycles(lcount * 4 + 1);
      cticks_left -= lcount * 3 + 1;
    }
  // loopcounter = 1                 -> 3 clocktics    ( clockticks used = 1 + loopcounter * 3 - 1 )
  // loopcounter = 2                 -> 6 clocktics
  // loopcounter = 255               -> 765 clocktics
  if (( cticks_left >= 6L ) and ( cticks_left <= 767L ))
    {
      uint32_t lcount;
      lcount =  cticks_left / 3;
      __builtin_avr_delay_cycles(lcount * 3);
      cticks_left -= lcount * 3;
    }
                                    // 6 clockticks:  asm( "breq  6f \n\t 6: brne  7f \n\t 7: breq  8f \n\t 8: brne  9f \n\t 9: \n\t" );
                                    // 6 clockticks:  asm( "brcs  7f \n\t 7: brcs  8f \n\t 8: brcs  9f \n\t 9: \n\t" );
                                    // 6 clockticks:  asm( "brcs .+0 \n\t    brcs .+0 \n\t    brcs .+0 \n\t" );    if carry is set
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
      __asm__ __volatile__ ("nop \n\t nop");
      cticks_left -= 2;
    }

  if (cticks_left == 1)
    {
      __asm__ __volatile__ ("nop");
      cticks_left -= 1;
    }
}
