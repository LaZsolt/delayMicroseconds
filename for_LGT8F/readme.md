# delayMicroseconds() for LGT8Fx MCUs (Tested)

Wasting the MCU time resource for the amount of time (in microseconds) specified by the parameter. (There are a thousand microseconds in a millisecond and a million microseconds in a second.) 
For delays longer than a few thousand microseconds, you should use delay() instead.
delayMicroseconds() could work a lot of board frequencies. (32, 25, 24, 20, 16, 12, 8, 4, 2 MHz)

## Syntax

delayMicroseconds(us);
	
## Parameter

<b>us</b>: The number of microseconds to pause. Allowed data types: unsigned int. <br>
    Current version designed for delaying 1 to 16383 microseconds and not designed to delay 0.
- Avoid zero delay.

## Returns

Nothing.

## Notes and Warnings

This function will produce an accurate delay in the range 1 microseconds and up, but accuracy depends on board frequency and parameter type.
- When parmeter is a variable at lower frequencies accuracy getting worse. When using LGT8Fx MCU the lowest frequency for accurate 1 us delay is 12 MHz and the lowest frequency for accurate 2 us delay is 8 MHz. However a variable passing to delayMicroseconds() funcion takes 1 to 4 clock cycles. This mean -1 and +2 clockticks difference compared to the exact delay.
- When parameter is a constant the accuracy can be clock tight precise at <b>any clock fequencies</b> down to 1 MHz. In this case the lowest possibile delay may 1/mcuMHz. The parameter can be lower than 1 microsecond. In this case the parameter not look like an unsigned int type.
- delayMicroseconds() not disables interrupts. For even more accuracy, recommended to disable interrupts during pulse timings.

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
