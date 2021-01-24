/* Delay for the given number of microseconds.  Assumes a 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 18, 20, 22, 24, 25, 30, 32 MHz clock. */
/* delayMicroseconds() has been modified by LaZsolt (github.com/LaZsolt/delayMicroseconds) in January 2021 */

static inline void delayMicroseconds(uint16_t us) __attribute__((always_inline, unused));

#define _NOP0_ ""
#define _NOP1_ "nop \n\t"
#define _NOP2_ "rjmp .+0 \n\t"
#define _NOP3_ "rjmp .+0 \n\t nop      \n\t"
#define _NOP4_ "rjmp .+0 \n\t rjmp .+0 \n\t"
#define _NOP5_ "rjmp .+0 \n\t rjmp .+0 \n\t nop      \n\t"
#define _NOP6_ "rjmp .+0 \n\t rjmp .+0 \n\t rjmp .+0 \n\t"
#define _NOP7_ "rjmp .+0 \n\t rjmp .+0 \n\t rjmp .+0 \n\t nop \n\t"

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
  #define _TUNINGNOPS_   _NOP2_
  #define _NOPSinCYCLE_  _NOP4_

//lif F_CPU >= 30000000L
#elif F_CPU >= 29400000L

  #define _CCALCULATE_   _MULby5_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_   _NOP4_
  #define _NOPSinCYCLE_  _NOP2_

#elif F_CPU >= 25000000L

  #define _CCALCULATE_   _MULby5_    \
         "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_   _NOP2_
  #define _NOPSinCYCLE_  _NOP1_

#elif F_CPU >= 24000000L

  #define _CCALCULATE_   _MULby4_    \
         "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP0_
  #define _NOPSinCYCLE_  _NOP2_

#elif F_CPU >= 22000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP7_
  #define _NOPSinCYCLE_  _NOP7_

/*    3 * 7 MHz
#elif F_CPU >= 21000000L

  #define _CCALCULATE_   _MULby3_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_   _NOP1_
  #define _NOPSinCYCLE_  _NOP3_
*/

#elif F_CPU >= 20000000L

  #define _CCALCULATE_   _MULby4_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_   _NOP4_
  #define _NOPSinCYCLE_  _NOP1_

#elif F_CPU >= 18000000L

  #define _CCALCULATE_  _MULby3_     \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP2_

#elif F_CPU >= 16000000L

  #define _CCALCULATE_   _MULby4_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_  _NOP2_
  #define _NOPSinCYCLE_ _NOP0_

//lif F_CPU >= 15000000L
#elif F_CPU >= 14700000L

  #define _CCALCULATE_   _MULby3_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_  _NOP4_
  #define _NOPSinCYCLE_ _NOP1_

#elif F_CPU >= 14000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP3_
  #define _NOPSinCYCLE_ _NOP3_

#elif F_CPU >= 12000000L

  #define _CCALCULATE_   _MULby3_    \
          "sbiw   %A0,2        \n\t"
  #define _TUNINGNOPS_  _NOP2_
  #define _NOPSinCYCLE_ _NOP0_

#elif F_CPU >= 11000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP7_

#elif F_CPU >= 10000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP1_
  #define _NOPSinCYCLE_ _NOP1_

#elif F_CPU >= 9000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP5_

#elif F_CPU >= 8000000L

  #define _CCALCULATE_   _MULby2_    \
          "sbiw   %A0,1        \n\t"
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

#elif F_CPU >= 7000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP3_

#elif F_CPU >= 6000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP2_

#elif F_CPU >= 5000000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP1_

//lif F_CPU >= 4000000L
#elif F_CPU >= 3660000L

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

#else // 2 MHz and below

  #define _CCALCULATE_  ""
  #define _TUNINGNOPS_  _NOP0_
  #define _NOPSinCYCLE_ _NOP0_

#endif

//if  F_CPU >= 4000000L
#if    F_CPU >= 3660000L
  #define _SBnBR_                    \
          "sbiw   %A0,1        \n\t" \
          "brne   "
//lif F_CPU >= 2000000L
#elif F_CPU >= 1800000L
  #define _SBnBR_                    \
          "sbiw   %A0,2        \n\t" \
          "brcc   "
#elif F_CPU >= 1000000L
  #define _SBnBR_                    \
          "sbiw   %A0,4        \n\t" \
          "brcc   "
#else
  #define _SBnBR_                    \
          "sbiw   %A0,8        \n\t" \
          "brcc   "
#endif

static inline void delayMicroseconds(uint16_t us)
{
    if (__builtin_constant_p(us)) {
      _delay_us(us);
    } else {
        uint16_t tmp;
        asm volatile(
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
          : "=w" (tmp)
          : "r"  (us)
        );
    }
}
