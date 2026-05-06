# Session 5 - Hardware PWM

---

## What is PWM

Pulse-Width Modulation (PWM) is a technique for controlling the average power delivered to a load by switching it rapidly between fully on and fully off. The ratio of on-time to total period is the duty cycle:

```text
duty cycle (%) = (on-time / period) x 100
```

At 0% duty cycle the output is always low. At 100% it is always high. At 50% the output is high for exactly half the period.

The average voltage at the output is:

```text
average voltage = VCC x (duty cycle / 100)
```

Hardware PWM uses the timer peripheral to generate the switching automatically. The CPU sets the duty cycle once by writing to a register and the hardware maintains the signal indefinitely without further software intervention.

---

## Hardware PWM on the ATmega644P

Timer 0 (8-bit) and Timer 1 (16-bit) can both generate hardware PWM. This session uses Timer 0 because its output compare pins map directly to LEDs on the current breadboard wiring.

Timer 0 hardware PWM output pins:

| Pin | Signal | Connected to     |
| --- | ------ | ---------------- |
| PB3 | OC0A   | Green LED (PB3)  |
| PB4 | OC0B   | Blue LED (PB4)   |

When Timer 0 is configured for hardware PWM and the OC0A or OC0B output is enabled, the timer hardware drives that pin automatically. Writing a value to OCR0A or OCR0B sets the duty cycle.

---

## Timer 0 Registers for PWM

| Register | Purpose                                                   |
| -------- | --------------------------------------------------------- |
| `TCCR0A` | Waveform generation and compare output mode               |
| `TCCR0B` | Clock select and waveform generation (WGM02)              |
| `OCR0A`  | Compare value A: sets duty cycle on OC0A (PB3)            |
| `OCR0B`  | Compare value B: sets duty cycle on OC0B (PB4)            |
| `TCNT0`  | 8-bit counter (runs automatically once timer is started)  |

---

## Fast PWM Mode (Mode 3)

In fast PWM mode the counter counts from 0 to 255 and then resets. The output is set high at 0 and cleared when the counter matches OCR0x. This gives a non-inverted PWM signal.

PWM frequency:

```text
f_PWM = f_clock / (256 x prescaler)
```

At 20 MHz with no prescaler:

```text
f_PWM = 20000000 / 256 = 78.125 kHz
```

With prescaler 8:

```text
f_PWM = 20000000 / (256 x 8) = 9765.6 Hz
```

### TCCR0A bit settings

For fast PWM mode 3 with non-inverting output on both channels:

- **WGM01, WGM00**: both set to 1 (fast PWM mode 3)
- **COM0A1, COM0A0**: set to 1, 0 (non-inverting PWM on OC0A, pin PB3)
- **COM0B1, COM0B0**: set to 1, 0 (non-inverting PWM on OC0B, pin PB4)

### TCCR0B bit settings for clock select

| CS02 | CS01 | CS00 | Clock              |
| ---- | ---- | ---- | ------------------ |
| 0    | 0    | 0    | Stopped            |
| 0    | 0    | 1    | clk (no prescale)  |
| 0    | 1    | 0    | clk / 8            |
| 0    | 1    | 1    | clk / 64           |
| 1    | 0    | 0    | clk / 256          |
| 1    | 0    | 1    | clk / 1024         |

---

## Single-Channel PWM Example

Drive the blue LED (PB4 = OC0B) at 50% duty cycle:

```c
#define F_CPU 20000000UL
#include <avr/io.h>

int main(void)
{
    DDRB |= (1<<PB4);                                       // PB4 (OC0B) must be an output; hardware PWM only appears on pin if DDR bit is set

    OCR0B  = 127;                                           // Compare value 127 / 255 gives approx 50% duty cycle on OC0B
    TCCR0A = (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);        // COM0B1: non-inverting PWM on OC0B; WGM01+WGM00: fast PWM mode 3
    TCCR0B = (1<<CS00);                                     // CS00=1: no prescaler, timer clock = F_CPU; this starts the timer

    while (1) { }                                           // PWM waveform runs entirely in hardware; CPU does not need to act
}
```

