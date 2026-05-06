# Session 2 Lab - Bit Shifting, Arrays and Data Types

Hardware: ATmega644P on Richard Reeves AVR PCB 2018.
Programmer: Pololu USB AVR Programmer v2.1 on COM4.
Clock: 20 MHz external crystal.

---

## Lab Goal

Build confidence with bit manipulation, arrays and loops by writing LED control patterns that use only safe bit-mask operations and no raw register overwrites.

---

## Breadboard Setup

This session uses five LEDs wired to PORTB via header J4 on the PCB.

**Header J4 (PORTB, 10-way):**
- Pin 1: VCC rail supply
- Pin 2: PB0 - Red LED via 220R resistor
- Pin 3: PB1 - Yellow LED via 220R resistor
- Pin 4: PB2 - White LED via 220R resistor
- Pin 5: PB3 - Green LED via 220R resistor
- Pin 6: PB4 - Blue LED via 220R resistor
- Pin 10: GND rail supply

**LED wiring (same for each LED):**
- Anode (long leg) connects via a 220R resistor to the header pin
- Cathode (short leg) connects directly to the GND rail

**Rails:**
- VCC rail: wire from J4 pin 1 to the positive rail on the breadboard
- GND rail: wire from J4 pin 10 to the negative rail on the breadboard

---

## Pre-Lab Checks

1. Power off before rewiring anything.
2. Confirm all five LED resistors are 220R and connected to J4 pins 2 to 6.
3. Confirm common GND between PCB and breadboard.
4. Confirm JP2 is not fitted (5V).

---

## Task 1 - Seven-Segment Bit Mapping (Theory)

A seven-segment display has segments labelled A to G. To display the digit 1, segments B and C are lit. To display 7, segments A, B and C are lit.

Although this setup uses individual LEDs rather than a seven-segment display, the same principle applies: each output is controlled by one bit in a port register.

Complete the table for the five LEDs by writing a 1 when the LED should be on:

| Number | Blue PB4 | Green PB3 | White PB2 | Yellow PB1 | Red PB0 | PORTB value |
| ------ | -------- | --------- | --------- | ---------- | ------- | ----------- |
| 0      | 0        | 0         | 0         | 0          | 0       | 0x00        |
| 1      | 0        | 0         | 0         | 0          | 1       | 0x01        |
| 2      |          |           |           |            |         |             |
| 3      |          |           |           |            |         |             |
| 4      |          |           |           |            |         |             |

These binary patterns will be used in Task 3.

---

## Task 2 - Single LED Blink with Bit Masks

File: [projects/learning_projects/01_blink/01_blink.c](../../projects/learning_projects/01_blink/01_blink.c)

1. Select environment `01_blink` in the VS Code status bar.
2. Open the source file and confirm PB0 is set as output using `DDRB |= (1<<PB0)`.
3. Confirm the LED is toggled with `|=` and `&= ~()`, not with direct PORTB assignment.
4. Build and upload.

Expected result: the red LED blinks at a steady rate.

Now change the ON delay to 200ms and the OFF delay to 800ms. Build and upload.

Expected result: the LED is on for noticeably less time than it is off.

Pass criteria: both delay values visible by inspection, no compiler warnings.

---

## Task 3 - LED Pattern Array

Create a new file based on `02_led_cycle.c`. Replace the explicit sequential assignments with an array of patterns and a `for` loop:

```c
uint8_t patterns[5] = {
    (1<<PB0),
    (1<<PB1),
    (1<<PB2),
    (1<<PB3),
    (1<<PB4)
};

for(int8_t i = 0; i < 5; i++)
{
    PORTB = patterns[i];
    _delay_ms(300);
}
PORTB = 0x00;
_delay_ms(300);
```

Build and test. Confirm the sequence matches the pattern array order.

Now add a sixth pattern to the array that lights the red and blue LEDs simultaneously:

```c
(1<<PB0) | (1<<PB4)
```

Update the loop limit to 6 and build again.

Pass criteria: six distinct states cycle correctly, including the combined pattern.

---

## Task 4 - Counting Patterns with a for Loop

Write a loop that counts from 0 to 31 in binary on the five LEDs:

```c
for(uint8_t count = 0; count < 32; count++)
{
    PORTB = count & 0x1F;   // Mask to lower five bits only
    _delay_ms(200);
}
```

The mask `0x1F` (binary `00011111`) ensures only PB0 to PB4 are affected even though `count` is 8 bits wide.

Pass criteria: LEDs count from 0 (all off) to 31 (all on) in binary, then restart.

---

## Task 5 - Adjusting the Range

Alter the loop from Task 4 to count from 3 to 8 only:

1. Change the initialiser to `count = 3`.
2. Change the condition to `count <= 8`.
3. Build and observe.

Make a note of what visible pattern you see and why the sequence looks the way it does.

---

## Task 6 - Counting Down

Alter the loop to count down from 31 to 0:

```c
for(int8_t count = 31; count >= 0; count--)
{
    PORTB = count & 0x1F;
    _delay_ms(200);
}
```

Note why `int8_t` is used instead of `uint8_t` for a loop that decrements to 0.

Pass criteria: LEDs count from 31 down to 0 then restart.

---

## Quick Fault Isolation

| Symptom                  | Likely cause                           | Fix                                  |
| ------------------------ | -------------------------------------- | ------------------------------------ |
| All LEDs wrong pattern   | Array index out of range               | Check loop limit matches array size  |
| Bits leak into wrong pin | Missing parentheses around mask        | Add parentheses before the tilde     |
| Loop runs once only      | Wrong condition or wrong variable type | Check condition and use int8_t       |
| Wrong LED responds       | Pin map error                          | Cross-check entry against wiring     |

---

## Lab Record

- Date:
- Tasks completed:
- Pattern array contents used:
- Timing values tested:
- Notes on counting up vs. counting down:
- Issues found:
- Fix applied:
