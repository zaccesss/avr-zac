/*
 * Fuse_Test.c
 *
 * Created: 29/04/2026 10:04:40
 * Author : reevesrp
 */
#define F_CPU 20E6
#include <avr/io.h>
#include <util/delay.h>

#define Test_Pin PB1
#define Loop_Delay 500

int main(void)
{
    DDRB = 1<<Test_Pin;				// Test pin now an output
    while (1)
    {
		PORTB |= 1<<Test_Pin;		// TP high
		_delay_ms(Loop_Delay);
		PORTB &= ~(1<<Test_Pin);	// TP low
		_delay_ms(Loop_Delay);
    }
}
