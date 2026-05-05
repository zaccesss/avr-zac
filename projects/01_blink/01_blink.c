/*
 * Test_1.c
 *
 * Created: 30/04/2026 04:01:36
 * Author : zac
 */

#define F_CPU 20E6              // 20 MHz external crystal
#include <avr/io.h>             // AVR input/output register definitions
#include <util/delay.h>         // AVR delay functions

#define Test_Pin PB0            // LED connected to Port B, pin 0
#define Loop_Delay 1000         // Delay in milliseconds

int main(void)
{
	DDRB = 1<<Test_Pin;         // Set Test_Pin as output

	while (1)
	{
		// First blink
		PORTB |= 1<<Test_Pin;       // LED on
		_delay_ms(2000);             // Wait 100 ms
		PORTB &= ~(1<<Test_Pin);    // LED off
		_delay_ms(1000);             // Wait 100 ms

		// Second blink
		//PORTB |= 1<<Test_Pin;       // LED on
		//_delay_ms(100);             // Wait 100 ms
		//PORTB &= ~(1<<Test_Pin);    // LED off
		//_delay_ms(500);             // Pause 500 ms before repeating
	}
}
