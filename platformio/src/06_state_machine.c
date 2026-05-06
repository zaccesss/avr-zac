/*
 * 06_state_machine.c
 *
 * Nine-mode LED state machine for the ATmega644P on the Richard Reeves AVR PCB 2018.
 * Button on PD2 (INT0) cycles through modes on each falling-edge press.
 * Modes: chase, blink all, alternate, PWM fade, Knight Rider, binary counter,
 *        random, reaction game and Tetris melody.
 *
 * Hardware: ATmega644P at 20 MHz, Pololu USB AVR Programmer v2.1
 * Build:    PlatformIO (VS Code), environment 06_state_machine
 */

#include <avr/io.h>             // AVR input/output register definitions
#include <util/delay.h>         // AVR delay functions
#include <avr/interrupt.h>      // AVR interrupt definitions
#include <stdlib.h>             // For rand()

// LED pins on PORTB
#define LED_RED    PB0          // Red LED on Port B pin 0
#define LED_YELLOW PB1          // Yellow LED on Port B pin 1
#define LED_WHITE  PB2          // White LED on Port B pin 2
#define LED_GREEN  PB3          // Green LED on Port B pin 3
#define LED_BLUE   PB4          // Blue LED on Port B pin 4

// Buzzer and button on PORTD
#define BUZZER     PD3          // Active buzzer on Port D pin 3
#define BUTTON     PD2          // Push button on Port D pin 2 (INT0)

// Bitmask for all 5 LEDs combined
#define ALL_LEDS ((1<<LED_RED)|(1<<LED_YELLOW)|(1<<LED_WHITE)|(1<<LED_GREEN)|(1<<LED_BLUE))

// Enum defining all LED modes
// Each mode is a named state in the state machine
typedef enum {
    MODE_CHASE,                 // Mode 0: LEDs chase one by one left to right
    MODE_BLINK_ALL,             // Mode 1: All LEDs blink on and off together
    MODE_ALTERNATE,             // Mode 2: Odd and even LEDs alternate
    MODE_PWM_FADE,              // Mode 3: All LEDs fade in and out smoothly
    MODE_KNIGHT_RIDER,          // Mode 4: Single LED sweeps left to right and back
    MODE_BINARY,                // Mode 5: LEDs count up in binary from 0 to 31
    MODE_RANDOM,                // Mode 6: LEDs light up in random patterns
    MODE_REACTION,              // Mode 7: Reaction game - press button on green LED
    MODE_MUSIC,                 // Mode 8: Tetris theme plays with LEDs in sync
    MODE_COUNT                  // Always last - used to wrap mode back to 0
} LED_Mode;

// Global volatile variables - shared between main loop and ISR
volatile LED_Mode current_mode = MODE_CHASE;    // Tracks the current active mode
volatile uint8_t button_pressed = 0;            // Flag set by ISR for reaction game

// Function prototypes - declared here so functions can be called before they are defined
void startup_animation(void);
void pwm_fade(void);
void knight_rider(void);
void binary_counter(void);
void random_mode(void);
void reaction_game(void);
void music_mode(void);
void beep(void);
void debounce_delay(void);
void tone(uint16_t freq, uint16_t duration_ms);
uint8_t get_random(void);