Change `OCR0B` to any value from 0 (fully off) to 255 (fully on) to adjust brightness.

---

## Dual-Channel PWM Example

Drive both the green LED (PB3 = OC0A) and the blue LED (PB4 = OC0B) at different brightness levels:

```c
#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB |= (1<<PB3) | (1<<PB4);                               // PB3 (OC0A, green) and PB4 (OC0B, blue) both as outputs

    TCCR0A = (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00); // Non-inverting PWM on OC0A and OC0B; fast PWM mode 3
    TCCR0B = (1<<CS00);                                         // No prescaler; timer starts running immediately

    while (1)
    {
        // Fade green LED from dim to bright in steps
        for(uint8_t brightness = 0; brightness < 255; brightness++)
        {
            OCR0A = brightness;             // Green LED duty cycle: 0 (off) to 254 (near full brightness)
            OCR0B = 255 - brightness;       // Blue LED duty cycle: inverse of green (full when green is off)
            _delay_ms(5);                   // 5 ms per step; full cycle takes 255 x 5 ms = 1.275 s
        }
    }
}
```

Both channels share the same timer, so they have the same frequency but independent duty cycles.

---

## Software PWM (for comparison)

The projects already built use software PWM, which toggles a pin manually in a tight loop:

```c
// Software PWM - blocks the CPU
for(uint8_t duty = 0; duty < 255; duty++)
{
    PORTB = ALL_LEDS;
    _delay_us(1);       // On time
    PORTB = 0x00;
    _delay_ms(1);       // Off time
}
```

Hardware PWM runs independently in the timer peripheral. Software PWM ties up the CPU for the entire waveform period.

---

## Waveform Generation with PWM

PWM can generate analogue-like waveforms by changing the duty cycle rapidly. A sine wave is approximated by stepping through a lookup table of pre-calculated duty cycle values:

```c
#define TABLE_LENGTH 36

uint8_t sine_table[TABLE_LENGTH] = {
    0x80, 0x96, 0xAB, 0xBF, 0xD1, 0xE1, 0xEE, 0xF7, 0xFD, 0xFF,
    0xFD, 0xF7, 0xEE, 0xE1, 0xD1, 0xBF, 0xAB, 0x96, 0x80, 0x69,
    0x54, 0x40, 0x2E, 0x1E, 0x11, 0x08, 0x02, 0x00, 0x02, 0x08,
    0x11, 0x1E, 0x2E, 0x40, 0x54, 0x69
};

for(uint8_t i = 0; i < TABLE_LENGTH; i++)
{
    OCR0B = sine_table[i];
    _delay_us(770);     // 36 steps x 770 µs = 27.72 ms period = ~36 Hz
}
```

The table contains 36 values representing one full sine cycle in 10-degree steps. Each value maps to a duty cycle between 0 and 255. When played through a low-pass filter, the LED or output pin produces a smooth sine waveform.

---

## Duty Cycle and Voltage

For a 5V supply, the average output voltage scales linearly with the duty cycle:

| OCR0x value | Duty cycle | Average voltage |
| ----------- | ---------- | --------------- |
| 0           | 0%         | 0.0 V           |
| 64          | 25%        | 1.25 V          |
| 128         | 50%        | 2.5 V           |
| 192         | 75%        | 3.75 V          |
| 255         | 100%       | 5.0 V           |

The visible LED brightness follows a similar relationship, though the human eye perceives brightness logarithmically rather than linearly.

---

## Practical Checklist

1. Set the DDR bit for the OC pin as an output before starting the timer.
2. Set OCR0x before enabling the timer clock (before CS bits in TCCR0B).
3. For OC0A, set COM0A1 in TCCR0A. For OC0B, set COM0B1.
4. WGM01 and WGM00 both need to be set for fast PWM mode 3.
5. OCR0x = 0 gives 0% duty (LED off), OCR0x = 255 gives 100% (LED fully on).
