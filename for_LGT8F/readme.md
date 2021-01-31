# delayMicroseconds() for LGT8Fx MCUs (Tested, 31 january 2021)

This function is wasting the MCU time resource for the amount of time (in microseconds) specified by the parameter. (There are a thousand microseconds in a millisecond and a million microseconds in a second.) 
For delays longer than a few thousand microseconds, you should use delay() instead.
delayMicroseconds() can work at a lot of board frequencies. (32, 30, 25, 24, 20, 16, 12, 10, 8, 6, 5, 4, 3, 2, 1 MHz)

## Syntax

delayMicroseconds(us);
	
## Parameter

<b>us</b>: The number of microseconds to pause. Allowed data types: <b>unsigned int</b>. <br>
    Current version is designed for delaying from 1 to 16383 microseconds and is not designed to delay 0.
- Avoid zero delay.

## Returns

Nothing.

## Notes and Warnings

This function will produce an accurate delay in the range from 1 microsecond and up, but accuracy depends on board frequency and parameter type.
- When parmeter is a variable, at frequencies lower than 4 MHz, the accuracy is getting worse. Passing a register optimized variable to delayMicroseconds() function takes 1 clock cycles. This means when a non register optimized 16 variable passed, delay will be longer by 3 clockticks.
- When then parameter is constant the accuracy can be clock cycle precise at <b>any clock fequencies</b> down to 1 MHz. In this case the lowest possibile delay may 1/mcuMHz. On frequencies higher than 1 MHz the parameter can be lower than 1 microsecond. In this case the parameter does not look like an unsigned int type.
- delayMicroseconds() does not disable interrupts. For even more accuracy, it is recommended to disable interrupts during (pulses) timings.
- This function does not care about if the main clock frequency has changed in runtime by user program, but possibile to call a second copy of delayMicroseconds() which built for this second frequency. En example code will be created later.

## Example Code

The code configures pin number 8 to work as an output pin. It sends a train of pulses of <b>approximately</b> 100 microseconds period.
The approximation is due to execution of the other instructions in the code, like digitalWrite().

```
int outPin = 8;                // digital pin 8

void setup() {
  pinMode(outPin, OUTPUT);     // sets the digital pin as output
}

void loop() {
  digitalWrite(outPin, HIGH);  // sets the pin on
  delayMicroseconds(49.25);    // pauses for 49.25 microseconds
  digitalWrite(outPin, LOW);   // sets the pin off
  delayMicroseconds(50.75);    // pauses for 50.75 microseconds
}
```