int main(void)
{
    // Set LED pins as outputs on PORTB
    DDRB |= ALL_LEDS;

    // Set buzzer as output on PORTD
    DDRD |= (1<<BUZZER);

    // Set button as input on PORTD
    DDRD &= ~(1<<BUTTON);

    // Configure INT0 (PD2) to trigger interrupt on falling edge
    // Falling edge means button press (HIGH to LOW transition)
    EICRA |= (1<<ISC01);        // Set ISC01 to trigger on falling edge
    EICRA &= ~(1<<ISC00);       // Clear ISC00 to complete falling edge config
    EIMSK |= (1<<INT0);         // Enable the INT0 external interrupt

    // Enable global interrupts so the ISR can fire
    sei();

    // Run the startup animation once when the board powers on
    startup_animation();

    // Main loop - runs forever switching between modes
    while (1)
    {
        switch (current_mode)
        {
            case MODE_CHASE:
                // Light each LED in sequence one at a time
                PORTB = (1<<LED_RED);           // Red on, all others off
                _delay_ms(300);
                PORTB = (1<<LED_YELLOW);        // Yellow on, all others off
                _delay_ms(300);
                PORTB = (1<<LED_WHITE);         // White on, all others off
                _delay_ms(300);
                PORTB = (1<<LED_GREEN);         // Green on, all others off
                _delay_ms(300);
                PORTB = (1<<LED_BLUE);          // Blue on, all others off
                _delay_ms(300);
                PORTB = 0x00;                   // All off before repeating
                _delay_ms(300);
                break;

            case MODE_BLINK_ALL:
                // All LEDs blink on and off together
                PORTB = ALL_LEDS;               // All on
                _delay_ms(400);
                PORTB = 0x00;                   // All off
                _delay_ms(400);
                break;

            case MODE_ALTERNATE:
                // Red, white and blue on first then yellow and green
                PORTB = (1<<LED_RED)|(1<<LED_WHITE)|(1<<LED_BLUE);
                _delay_ms(400);
                PORTB = (1<<LED_YELLOW)|(1<<LED_GREEN);
                _delay_ms(400);
                break;

            case MODE_PWM_FADE:
                // Software PWM fade in and out
                pwm_fade();
                break;

            case MODE_KNIGHT_RIDER:
                // Single LED sweeps left to right and back like KITT
                knight_rider();
                break;

            case MODE_BINARY:
                // Count from 0 to 31 in binary on the 5 LEDs
                binary_counter();
                break;

            case MODE_RANDOM:
                // Random LED combinations
                random_mode();
                break;

            case MODE_REACTION:
                // Reaction game - press button when green LED is lit
                reaction_game();
                break;

            case MODE_MUSIC:
                // Play Tetris theme with LEDs synced to the notes
                music_mode();
                break;

            default:
                // Safety net - reset to chase if mode is unknown
                current_mode = MODE_CHASE;
                break;
        }
    }
}

// ISR fires instantly when button is pressed (INT0 falling edge)
// Debounces the press then cycles to the next mode
ISR(INT0_vect)
{
    debounce_delay();                           // Wait for button bounce to settle

    if (!(PIND & (1<<BUTTON)))                  // Confirm button is still held down
    {
        button_pressed = 1;                     // Set flag used by reaction game
        current_mode = (current_mode + 1) % MODE_COUNT;    // Move to next mode
        PORTB = 0x00;                           // Clear all LEDs on mode change
        beep();                                 // Short beep confirms the mode change
    }
}

// Startup animation plays once when the board powers on
// LEDs chase on one by one then flash three times with a long ready beep
void startup_animation(void)
{
    uint8_t i;

    // Chase LEDs on one by one with a beep each
    for (i = 0; i < 5; i++)
    {
        PORTB |= (1<<i);                        // Turn on LED i
        beep();
        _delay_ms(100);
    }

    // Flash all LEDs three times to signal ready
    for (i = 0; i < 3; i++)
    {
        PORTB = ALL_LEDS;                       // All on
        _delay_ms(150);
        PORTB = 0x00;                           // All off
        _delay_ms(150);
    }

    // Long beep to confirm board is ready
    PORTD |= (1<<BUZZER);
    _delay_ms(300);
    PORTD &= ~(1<<BUZZER);
    _delay_ms(200);
}

// Software PWM fade - rapidly toggles LEDs to simulate brightness levels
// Longer on time = brighter, shorter on time = dimmer
void pwm_fade(void)
{
    uint8_t i;
    uint8_t j;

    // Fade in - on time increases from 0 to 254
    for (i = 0; i < 255; i++)
    {
        for (j = 0; j < 50; j++)               // Repeat each level 50 times for visibility
        {
            PORTB = ALL_LEDS;                   // LEDs on
            _delay_us(1);                       // Fixed on time
        }
        PORTB = 0x00;                           // LEDs off
        _delay_ms(1);                           // Fixed off time
    }

    // Fade out - on time decreases from 254 to 1
    for (i = 255; i > 0; i--)
    {
        for (j = 0; j < 50; j++)
        {
            PORTB = ALL_LEDS;
            _delay_us(1);
        }
        PORTB = 0x00;
        _delay_ms(1);
    }
}

// Knight Rider sweep - single LED moves right then left repeatedly
void knight_rider(void)
{
    int8_t i;

    // Sweep right from LED 0 to LED 4
    for (i = 0; i < 5; i++)
    {
        PORTB = (1<<i);                         // Light only LED i
        _delay_ms(100);
    }

    // Sweep left from LED 3 back to LED 0
    for (i = 3; i >= 0; i--)
    {
        PORTB = (1<<i);
        _delay_ms(100);
    }
}

// Binary counter - counts from 0 to 31 displayed in binary on 5 LEDs
// Each LED represents a binary bit: BLUE=16 GREEN=8 WHITE=4 YELLOW=2 RED=1
void binary_counter(void)
{
    uint8_t i;

    for (i = 0; i < 32; i++)
    {
        PORTB = i & ALL_LEDS;                   // Mask to only affect LED pins
        _delay_ms(300);
    }

    PORTB = 0x00;                               // Clear after reaching 31
    _delay_ms(300);
}

