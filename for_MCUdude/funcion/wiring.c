/* Delay for the given number of microseconds.  Assumes a 1, 1.8432, 2, 3.6864, 4,
 * 7.3728, 8, 9.216, 10, 11.0592, 12, 14.7456, 16, 18.432, 20, 24, 25 or 32 MHz clock. */

#define _MORENOP_ ""

/* Link time optimization (LTO for short) has been supported by the IDE since v1.6.11.
 * In Arduino IDE 1.6.11 and newer LTO is enabled by default.  The LTO optimizes the code
 * at link time, making the code (often) significantly smaller without making it "slower"
 * and sometimes destroy acccurate software timings like delayMicroseconds() with lower values.
 * To avoid LTO optimization, the line of delayMicrosecons() definition in arduino.h must be replace to this:
 * void delayMicroseconds(unsigned int) __attribute__ ((noinline)) ;
 */
void delayMicroseconds(unsigned int us)
{
  // call = 4 cycles + 2 to 4 cycles to init us(2 for constant delay, 4 for variable)

  // calling avrlib's delay_us() function with low values (e.g. 1 or
  // 2 microseconds) gives delays longer than desired.
  //delay_us(us);

#if F_CPU >= 32000000L
  // No reason for zero delay fix
  // if (!us) return; // = 3 cycles, (4 when true)

  // the following loop takes a 1/4 of a microsecond (8 cycles with nops)
  // per iteration, so execute it four times for each microsecond of
  // delay requested.
  us <<= 2; // x4 us, = 4 cycles

  // 32 MHz is two times faster than 16 MHz so need more nop in the
  // wait cycle and keep the same delay capability than 16 MHz
#undef  _MORENOP_
#define _MORENOP_ " nop \n\t  nop \n\t  nop \n\t  nop \n\t"

  // account for the time taken in the preceeding commands.
  // we just burned 16 (18) cycles above, remove 3, (2*8=16)
  // us is at least 4 so we can substract 2
  us -= 2; // = 2 cycles

// # elif F_CPU >= 29491200L

#elif F_CPU >= 25000000L
  // Is there any reason for zero delay fix?
  // if (!us) return; // = 3 cycles, (4 when true)

  // the following loop takes a 1/5 of a microsecond (5 cycles)
  // per iteration, so execute it six times for each microsecond of
  // delay requested.
  us = (us << 2) + us; // x5 us, = 7 cycles

#undef  _MORENOP_
#define _MORENOP_ " nop \n\t"

  // account for the time taken in the preceeding commands.
  // we just burned 19 (21) cycles above, remove 4, (4*5=20)
  // us is at least 5 so we can substract 4
  us -= 4; // = 2 cycles
  
#elif F_CPU >= 24000000L
  // for the 24 MHz external clock if somebody is working with USB

  // Is there any reason for zero delay fix?
  // if (!us) return; // = 3 cycles, (4 when true)

  // the following loop takes a 1/6 of a microsecond (4 cycles)
  // per iteration, so execute it six times for each microsecond of
  // delay requested.
  us *= 6; // x6 us, = 9 cycles [{ us = (us<<2)+(us<<1); = 9 cycles too }]

  // account for the time taken in the preceeding commands.
  // we just burned 21 (23) cycles above, remove 5, (5*4=20)
  // us is at least 6 so we can substract 5
  us -= 5; // = 2 cycles

// # elif F_CPU >= 22118400L

#elif F_CPU >= 20000000L
  __asm__ __volatile__ (
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop"); // just waiting 5 cycles
  // for a one-microsecond delay, simply return.  the overhead
  // of the function call takes 19 (21) cycles, which is 1us
  if (us <= 1) return; //  = 3 cycles, (4 when true)

  // the following loop takes a 1/5 of a microsecond (4 cycles)
  // per iteration, so execute it five times for each microsecond of
  // delay requested.
  us = (us << 2) + us; // x5 us, = 7 cycles

  // account for the time taken in the preceeding commands.
  // we just burned 27 (29) cycles above, remove 7, (7*4=28)
  // to 2nd us is at least 10 so we can substract 7
  us -= 7; // = 2 cycles

#elif F_CPU >= 18432000L
  __asm__ __volatile__ (
    "nop" "\n\t"
    "nop" "\n\t"
    "nop" "\n\t"
    "nop"); // just waiting 4 cycles
  // for a one-microsecond delay, simply return.  the overhead
  // of the function call takes 18 (20) cycles, which is aprox. 1us
  if (us <= 1) return; // = 3 cycles, (4 when true)

  // the following loop takes nearly 1/5 (0.217%) of a microsecond (4 cycles)
  // per iteration, so execute it five times for each microsecond of
  // delay requested.
  us = (us << 2) + us; // x5 us, = 7 cycles

                       // +1 cycle (register save)
  // user wants to wait longer than 3 us
  if (us > 15) // = 3 cycles
  {
    // Since the loop is not accurately 1/5 of a microsecond we need
    // to multiply us by 0.9216 (18.432 / 20)
    //us = (us >> 1) + (us >> 2) + (us >> 3) + (us >> 4); // x0.9375 us, = 50 cycles (really!)
    us = (us * 60398L) >> 16;   // x0.9216 us = 29 cycles (60398 = 0.9216 * 0x10000L)

    // account for the time taken in the preceeding commands.
    // we just burned 59 (61) cycles above, remove 15, (15*4=60)
    us -= 15; // = 2 cycles
  }
  else 
  {
    // account for the time taken in the preceeding commands.
    // we just burned 33 (35) cycles above, remove 9, (9*4=36)

             // 1 cycle when if jump here
    us -= 9; // 2 cycles
             // 2 cycles to jump back to delay cycle.
  }

#elif F_CPU >= 16000000L
  // for a one-microsecond delay, simply return.  the overhead
  // of the function call takes 14 (16) cycles, which is 1 us
  if (us <= 1) return; //  = 3 cycles, (4 when true)

  // the following loop takes 1/4 of a microsecond (4 cycles)
  // per iteration, so execute it four times for each microsecond of
  // delay requested.
  us <<= 2; // x4 us, = 4 cycles

  // account for the time taken in the preceeding commands.
  // we just burned 19 (21) cycles above, remove 5, (5*4=20)
  // to 2nd us is at least 8 so we can substract 5
  us -= 5; // = 2 cycles,

#elif F_CPU >= 14745600L
  // The overhead of the function call is 14 (16) cycles which is 1 us
  if (us <= 1) return; //  = 3 cycles, (4 when true)

  us <<= 2; // x4 us, = 4 cycles

                       // +1 cycle (register save)
  // user wants to wait longer than 3 us
  if (us > 15) // = 3 cycles
  {
    // Since the loop is not accurately 1/4 of a microsecond we need
    // to multiply us by 0.9216 (14.7456 / 16)
    us = (us * 60398L) >> 16;   // x0.9216 us = 29 cycles (60398 = 0.9216 x 0x10000L)

    // account for the time taken in the preceeding commands.
    // we just burned 53 (57) cycles above, remove 13, (13*4=52)
    us -= 13; // = 2 cycles
  }
  else 
  {
    // account for the time taken in the preceeding commands.
    // we just burned 27 (29) cycles above, remove 7, (7*4=28)

             // 1 cycle when if jump here
    us -= 7; // 2 cycles
             // 2 cycles to jump back to delay cycle.
  }

#elif F_CPU >= 12000000L
  // for the 12 MHz clock if somebody is working with USB

  // for a 1 microsecond delay, simply return.  the overhead
  // of the function call takes 14 (16) cycles, which is 1.2 us
  if (us <= 1) return; // = 3 cycles, (4 when true)

  // the following loop takes 1/3 of a microsecond (4 cycles)
  // per iteration, so execute it three times for each microsecond of
  // delay requested.
  us = (us << 1) + us; // x3 us, = 5 cycles

  // account for the time taken in the preceeding commands.
  // we just burned 20 (22) cycles above, remove 5, (5*4=20)
  // us is at least 6 so we can substract 5
  us -= 5; // = 2 cycles

#elif F_CPU >= 11059200L
  // the overhead of the function call is 14 (16) cycles which is ~1.3 us
  if (us <= 2) return; // = 3 cycles, (4 when true)

  us = (us << 1) + us; // x3 us, = 5 cycles

                       // +1 cycle (register save)
  // user wants to wait longer than 4 us
  if (us > 14) // = 3 cycles
  {
    // since the loop is not accurately 1/3 of a microsecond we need
    // to multiply us by 0.9216 (11.0592 / 12)
    us = (us * 60398L) >> 16;   // x0.9216 us = 29 cycles (60398 = 0.9216 x 0x10000L)

    // account for the time taken in the preceeding commands.
    // we just burned 53 (55) cycles above, remove 13, (13*4=52)
    us -= 13; // = 2 cycles
  }
  else
  {
    // account for the time taken in the preceeding commands.
    // we just burned 27 (29) cycles above, remove 7, (7*4=28)

             // 1 cycle when if jump here
    us -= 7; // 2 cycles
             // 2 cycles to jump back to delay cycle.
  }

#elif F_CPU >= 10000000L
  // for the 10 MHz clock

  // for a 1 or 2 microsecond delay, simply return.  the overhead
  // of the function call takes 14 (16) cycles, which is 1.5 us
  if (us <= 2) return; // = 3 cycles, (4 when true)

  // the following loop takes 2/5 of a microsecond (4 cycles)
  // per iteration, so execute it three times for each microsecond of
  // delay requested.
  us = (us << 1) + (us >> 1); // x2.5 us, = 7 cycles

  // account for the time taken in the preceeding commands.
  // we just burned 22 (24) cycles above, remove 5, (5*4=20)
  // us is at least 20 so we can substract 5
  us -= 5; // = 2 cycles

# elif F_CPU >= 9216000L
  // the overhead of the function call is 14 (16) cycles which is ~1.5 us
  if (us <= 3) return; // = 3 cycles, (4 when true)

//  us = (us << 1) + (us >> 1); // x2.5 us, = 7 cycles
  us = (us << 2) + us ; // x2.5x2 us, = 7 cycles

                       // +1 cycle (register save)
  // user wants to wait longer than 6 us
  if (us > 30) // = 3 cycles
  {
    // since the loop is not accurately 2/5 of a microsecond we need
    // to multiply us by 0.9216 (11.0592 / 12)
    us = (us * 30199L) >> 16;   // x(0.9216/2) us = 29 cycles (30199 = 0.4608 x 0x10000L)

    // account for the time taken in the preceeding commands.
    // we just burned 53 (55) cycles above, remove 13, (13*4=52)
    us -= 13; // = 2 cycles
  }
  else
  {
    // account for the time taken in the preceeding commands.
    // we just burned 31 (33) cycles above, remove 8, (8*4=32)

              // 1 cycle when if jump here
    us >>= 1; // 2 cycles restore x2.5 us
    us -=  8; // 2 cycles
              // 2 cycles to jump back to delay cycle.
  }

#elif F_CPU >= 8000000L
  // for the 8 MHz clock
  __asm__ __volatile__ (
    "nop" "\n\t"
    "nop"); //just waiting 2 cycles

  // for a 1 and 2 microsecond delay, simply return.  the overhead
  // of the function call takes 16 (18) cycles, which is 2us
  if (us <= 2) return; // = 3 cycles, (4 when true)

  // the following loop takes 1/2 of a microsecond (4 cycles)
  // per iteration, so execute it twice for each microsecond of
  // delay requested.
  us <<= 1; //x2 us, = 2 cycles

  // account for the time taken in the preceeding commands.
  // we just burned 19 (21) cycles above, remove 5, (5*4=20)
  // us is at least 6 so we can substract 5
  us -= 5; // = 2 cycles

#elif F_CPU >= 7372800L
  __asm__ __volatile__ ("nop"); // just waiting 1 cycle

  // for a 1, 2 and 3 microsecond delay, simply return. the overhead
  // of the function call takes 15 (17) cycles, which is 2 us
  if (us <= 3) return; // = 3 cycles, (4 when true)

  us <<= 1; //x2 us, = 2 cycles

                       // +1 cycle (register save)
  // user wants to wait longer than 7 us
  if (us > 15) // = 3 cycles
  {
    // since the loop is not accurately 1/2 of a microsecond we need
    // to multiply us by 0.9216 (7.3728 / 8)
    us = (us * 60398L) >> 16;   // x0.9216 us = 29 cycles (60398 = 0.9216 x 0x10000L)

    // account for the time taken in the preceeding commands.
    // we just burned 52 (54) cycles above, remove 13, (13*4=52)
    // (50 cycles = 6.78 us) + (12 cycles = 1.63 us) = 8.41 us when 8 us wanted
    us -= 13; // = 2 cycles
  }
  else
  {
    // account for the time taken in the preceeding commands.
    // we just burned 26 (28) cycles above, remove 7, (7*4=28)

             // 1 cycle when if jump here
    us -= 7; // 2 cycles
             // 2 cycles to jump back to delay cycle.
  }

#elif F_CPU >= 4000000L
  __asm__ __volatile__ ("nop"); // just waiting 1 cycle
  // the overhead of the function call is 15 (17) cycles which is 4 us
  if (us <= 4) return; // = 3 cycles, (4 when true)

  // subtract microseconds that were wasted in this function
  us -= 4; // = 2 cycles

  // we don't need to multiply here because one request microsecond is exactly one loop cycle

#elif F_CPU >= 3686400L
  // for less than 7 microsecond delay, simply return. the overhead
  // of the function call takes 14 (16) cycles, which is almost 4 us
  if (us <= 6) return; // = 3 cycles, (4 when true)

                       // +1 cycle (register save)
  // user wants to wait longer than 12 us
  if (us > 12) // = 3 cycles
  {
    // since the loop is not accurately 1 microsecond we need
    // to multiply us by 0.9216 ( = 3.6864 / 4)
    us = (us * 60398L) >> 16;   // x0.9216 us = 29 cycles (60398 = 0.9216 x 0x10000L)

    // account for the time taken in the preceeding commands.
    // we just burned 47 (49) cycles above, remove 12, (12*4=48)
    us -= 12; // = 2 cycles
  }
  else
  {
    // account for the time taken in the preceeding commands.
    // we just burned 21 (23) cycles above, remove 6 microseconds

             // 1 cycle when if jump here
    us -= 6; // 2 cycles
             // 2 cycles to jump back to delay cycle.
  }

  // we don't need to multiply here because one request microsecond is almost one loop cycle

#elif F_CPU >= 2000000L
  // the overhead of the function call is 14 (16) cycles which is ~7 us
  // if someone request less than 11 us it will delay ~7 microseconds
  if (us <= 10) return;

  // Subtract microseconds (9) that were wasted in this function
  // so we can substract 9 ( every 2nd delay accurate 11 us and above)
  us -= 9;  // = 2 cycles

  us >>= 1; // division by 2 = 2 cycles

  __asm__ __volatile__ ("nop"); // waiting 1 cycle to adjust time
  
#elif F_CPU >= 1843200L
  // for less than 13 microsecond delay, simply return. the overhead
  // of the function call takes 14 (16) cycles, which is almost 8 us
  if (us <= 12) return; // = 3 cycles, (4 when true)

                       // no register save here
  // user wants to wait longer than 25 us
  if (us > 25) // = 3 cycles
  {
    // since the loop takes ~2.17 microseconds we need
    // to multiply us by 0.4608 ( = 1.8432 / 2 / 2 )
    us = (us * 30199L) >> 16;   // x(0.9216/2) us = 29 cycles (30199 = 0.4608 x 0x10000L)

    // account for the time taken in the preceeding commands.
    // we just burned 47 (49) cycles above, remove 24, microseconds
    us -= 24; // = 2 cycles
  }
  else
  {
    // account for the time taken in the preceeding commands.
    // we just burned 23 (25) cycles above, remove 12 microseconds

              // 1 cycle when if jump here
    us -= 12; // 2 cycles

    us >>= 1; // division by 2 = 2 cycles
	          // 2 cycles to jump back to delay cycle.
  }

#else
  // for the 1 MHz internal clock (default settings for most ATmega microcontrollers)

  // the overhead of the function calls is 14 (16) cycles
  if (us <= 16) return; //= 3 cycles, (4 when true)
  if (us <= 25) return; //= 3 cycles, (4 when true), (must be at least 25 if we want to substract 22)

  // compensate for the time taken by the preceeding and next commands (about 22 cycles)
  us -= 22; // = 2 cycles
  // the following loop takes 4 microseconds (4 cycles)
  // per iteration, so execute it us/4 times
  // us is at least 4, divided by 4 gives us 1 (no zero delay bug)
  us >>= 2; // us div 4, = 4 cycles


#endif

  // busy wait
  __asm__ __volatile__ (
    "1: sbiw %0,1" "\n\t"            // 2 cycles
        _MORENOP_                    // 4 cycles if 32 MHz or 1 cycle if 25 MHz
    "   brne 1b"                     // 2 cycles
    : /* no outputs */ 
    : "w" (us)
  );
  // return = 4 cycles
}
