/*
 * Test_2.c
 *
 * created: 30/04/2026 05:30:27
 * author : zac
 */

#define F_CPU 20E6              // 20 MHz external crystal
#include <avr/io.h>             // AVR input/output register definitions
#include <util/delay.h>         // AVR delay functions

// LED pins on PORTB
#define LED_RED    PB0          // red LED
#define LED_YELLOW PB1          // yellow LED
#define LED_WHITE  PB2          // white LED
#define LED_GREEN  PB3          // green LED
#define LED_BLUE   PB4          // blue LED
#define BUZZER     PB5          // active buzzer

// button pin on PORTD
#define BUTTON     PD2          // push button input

int main(void)
{
    // set LEDs and buzzer as outputs on PORTB
    DDRB |= (1<<LED_RED) | (1<<LED_YELLOW) | (1<<LED_WHITE) | (1<<LED_GREEN) | (1<<LED_BLUE) | (1<<BUZZER);

    // set button as input on PORTD
    DDRD &= ~(1<<BUTTON);

    while (1)
    {
        PORTB |= (1<<LED_RED);      // red on
        _delay_ms(500);
        PORTB &= ~(1<<LED_RED);     // red off

        PORTB |= (1<<LED_YELLOW);   // yellow on
        _delay_ms(500);
        PORTB &= ~(1<<LED_YELLOW);  // yellow off

        PORTB |= (1<<LED_WHITE);    // white on
        _delay_ms(500);
        PORTB &= ~(1<<LED_WHITE);   // white off

        PORTB |= (1<<LED_GREEN);    // green on
        _delay_ms(500);
        PORTB &= ~(1<<LED_GREEN);   // green off

        PORTB |= (1<<LED_BLUE);     // blue on
        _delay_ms(500);
        PORTB &= ~(1<<LED_BLUE);    // blue off
    }
}