// Read ADC noise from floating pin to generate pseudo random number
uint8_t get_random(void)
{
    ADMUX = 0x00;                               // Select ADC0 with AVCC reference
    ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);    // Enable and start conversion
    while (ADCSRA & (1<<ADSC));                 // Wait for conversion to complete
    return ADCL;                                // Return low byte as random value
}

// Random mode - lights random combinations of LEDs every 300ms
void random_mode(void)
{
    uint8_t random_val = get_random() & 0x1F;  // Mask to 5 bits for 5 LEDs
    PORTB = random_val;                         // Apply random pattern
    _delay_ms(300);
}

// Reaction game - LEDs cycle randomly, press button when green LED is lit
// Win = all LEDs flash with long beep, Lose = three short beeps
void reaction_game(void)
{
    uint8_t target = (1<<LED_GREEN);            // Green LED is the target
    uint8_t current_led;
    uint8_t i;

    button_pressed = 0;                         // Reset button flag at game start

    // Cycle through 10 random LEDs
    for (i = 0; i < 10; i++)
    {
        current_led = (1<<(get_random() % 5)); // Pick a random LED
        PORTB = current_led;                    // Light it up
        _delay_ms(500);                         // Hold for 500 ms

        if (button_pressed)                     // Button was pressed during this LED
        {
            button_pressed = 0;                 // Clear the flag

            if (current_led == target)          // Correct - green LED was lit
            {
                // Win reaction - flash all LEDs with long beep
                PORTB = ALL_LEDS;
                PORTD |= (1<<BUZZER);
                _delay_ms(500);
                PORTD &= ~(1<<BUZZER);
                PORTB = 0x00;
            }
            else                                // Wrong LED was lit
            {
                // Lose reaction - three short beeps
                uint8_t j;
                for (j = 0; j < 3; j++)
                {
                    PORTD |= (1<<BUZZER);
                    _delay_ms(100);
                    PORTD &= ~(1<<BUZZER);
                    _delay_ms(100);
                }
                PORTB = 0x00;
            }
            return;                             // Exit game after button press
        }
    }
}

// Generate a buzzer tone using fixed 250us half period (approx 2kHz)
// freq parameter is kept for future improvement when variable delay is possible
void tone(uint16_t freq, uint16_t duration_ms)
{
    uint16_t i;
    uint16_t cycles = (uint32_t)duration_ms * freq / 1000;     // Number of on/off cycles

    for (i = 0; i < cycles; i++)
    {
        PORTD |= (1<<BUZZER);                   // Buzzer on
        _delay_us(250);                         // Fixed half period
        PORTD &= ~(1<<BUZZER);                  // Buzzer off
        _delay_us(250);                         // Fixed half period
    }
}

// Tetris theme melody with LEDs cycling in time with the notes
void music_mode(void)
{
    // Melody stored as pairs of frequency (Hz) and duration (ms)
    // 0 frequency = rest (silence)
    uint16_t melody[][2] = {
        {659, 300}, {494, 150}, {523, 150},     // E5 B4 C5
        {587, 300}, {523, 150}, {494, 150},     // D5 C5 B4
        {440, 300}, {440, 150}, {523, 150},     // A4 A4 C5
        {659, 300}, {587, 150}, {523, 150},     // E5 D5 C5
        {494, 450}, {523, 150}, {587, 300},     // B4 C5 D5
        {659, 300}, {523, 300}, {440, 300},     // E5 C5 A4
        {440, 300}, {0,   300}                  // A4 rest
    };

    uint8_t note_count = sizeof(melody) / sizeof(melody[0]);
    uint8_t i;

    for (i = 0; i < note_count; i++)
    {
        PORTB = (1<<(i % 5));                   // Cycle through LEDs in time with notes

        if (melody[i][0] == 0)
            _delay_ms(melody[i][1]);            // Rest - just wait
        else
            tone(melody[i][0], melody[i][1]);   // Play the note

        PORTB = 0x00;                           // Brief LED off between notes
        _delay_ms(50);
    }
}

// Short beep used for button confirmation and startup
void beep(void)
{
    PORTD |= (1<<BUZZER);                       // Buzzer on
    _delay_ms(80);                              // 80 ms beep duration
    PORTD &= ~(1<<BUZZER);                      // Buzzer off
}

// Software debounce - waits for mechanical button bounce to settle
void debounce_delay(void)
{
    _delay_ms(20);                              // 20 ms is sufficient for most buttons
}
