# Session 5 Lab - Hardware PWM

Hardware: ATmega644P on Richard Reeves AVR PCB 2018.
Programmer: Pololu USB AVR Programmer v2.1 on COM4.
Clock: 20 MHz external crystal.

---

## Lab Goal

Configure Timer 0 to generate hardware PWM on the green and blue LEDs, control LED brightness by writing to OCR0A and OCR0B, and generate a waveform by stepping through a lookup table.

---

## Pre-Lab Calculations

At 20 MHz with no prescaler:

```text
f_PWM = f_clock / 256 = 20000000 / 256 = ____________ Hz
```

With prescaler 8:

```text
f_PWM = f_clock / (256 x 8) = 20000000 / 2048 = ____________ Hz
```

Write both answers above before starting.

---

## Breadboard Setup

This session uses Timer 0 hardware PWM on two output pins only. No other LEDs or peripherals are needed.

**Components required:**
- 1x green LED with a 330 Ω current-limiting resistor
- 1x blue LED with a 330 Ω current-limiting resistor
- Jumper wires

**Connections:**
- J4 pin 1 (VCC) → positive power rail on breadboard
- J4 pin 10 (GND) → negative power rail on breadboard
- J4 pin 5 (PB3, OC0A) → anode of green LED → cathode → 330 Ω resistor → GND rail
- J4 pin 6 (PB4, OC0B) → anode of blue LED → cathode → 330 Ω resistor → GND rail

No UART header, no potentiometer and no other components are needed for this session.

---

## Task 1 - Fixed Duty Cycle on Blue LED

Write a programme that drives OC0B (PB4, blue LED) at approximately 25% duty cycle:

```c
#define F_CPU 20000000UL
#include <avr/io.h>

int main(void)
{
    DDRB |= (1<<PB4);                               // OC0B must be an output

    OCR0B  = 64;                                    // 25% of 255
    TCCR0A = (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
    TCCR0B = (1<<CS00);                             // No prescaler

    while (1) { }
}
```

Build and upload. Observe the blue LED.

Expected result: the blue LED is visibly dimmer than at full brightness.

Now change OCR0B to each of the values in the table and record the perceived brightness:

| OCR0B | Duty cycle | Observation |
| ----- | ---------- | ----------- |
| 0     | 0%         |             |
| 64    | 25%        |             |
| 128   | 50%        |             |
| 192   | 75%        |             |
| 255   | 100%       |             |

Pass criteria: visible brightness steps between each value.

---

## Task 2 - Dual-Channel PWM

Add OC0A (PB3, green LED) as a second PWM output and drive both channels simultaneously at opposite duty cycles:

```c
#define F_CPU 20000000UL
#include <avr/io.h>

int main(void)
{
    DDRB |= (1<<PB3) | (1<<PB4);

    TCCR0A = (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
    TCCR0B = (1<<CS00);

    OCR0A = 64;     // Green at 25%
    OCR0B = 192;    // Blue at 75%

    while (1) { }
}
```

Build and observe both LEDs simultaneously.

Expected result: green is dim, blue is bright.

Swap the values so green is bright and blue is dim. Rebuild and confirm.

Pass criteria: both channels respond independently to OCR0A and OCR0B changes.

---

## Task 3 - Software Fade

Use a `for` loop to slowly fade the blue LED from off to full brightness and back:

```c
#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB |= (1<<PB4);

    TCCR0A = (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
    TCCR0B = (1<<CS00);

    while (1)
    {
        for(uint8_t i = 0; i < 255; i++)
        {
            OCR0B = i;
            _delay_ms(5);
        }
        for(uint8_t i = 255; i > 0; i--)
        {
            OCR0B = i;
            _delay_ms(5);
        }
    }
}
```

Build and observe.

Expected result: blue LED fades smoothly in and out.

Now add the green LED fading in the opposite direction (when blue is bright, green is dim):

```c
OCR0A = 255 - i;    // Add this line inside both loops
```

Pass criteria: blue and green LED crossfade smoothly.

---

## Task 4 - Sine Wave Table

Add a sine wave lookup table and step through it to drive the blue LED:

```c
#define TABLE_LENGTH 36

uint8_t sine_table[TABLE_LENGTH] = {
    0x80, 0x96, 0xAB, 0xBF, 0xD1, 0xE1, 0xEE, 0xF7, 0xFD, 0xFF,
    0xFD, 0xF7, 0xEE, 0xE1, 0xD1, 0xBF, 0xAB, 0x96, 0x80, 0x69,
    0x54, 0x40, 0x2E, 0x1E, 0x11, 0x08, 0x02, 0x00, 0x02, 0x08,
    0x11, 0x1E, 0x2E, 0x40, 0x54, 0x69
};
```

Inside `while(1)`, replace the fade loop with:

```c
for(uint8_t i = 0; i < TABLE_LENGTH; i++)
{
    OCR0B = sine_table[i];
    _delay_us(770);     // 36 steps x 770 us = approx 36 Hz
}
```

Build and observe.

Expected result: blue LED pulses with a smooth sine-wave rhythm rather than a linear ramp.

Calculate the period and frequency:

```text
period  = TABLE_LENGTH x step_delay = 36 x 770 us = ____________ ms
f_wave  = 1 / period = ____________ Hz
```

Adjust the delay to produce a 50 Hz waveform and recalculate.

---

## Quick Fault Isolation

- **LED stays fully off**: COM0B1 not set in TCCR0A, or DDR bit not set. Check both.
- **LED stays fully on**: WGM bits wrong. Confirm WGM01=1 and WGM00=1 in TCCR0A.
- **Fade is not smooth**: step delay too large. Reduce delay value or increase table length.
- **Both LEDs behave the same**: COM0A1 missing for the green channel. Add it to TCCR0A.

---

## Lab Record

- Date:
- PWM frequency calculated (Task pre-lab):
- Brightness observations (Task 1 table):
- Crossfade behaviour (Task 3):
- Sine wave period calculated (Task 4):
- Adjusted delay for 50 Hz:
- Issues found:
- Fix applied:
