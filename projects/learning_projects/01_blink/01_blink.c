/*
 * Test_1.c
 *
 * created: 30/04/2026 04:01:36
 * author : zac
 */

#define F_CPU 20E6              // 20 MHz external crystal
#include <avr/io.h>             // AVR input/output register definitions
#include <util/delay.h>         // AVR delay functions

#define Test_Pin PB0            // LED connected to Port B, pin 0
#define Loop_Delay 1000         // delay in milliseconds

int main(void)
{
	DDRB = 1<<Test_Pin;         // set Test_Pin as output

	while (1)
	{
		// first blink
		PORTB |= 1<<Test_Pin;       // LED on
		_delay_ms(2000);             // wait 2000 ms
		PORTB &= ~(1<<Test_Pin);    // LED off
		_delay_ms(1000);             // wait 1000 ms

		// second blink
		//PORTB |= 1<<Test_Pin;       // LED on
		//_delay_ms(100);             // wait 100 ms
		//PORTB &= ~(1<<Test_Pin);    // LED off
		//_delay_ms(500);             // pause 500 ms before repeating
	}
}
