# Session 4 Lab - Timers: Overflow and Output Compare

Hardware: ATmega644P on Richard Reeves AVR PCB 2018.
Programmer: Pololu USB AVR Programmer v2.1 on COM4.
Clock: 20 MHz external crystal.

---

## Lab Goal

Use Timer 1 to generate precise timed events without blocking the CPU, first in overflow mode then in output-compare (CTC) mode.

---

## Breadboard Setup

This session uses two LEDs wired to PORTB via header J4 on the PCB. Only PB0 and PB1 need to be connected for the timer tasks.

**Header J4 (PORTB, 10-way):**
- Pin 1: VCC rail supply
- Pin 2: PB0 - Red LED via 220R resistor
- Pin 3: PB1 - Yellow LED via 220R resistor
- Pin 10: GND rail supply

**LED wiring (same for each LED):**
- Anode (long leg) connects via a 220R resistor to the header pin
- Cathode (short leg) connects directly to the GND rail

**Rails:**
- VCC rail: wire from J4 pin 1 to the positive rail on the breadboard
- GND rail: wire from J4 pin 10 to the negative rail on the breadboard

---

## Pre-Lab Calculations

Complete these before writing any code.

### Overflow mode

Target: toggle the red LED at 2 Hz (500 ms on, 500 ms off = 250 ms per toggle).

Using no prescaler:

```text
reload = 65536 - (f_clock x time) = 65536 - (20000000 x 0.250)
reload = 65536 - 5000000
```

This exceeds the 16-bit range. Choose prescaler 256:

```text
reload = 65536 - ((20000000 / 256) x 0.250)
reload = 65536 - 19531
reload = 45005
```

Write your reload value here: ____________

### CTC mode

Target: toggle the red LED at 2 Hz (250 ms per toggle).

Using prescaler 256:

```text
OCR1A = ((f_clock / 256) x 0.250) - 1
OCR1A = (78125 x 0.250) - 1
OCR1A = 19531 - 1
OCR1A = 19530
```

Write your compare value here: ____________

---

## Task 1 - Timer 1 in Overflow Mode

Create a new project environment or use a scratch file. Enter the following programme, substituting your calculated reload value:

```c
#define F_CPU 20000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define RED_LED  PB0
#define RELOAD   45005U

int main(void)
{
    DDRB |= (1<<RED_LED);       // Set the red LED pin as an output

    TCNT1  = RELOAD;            // Preload the counter so it overflows after 250 ms
    TIMSK1 = (1<<TOIE1);        // Enable Timer 1 overflow interrupt
    TCCR1B = (1<<CS12);         // Prescaler 256: CS12=1, CS11=0, CS10=0

    sei();                      // Enable global interrupts so the timer ISR can fire

    while (1) { }               // CPU is free; timer handles LED toggling
}

ISR(TIMER1_OVF_vect)
{
    TCNT1 = RELOAD;             // Reload must be first line in ISR
    PORTB ^= (1<<RED_LED);      // Toggle the red LED on each overflow
}
```

Build and flash. Observe the red LED.

Expected result: red LED toggles at a steady 250 ms interval (2 Hz blink).

Pass criteria: blink rate is visually consistent over 30 seconds.

---

## Task 2 - Main Loop Independence

Add a second LED pattern to the main loop that runs independently of the timer:

```c
while (1)
{
    PORTB ^= (1<<PB1);          // Yellow LED toggles in main loop
    _delay_ms(1000);            // 1 second blocking delay
}
```

Build and observe both LEDs.

Expected result: the red LED blinks at 250 ms driven by the timer interrupt, the yellow LED blinks at 1000 ms driven by `_delay_ms()` in the main loop. Both run simultaneously and independently.

Pass criteria: removing the timer ISR stops the red LED but the yellow LED is unaffected.

---

## Task 3 - Timer 1 in CTC Mode

Replace the overflow-mode programme with a CTC version using your calculated OCR1A value:

```c
#define F_CPU 20000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define RED_LED  PB0
#define COMPARE  19530U

int main(void)
{
    DDRB |= (1<<RED_LED);       // Set the red LED pin as an output

    OCR1A  = COMPARE;           // Set the compare target value for CTC mode
    TCCR1B = (1<<WGM12) | (1<<CS12);   // CTC mode, prescaler 256
    TIMSK1 = (1<<OCIE1A);       // Enable Timer 1 compare A interrupt

    sei();                      // Enable global interrupts so the timer ISR can fire

    while (1) { }               // CPU is free; timer ISR handles LED toggling
}

ISR(TIMER1_COMPA_vect)
{
    PORTB ^= (1<<RED_LED);      // No reload needed in CTC mode
}
```

Build and flash. Confirm the blink rate matches the overflow mode version.

Pass criteria: LED blinks at 2 Hz with no reload needed in the ISR.

---

## Task 4 - Adjusting the Target Frequency

Calculate the OCR1A value for a 4 Hz blink (125 ms per toggle) with prescaler 256:

```text
OCR1A = ((20000000 / 256) x 0.125) - 1 = ____________
```

Edit the `COMPARE` define and rebuild. Observe the new blink rate.

Pass criteria: LED blinks visibly faster than in Task 3.

---

## Task 5 - Longer Period

Calculate the OCR1A value for a 0.5 Hz blink (1000 ms per toggle) with prescaler 1024:

```text
OCR1A = ((20000000 / 1024) x 1.000) - 1 = ____________
```

Update `TCCR1B` to select prescaler 1024 (CS12=1, CS10=1) and update `COMPARE`. Rebuild and observe.

Pass criteria: LED blinks clearly slower than in Task 3.

---

## Quick Fault Isolation

| Symptom                   | Likely cause                        | Fix                                         |
| ------------------------- | ----------------------------------- | ------------------------------------------- |
| LED never blinks          | TIMSK1 or sei() missing             | Check both lines are present                |
| Blink rate wildly wrong   | Prescaler bits set incorrectly      | Cross-check CS12/CS11/CS10 against table    |
| Overflow mode drifts      | TCNT1 reload missing from ISR       | Reload must be the first line in the ISR    |
| CTC mode has no ISR fires | WGM12 not set in TCCR1B             | Confirm WGM12 bit is set alongside CS bits  |

---

## Lab Record

- Date:
- Calculated reload for overflow mode:
- Calculated OCR1A for CTC mode:
- Observed blink rates (Task 3, Task 4, Task 5):
- Main loop observation (Task 2):
- Issues found:
- Fix applied:
