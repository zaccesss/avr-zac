/*
 * Test_4.c
 *
 * created: 05/05/2026 13:28:57
 * author : zac
 */ 

#define F_CPU 20E6              // 20 MHz external crystal
#include <avr/io.h>             // AVR input/output register definitions
#include <util/delay.h>         // AVR delay functions
#include <avr/interrupt.h>      // AVR interrupt definitions

// LED pins on PORTB
#define LED_RED    PB0          // red LED
#define LED_YELLOW PB1          // yellow LED
#define LED_WHITE  PB2          // white LED
#define LED_GREEN  PB3          // green LED
#define LED_BLUE   PB4          // blue LED
#define BUZZER     PD3          // active buzzer on PORTD
#define BUTTON     PD2          // push button input (INT0 pin)

int main(void)
{
    // set LEDs as outputs on PORTB
    DDRB |= (1<<LED_RED) | (1<<LED_YELLOW) | (1<<LED_WHITE) | (1<<LED_GREEN) | (1<<LED_BLUE);

    // set buzzer as output on PORTD
    DDRD |= (1<<BUZZER);

    // set button as input on PORTD
    DDRD &= ~(1<<BUTTON);

    // configure INT0 (PD2) to trigger on any logic change
    EICRA |= (1<<ISC00);        // trigger on any change of INT0
    EIMSK |= (1<<INT0);         // enable INT0 interrupt

    // enable global interrupts
    sei();

    while (1)
    {
        // LEDs cycle continuously regardless of button
        PORTB |= 1<<LED_RED;            // red on
        _delay_ms(500);                 // wait 500 ms
        PORTB &= ~(1<<LED_RED);         // red off

        PORTB |= 1<<LED_YELLOW;         // yellow on
        _delay_ms(500);                 // wait 500 ms
        PORTB &= ~(1<<LED_YELLOW);      // yellow off

        PORTB |= 1<<LED_WHITE;          // white on
        _delay_ms(500);                 // wait 500 ms
        PORTB &= ~(1<<LED_WHITE);       // white off

        PORTB |= 1<<LED_GREEN;          // green on
        _delay_ms(500);                 // wait 500 ms
        PORTB &= ~(1<<LED_GREEN);       // green off

        PORTB |= 1<<LED_BLUE;           // blue on
        _delay_ms(500);                 // wait 500 ms
        PORTB &= ~(1<<LED_BLUE);        // blue off
    }
}

// interrupt Service Routine for INT0 (PD2 button)
ISR(INT0_vect)
{
    if (PIND & (1<<BUTTON))             // if button is pressed (HIGH)
        PORTD |= (1<<BUZZER);           // buzzer on
    else
        PORTD &= ~(1<<BUZZER);          // buzzer off
}
