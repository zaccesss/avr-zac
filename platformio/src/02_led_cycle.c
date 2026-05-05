/*
 * Test_2.c
 *
 * Created: 30/04/2026 05:30:27
 * Author : zac
 */

#define F_CPU 20E6              // 20 MHz external crystal
#include <avr/io.h>             // AVR input/output register definitions
#include <util/delay.h>         // AVR delay functions

// LED pins on PORTB
#define LED_RED    PB0          // Red LED
#define LED_YELLOW PB1          // Yellow LED
#define LED_WHITE  PB2          // White LED
#define LED_GREEN  PB3          // Green LED
#define LED_BLUE   PB4          // Blue LED
#define BUZZER     PB5          // Active buzzer

// Button pin on PORTD
#define BUTTON     PD2          // Push button input

int main(void)
{
    // Set LEDs and buzzer as outputs on PORTB
    DDRB |= (1<<LED_RED) | (1<<LED_YELLOW) | (1<<LED_WHITE) | (1<<LED_GREEN) | (1<<LED_BLUE) | (1<<BUZZER);

    // Set button as input on PORTD
    DDRD &= ~(1<<BUTTON);

    while (1)
    {
        PORTB |= (1<<LED_RED);      // Red on
        _delay_ms(500);
        PORTB &= ~(1<<LED_RED);     // Red off

        PORTB |= (1<<LED_YELLOW);   // Yellow on
        _delay_ms(500);
        PORTB &= ~(1<<LED_YELLOW);  // Yellow off

        PORTB |= (1<<LED_WHITE);    // White on
        _delay_ms(500);
        PORTB &= ~(1<<LED_WHITE);   // White off

        PORTB |= (1<<LED_GREEN);    // Green on
        _delay_ms(500);
        PORTB &= ~(1<<LED_GREEN);   // Green off

        PORTB |= (1<<LED_BLUE);     // Blue on
        _delay_ms(500);
        PORTB &= ~(1<<LED_BLUE);    // Blue off
    }
}
