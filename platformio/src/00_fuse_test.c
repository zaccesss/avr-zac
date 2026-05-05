/*
 * Fuse_Test.c
 *
 * Created: 29/04/2026 10:04:40
 * Author : reevesrp
 */
#define F_CPU 20E6              // 20 MHz external crystal
#include <avr/io.h>             // AVR input/output register definitions
#include <util/delay.h>         // AVR delay functions

#define Test_Pin PB1            // LED on Port B pin 1
#define Loop_Delay 500          // Delay in milliseconds

int main(void)
{
    DDRB = 1<<Test_Pin;         // Set Test_Pin as output
    while (1)
    {
        PORTB |= 1<<Test_Pin;       // LED on
        _delay_ms(Loop_Delay);      // Wait Loop_Delay ms
        PORTB &= ~(1<<Test_Pin);    // LED off
        _delay_ms(Loop_Delay);      // Wait Loop_Delay ms
    }
}
