On January 12, 2021, a new version of the `delayMicroseconds()` function arrived.

# delayMicroseconds() (tested)

This function is designed with maximum compatibility in mind, but it is not checking zero delay parameter. <br>
(The older version of Arduino `delayMicroseconds()` is checking zero delaying to avoid overflow caused incorrect delay.) <br>
When parameter is a variable, my version does not check the 0 delay to achieve more accurate timing. Anyway zero delay is impossible if MCU checking the parameter for 0. <br>
Tested with https://github.com/matthijskooijman/Arduino-delay-test

This version of `delayMicroseconds()` has more advantages. 
- It can operate on more different frequencies than before. 
- More accurate timing than before. 
- When using a compiler constant delay parameter, delay will be always CPU clock accurate at any frequencies, because of using `_delay_us()` function. 
- The maximum delay capability is same on 18, 20, 22, 24 and 32 MHz as on the standard 16 MHz. On 25 and 30 MHz the maximum delay capability decreased to 13107 microseconds.
- When the parameter is a variable, the assembly code is compiled. (Designed for register optimized variable.) 
- When the parameter is a variable, the assembly code accuracy is same as `_delay_us()` generated code from 4 MHz and upper, on the designed CPU clock frequencies. These frequencies: 32, 30, 25, 24, 22, 20, 18, 16, 15, 14, 12, 11, 10, 9, 8, 7, 6, 5 and 4 MHz 
- With the assembly code, it can avoid the effect of optimization and link time optimization (LTO) 
- On lower frequencies ( 2, 1 and 0.5 MHz ), with a variable parameter, the accuracy is decreased, but as close as possibile to the requested delay, even on low values. 
- This version is useful even CPU clock has fractional MHz. 
- Delay accuracy on other frequencies: 
  + On  1.8432 MHz, delay is approximately 8.65% longer than requested, 
  + On  3.6864 MHz, delay is approximately 8.51% longer,
  + On  7.3728 MHz, delay is approximately 5.05% shorter, 
  + On  9.2160 MHz, delay is approximately 2.34% shorter, 
  + On  9.5000 MHz, delay is approximately 5.26% shorter, 
  + On 11.0592 MHz, delay is approximately 0.53% shorter,
  + On 12.8000 MHz, delay is approximately 6.25% shorter,
  + On 14.7456 MHz, delay is approximately 1.73% longer, 
  + On 16.5000 MHz, delay is approximately 3.03% shorter, 
  + On 18.4320 MHz, delay is approximately 2.34% shorter, 
  + On 22.1184 MHz, delay is approximately 0.54% shorter, 
  + On 29.4912 MHz, delay is approximately 1,73% longer than requested. 
