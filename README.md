# delayMicroseconds
Code ideas and analysis for delayMicroseconds()

Under construction...

## How to accomplish delayMicroseconds() function

### - Calculating the cycle counter
   + <b><i>Multiplication</i></b> (Not multiplying more than 4 for the same delaying capability than the old 16 MHz Uno)
   + <b><i>Subtraction</i></b> (Decrease the cycle counter for compensate partially the wasted clock cycles before the waiting cycle)
   (or get a calculated counter from a table to avoid multiplication when frequency is not a round value in MHz, or MCU has no instruction for mulptilication)

### - <b><i>Tuning</i></b> with some nops
   + This is the rest of partially compensated wasted cycles (for the clock accurate timings. I hope this is comprehensible.)

### - Doing the waiting cycle
   + <b><i>variable nops</i></b> (for higher frequencies than 16 MHz)
   + <b><i>variable decrement</i></b> (for lower frequencies than 4 MHz)

It must be an assember source, because of LTO, compiler may generate shorter code than desired if it written in C.<br>
This function must be inline to avoid call and return timings.<br>
It does not check the zero delay, because if user calls it with a zero parameter, it is a user program error anyway. Impossible to delay zero time.<br>
When parameter not a constant, the function optimized for register variables. So when variable isn't a register optimized variable (like a volatile variable) the delay will be longer with 3 clock cycles.<br>
This function does not care about if the clock frequency has changed in runtime by user program.<br>
