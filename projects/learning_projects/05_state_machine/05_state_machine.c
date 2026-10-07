/*
 * Test_5.c
 *
 * created: 05/05/2026 13:34:08
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

// buzzer and button on PORTD
#define BUZZER     PD3          // active buzzer
#define BUTTON     PD2          // push button (INT0)

// all LEDs mask for convenience
#define ALL_LEDS ((1<<LED_RED)|(1<<LED_YELLOW)|(1<<LED_WHITE)|(1<<LED_GREEN)|(1<<LED_BLUE))

// enum defining all LED modes
typedef enum {
    MODE_CHASE,                 // mode 0: LEDs chase one by one
    MODE_BLINK_ALL,             // mode 1: All LEDs blink together
    MODE_ALTERNATE,             // mode 2: Alternating odd/even LEDs
    MODE_PWM_FADE,              // mode 3: PWM fade in and out
    MODE_COUNT                  // total number of modes (always last)
} LED_Mode;

// global volatile variables shared between ISR and main
volatile LED_Mode current_mode = MODE_CHASE;    // current LED mode

// function prototypes
void pwm_fade(void);
void beep(void);
void debounce_delay(void);

int main(void)
{
    // set LEDs as outputs on PORTB
    DDRB |= ALL_LEDS;

    // set buzzer as output and button as input on PORTD
    DDRD |= (1<<BUZZER);
    DDRD &= ~(1<<BUTTON);

    // configure INT0 (PD2) to trigger on falling edge (button press)
    EICRA |= (1<<ISC01);        // trigger on falling edge of INT0
    EICRA &= ~(1<<ISC00);       // clear ISC00
    EIMSK |= (1<<INT0);         // enable INT0 interrupt

    // enable global interrupts
    sei();

    while (1)
    {
        switch (current_mode)
        {
            case MODE_CHASE:
                // LEDs chase one by one
                PORTB = (1<<LED_RED);                                   // red on
                _delay_ms(300);
                PORTB = (1<<LED_YELLOW);                                // yellow on
                _delay_ms(300);
                PORTB = (1<<LED_WHITE);                                 // white on
                _delay_ms(300);
                PORTB = (1<<LED_GREEN);                                 // green on
                _delay_ms(300);
                PORTB = (1<<LED_BLUE);                                  // blue on
                _delay_ms(300);
                PORTB = 0x00;                                           // all off
                _delay_ms(300);
                break;

            case MODE_BLINK_ALL:
                // all LEDs blink together
                PORTB = ALL_LEDS;                                       // all on
                _delay_ms(400);
                PORTB = 0x00;                                           // all off
                _delay_ms(400);
                break;

            case MODE_ALTERNATE:
                // alternate odd and even LEDs
                PORTB = (1<<LED_RED)|(1<<LED_WHITE)|(1<<LED_BLUE);     // odd on
                _delay_ms(400);
                PORTB = (1<<LED_YELLOW)|(1<<LED_GREEN);                 // even on
                _delay_ms(400);
                break;

            case MODE_PWM_FADE:
                // smooth fade in and out
                pwm_fade();
                break;

            default:
                current_mode = MODE_CHASE;                              // reset to chase if unknown
                break;
        }
    }
}

// interrupt Service Routine for INT0 (PD2 button press)
ISR(INT0_vect)
{
    debounce_delay();                                                   // debounce the button

    if (!(PIND & (1<<BUTTON)))                                          // confirm button still pressed
    {
        current_mode = (current_mode + 1) % MODE_COUNT;                // cycle to next mode
        PORTB = 0x00;                                                   // clear LEDs on mode change
        beep();                                                         // beep to confirm mode change
    }
}

// short beep on the buzzer to confirm button press
void beep(void)
{
    PORTD |= (1<<BUZZER);       // buzzer on
    _delay_ms(80);              // short beep duration
    PORTD &= ~(1<<BUZZER);      // buzzer off
}

// software debounce delay
void debounce_delay(void)
{
    _delay_ms(20);              // wait 20 ms for button to settle
}

// software PWM fade on all LEDs. Each frame is 256 steps long and the LEDs stay on for the
// first `duty` steps, so a frame lasts about 0.7 ms (roughly 1.4 kHz, too fast to see flicker)
// and only the on/off ratio changes. Ramping the duty from 0 to 255 and back fades the LEDs up
// and down, about 0.7 s each way.
#define FADE_FRAMES_PER_LEVEL 4     // frames shown at each brightness level, sets the fade speed

static void pwm_frame(uint8_t duty)
{
    uint8_t step = 0;

    do
    {
        PORTB = (step < duty) ? ALL_LEDS : 0x00;    // on for the first duty steps, off after
        _delay_us(2);                               // fixed step length
    } while (++step != 0);                          // 256 steps, wraps back to 0
}

void pwm_fade(void)
{
    uint16_t level;
    uint8_t frame;

    // fade in - duty rises from 0 (off) to 255 (fully on)
    for (level = 0; level <= 255; level++)
    {
        for (frame = 0; frame < FADE_FRAMES_PER_LEVEL; frame++)
        {
            pwm_frame((uint8_t)level);
        }
    }

    // fade out - duty falls from 255 back to 0
    for (level = 256; level > 0; level--)
    {
        for (frame = 0; frame < FADE_FRAMES_PER_LEVEL; frame++)
        {
            pwm_frame((uint8_t)(level - 1));
        }
    }

    PORTB = 0x00;                                   // leave every LED off
}
