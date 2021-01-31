/* delayus.h -- This file help using delayMicroseconds() two or more different clock speeds in one skech. */
 
/* Delay for the given number of microseconds.  Assumes a 1, 2, 3, 4, 5, 6, 8, 10, 12, 16, 20, 24, 25, 30, 32 MHz clock. */
/* In January 2021, delayMicroseconds() for LGT8Fx series MCUs by LaZsolt (github.com/LaZsolt/delayMicroseconds)         */

  // The us parameter looks like double type, but it only for constant case.
__inline__ void  delayMicroseconds  (double us)             __attribute__ ((always_inline, unused)); // for variable case

// AVR-GCC Inline Assembler Cookbook : https://www.nongnu.org/avr-libc/user-manual/inline_asm.html

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
