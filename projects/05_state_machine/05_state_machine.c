/*
 * Test_5.c
 *
 * Created: 05/05/2026 13:34:08
 * Author : zac
 */

#define F_CPU 20E6              // 20 MHz external crystal
#include <avr/io.h>             // AVR input/output register definitions
#include <util/delay.h>         // AVR delay functions
#include <avr/interrupt.h>      // AVR interrupt definitions

// LED pins on PORTB
#define LED_RED    PB0          // Red LED
#define LED_YELLOW PB1          // Yellow LED
#define LED_WHITE  PB2          // White LED
#define LED_GREEN  PB3          // Green LED
#define LED_BLUE   PB4          // Blue LED

// Buzzer and button on PORTD
#define BUZZER     PD3          // Active buzzer
#define BUTTON     PD2          // Push button (INT0)

// All LEDs mask for convenience
#define ALL_LEDS ((1<<LED_RED)|(1<<LED_YELLOW)|(1<<LED_WHITE)|(1<<LED_GREEN)|(1<<LED_BLUE))

// Enum defining all LED modes
typedef enum {
    MODE_CHASE,                 // Mode 0: LEDs chase one by one
    MODE_BLINK_ALL,             // Mode 1: All LEDs blink together
    MODE_ALTERNATE,             // Mode 2: Alternating odd/even LEDs
    MODE_PWM_FADE,              // Mode 3: PWM fade in and out
    MODE_COUNT                  // Total number of modes (always last)
} LED_Mode;

// Global volatile variables shared between ISR and main
volatile LED_Mode current_mode = MODE_CHASE;    // Current LED mode

// Function prototypes
void pwm_fade(void);
void beep(void);
void debounce_delay(void);

int main(void)
{
    // Set LEDs as outputs on PORTB
    DDRB |= ALL_LEDS;

    // Set buzzer as output and button as input on PORTD
    DDRD |= (1<<BUZZER);
    DDRD &= ~(1<<BUTTON);

    // Configure INT0 (PD2) to trigger on falling edge (button press)
    EICRA |= (1<<ISC01);        // Trigger on falling edge of INT0
    EICRA &= ~(1<<ISC00);       // Clear ISC00
    EIMSK |= (1<<INT0);         // Enable INT0 interrupt

    // Enable global interrupts
    sei();

    while (1)
    {
        switch (current_mode)
        {
            case MODE_CHASE:
                // LEDs chase one by one
                PORTB = (1<<LED_RED);                                   // Red on
                _delay_ms(300);
                PORTB = (1<<LED_YELLOW);                                // Yellow on
                _delay_ms(300);
                PORTB = (1<<LED_WHITE);                                 // White on
                _delay_ms(300);
                PORTB = (1<<LED_GREEN);                                 // Green on
                _delay_ms(300);
                PORTB = (1<<LED_BLUE);                                  // Blue on
                _delay_ms(300);
                PORTB = 0x00;                                           // All off
                _delay_ms(300);
                break;

            case MODE_BLINK_ALL:
                // All LEDs blink together
                PORTB = ALL_LEDS;                                       // All on
                _delay_ms(400);
                PORTB = 0x00;                                           // All off
                _delay_ms(400);
                break;

            case MODE_ALTERNATE:
                // Alternate odd and even LEDs
                PORTB = (1<<LED_RED)|(1<<LED_WHITE)|(1<<LED_BLUE);     // Odd on
                _delay_ms(400);
                PORTB = (1<<LED_YELLOW)|(1<<LED_GREEN);                 // Even on
                _delay_ms(400);
                break;

            case MODE_PWM_FADE:
                // Smooth fade in and out
                pwm_fade();
                break;

            default:
                current_mode = MODE_CHASE;                              // Reset to chase if unknown
                break;
        }
    }
}

// Interrupt Service Routine for INT0 (PD2 button press)
ISR(INT0_vect)
{
    debounce_delay();                                                   // Debounce the button

    if (!(PIND & (1<<BUTTON)))                                          // Confirm button still pressed
    {
        current_mode = (current_mode + 1) % MODE_COUNT;                // Cycle to next mode
        PORTB = 0x00;                                                   // Clear LEDs on mode change
        beep();                                                         // Beep to confirm mode change
    }
}

// Short beep on the buzzer to confirm button press
void beep(void)
{
    PORTD |= (1<<BUZZER);       // Buzzer on
    _delay_ms(80);              // Short beep duration
    PORTD &= ~(1<<BUZZER);      // Buzzer off
}

// Software debounce delay
void debounce_delay(void)
{
    _delay_ms(20);              // Wait 20 ms for button to settle
}

// Software PWM fade on all LEDs
void pwm_fade(void)
{
    uint8_t i;
    uint8_t j;

    // Fade in
    for (i = 0; i < 255; i++)
    {
        for (j = 0; j < 50; j++)       // Repeat each brightness level 50 times
        {
            PORTB = ALL_LEDS;           // All LEDs on
            _delay_us(1);               // Fixed on time
        }
        PORTB = 0x00;                   // All LEDs off
        _delay_ms(1);                   // Fixed off time
    }

    // Fade out
    for (i = 255; i > 0; i--)
    {
        for (j = 0; j < 50; j++)       // Repeat each brightness level 50 times
        {
            PORTB = ALL_LEDS;           // All LEDs on
            _delay_us(1);               // Fixed on time
        }
        PORTB = 0x00;                   // All LEDs off
        _delay_ms(1);                   // Fixed off time
    }
}
