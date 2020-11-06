/* This structure designed for delayMicroseconds() to make sure compiler will not optimize by linker.
 * The idea came from Raymond Chen https://devblogs.microsoft.com/oldnewthing/20150102-00/?p=43233
 */
struct ForceFunctionToBeLinkedAndNotOptimized
{
  ForceFunctionToBeLinkedAndNotOptimized(void (* p) (unsigned int)) // This stuct will be stored, but never called.
    { GPIOR0=((uint16_t)p) & 0xFF; } // This is the shortest operation with the address in the LGT8Fx 8-bit AVR system without any affects.
};

ForceFunctionToBeLinkedAndNotOptimized ForceNotDoingSomething(delayMicroseconds_v);