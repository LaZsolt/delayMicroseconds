/* Delay for the given number of microseconds.  Assumes a 1, 2, 3, 4, 5, 6, 8, 10, 12, 16, 20, 24, 25, 30, 32 MHz clock. */
/* In January 2021, delayMicroseconds() for LGT8Fx series MCUs by LaZsolt (github.com/LaZsolt/delayMicroseconds)         */

  // The us parameter looks like double type, but it only for constant case.
__inline__ void  delayMicroseconds  (double us)             __attribute__ ((always_inline, unused)); // for variable case
__inline__ void _lgt8fx_delay_cycles(const uint32_t cticks) __attribute__ ((always_inline, unused)); // for constant case

// AVR-GCC Inline Assembler Cookbook : https://www.nongnu.org/avr-libc/user-manual/inline_asm.html

__inline__ void _lgt8fx_delay_cycles(const uint32_t cticks)
{
  uint32_t cticks_left = cticks;

  // loopcounter = 1               -> 7  clockticks    ( clockticks used = 3 + (loopcounter) * 5 - 1 )
  // loopcounter = 2               -> 12 clockticks
  // loopcounter = 16777215        -> 67108862 clocktics
  if (( cticks_left >= 262146L ) && ( cticks_left <= 83886077L ))
    {
      uint32_t lcount;
      lcount  = ( (cticks_left - 2) / 5);
      __builtin_avr_delay_cycles(lcount * 5 + 2);
      cticks_left -= ( lcount * 5 ) + 2;
    }

  // loopcounter = 1                 -> 4 clocktics    ( clockticks used = 2 + loopcounter * 3 - 1 )
  // loopcounter = 2                 -> 7 clocktics
  // loopcounter = 65535             -> 196606 clocktics
  if (( cticks_left >= 196607L ) && ( cticks_left <= 262145L ))
    {
      uint32_t lcount;
//      lcount = ((cticks_left - 196606L) / 3);  Avoid builtin_avr_delay_cycles value became lower than 768 here.
      lcount = ((cticks_left - 195826L) / 3);
      __builtin_avr_delay_cycles(lcount * 4 + 1);
      cticks_left -= lcount * 3 + 1;
    }
  if (( cticks_left >= 768L ) && ( cticks_left <= 196606L ))
    {
      uint32_t lcount;
      lcount = ((cticks_left - 1) / 3);
      __builtin_avr_delay_cycles(lcount * 4 + 1);
      cticks_left -= lcount * 3 + 1;
    }

  // loopcounter = 1                 -> 3 clocktics    ( clockticks used = 1 + loopcounter * 3 - 1 )
  // loopcounter = 2                 -> 6 clocktics
  // loopcounter = 255               -> 765 clocktics
  if (( cticks_left >= 6L ) && ( cticks_left <= 767L ))
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

#ifndef _NOP8_
  #define _NOP0_ ""
  #define _NOP1_ "nop      \n\t"
  #define _NOP2_ "nop      \n\t nop      \n\t"
  #define _NOP3_ "brcc .+0 \n\t brcs .+0 \n\t"
  #define _NOP4_ "brcc .+0 \n\t brcs .+0 \n\t nop      \n\t"
  #define _NOP5_ "brcc .+0 \n\t brcs .+0 \n\t nop      \n\t nop      \n\t"
  #define _NOP6_ "brcc .+0 \n\t brcs .+0 \n\t brcc .+0 \n\t brcs .+0 \n\t"
  #define _NOP7_ "brcc .+0 \n\t brcs .+0 \n\t brcc .+0 \n\t brcs .+0 \n\t nop \n\t"
  #define _NOP8_ "brcc .+0 \n\t brcs .+0 \n\t brcc .+0 \n\t brcs .+0 \n\t nop \n\t nop      \n\t"
#endif

// Multiply by two takes 2 clock cycles
#define _MULby2_                     \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" 

// Multiply by three takes 4 clock cycles
#define _MULby3_                     \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" \
          "add    %A0,%A1      \n\t" \
          "adc    %B0,%B1      \n\t"

// Multiply by four takes 4 clock cycles
#define _MULby4_                     \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" 

// Multiply by five takes 6 clock cycles
#define _MULby5_                     \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" \
          "add    %A0,%A0      \n\t" \
          "adc    %B0,%B0      \n\t" \
          "add    %A0,%A1      \n\t" \
          "adc    %B0,%B1      \n\t"
    
#if   F_CPU >= 32000000L

  #define _CCALCULATE_   _MULby4_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP3_
  #define _NOPSinCYCLE_  _NOP5_

//lif F_CPU >= 30000000L
#elif F_CPU >= 29400000L

  #define _CCALCULATE_   _MULby5_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_   _NOP5_
  #define _NOPSinCYCLE_  _NOP3_

// Ethernet crystal frequency
#elif F_CPU >= 25000000L

  #define _CCALCULATE_   _MULby5_    \
         "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_   _NOP3_
  #define _NOPSinCYCLE_  _NOP2_

#elif F_CPU >= 24000000L

  #define _CCALCULATE_   _MULby4_    \
         "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP1_
  #define _NOPSinCYCLE_  _NOP3_

#elif F_CPU >= 22000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP8_
  #define _NOPSinCYCLE_  _NOP8_

/* 3 * 7 MHz
#elif F_CPU >= 21000000L

  #define _CCALCULATE_   _MULby3_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP2_
  #define _NOPSinCYCLE_  _NOP4_
*/

#elif F_CPU >= 20000000L

  #define _CCALCULATE_   _MULby4_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP0_
  #define _NOPSinCYCLE_  _NOP2_

#elif F_CPU >= 18000000L

  #define _CCALCULATE_  _MULby3_     \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP1_
  #define _NOPSinCYCLE_ _NOP3_

#elif F_CPU >= 16000000L

  #define _CCALCULATE_   _MULby4_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_  _NOP3_
  #define _NOPSinCYCLE_ _NOP1_

//lif F_CPU >= 15000000L
#elif F_CPU >= 14700000L

  #define _CCALCULATE_   _MULby3_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP2_

#elif F_CPU >= 14000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP4_
  #define _NOPSinCYCLE_ _NOP4_

// 12 MHz resonator on Wavgat board or for USB communication
#elif F_CPU >= 12000000L

  #define _CCALCULATE_   _MULby4_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_  _NOP1_
  #define _NOPSinCYCLE_ _NOP0_

#elif F_CPU >= 11000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP8_

#elif F_CPU >= 10000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP2_
  #define _NOPSinCYCLE_ _NOP2_

#elif F_CPU >= 9000000L

  #define _CCALCULATE_  _MULby3_     \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_  _NOP1_
  #define _NOPSinCYCLE_ _NOP0_

#elif F_CPU >= 8000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP1_
  #define _NOPSinCYCLE_ _NOP1_

#elif F_CPU >= 7000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP4_

// 6 MHz for 12 MHz resonator divided by 2 on Wavgat board
#elif F_CPU >= 6000000L

  #define _CCALCULATE_  _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

#elif F_CPU >= 5000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP2_

//lif F_CPU >= 4000000L
#elif F_CPU >= 3660000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

// 3 MHz for 12 MHz resonator divided by 4 on Wavgat board
#elif   F_CPU >= 3000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

//#elif F_CPU >= 2000000L
#elif   F_CPU >= 1800000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

// 1.5 MHz for 12 MHz resonator divided by 8 on Wavgat board
#elif   F_CPU >= 1500000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

#elif   F_CPU >= 1000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

// 0.75 MHz for 12 MHz resonator divided by 16 on Wavgat board
#elif   F_CPU >= 750000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

#elif   F_CPU >= 500000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

#else

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

#endif

#if     F_CPU >= 3000000L
  #define _SBnBR_                    \
          "sbiw   %A0,1        \n\t" \
          "brne   "
//#elif F_CPU >= 2000000L
//#elif F_CPU >= 1800000L
#elif   F_CPU >= 1500000L
  #define _SBnBR_                    \
          "sbiw   %A0,2        \n\t" \
          "brcc   "
// 0.75 MHz for 12 MHz resonator divided by 16 on Wavgat board and 1 MHz on a normal board
#elif   F_CPU >= 750000L
  #define _SBnBR_                    \
          "sbiw   %A0,4        \n\t" \
          "brcc   "
#else
  #define _SBnBR_                    \
          "sbiw   %A0,8        \n\t" \
          "brcc   "
#endif

__inline__ void delayMicroseconds(double us)
{
    if ( __builtin_constant_p(us)) 
    { 
        _lgt8fx_delay_cycles((uint32_t)(F_CPU/1000000.0)*((double)us));
        return;
    }

    uint16_t tmp0;
    uint16_t tmp1 = us;
    __asm__ volatile(
                // Movw compensates for the missing cycle in
                // the last brne below, and also prevents the
                // compiler from inserting this one. Both operands
                // might actually be the same register, but that's
                // ok.
      "movw   %A0, %A1     \n\t"  // 1 ( tmp = us; )
      _CCALCULATE_                // Cycle counter multiplication and subtraction if needed
      _TUNINGNOPS_
      "1:                  \n\t"
      _NOPSinCYCLE_
      _SBnBR_ "1b          \n\t"  // 4 (3 on last)
      : "=w" (tmp0)
      : "r"  (tmp1)
    );
}

#undef _CCALCULATE_
#undef _TUNINGNOPS_
#undef _NOPSinCYCLE_
#undef _SBnBR_
