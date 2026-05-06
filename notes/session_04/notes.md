# Session 4 - Timers: Overflow and Output Compare

---

## Why Timers

Using `_delay_ms()` blocks the CPU for the entire delay duration. The CPU cannot do anything else while waiting. Hardware timers count independently of the CPU. They generate an interrupt when they reach a target value, freeing the CPU to do other work between events.

---

## Counter Fundamentals

A counter with N bits has 2^N possible states and counts from 0 to 2^N - 1 before rolling over. The repeat time depends on the number of states counted and the clock speed:

```text
Loop time = states × clock period = states / f_clock
```

For a 16-bit counter at 20 MHz with no prescaler:

```text
Maximum loop time = 65536 / 20000000 = 3.2768 ms
Resolution = 1 / 20000000 = 50 ns
```

---

## Timer 1 on the ATmega644P

Timer 1 is a 16-bit timer with a 16-bit counter register TCNT1. It can run in several modes. This session covers overflow mode and output-compare (CTC) mode.

Key registers:

| Register | Purpose                                             |
| -------- | --------------------------------------------------- |
| `TCNT1`  | 16-bit counter value (read or write to set reload)  |
| `TCCR1A` | Timer 1 control register A (waveform generation)    |
| `TCCR1B` | Timer 1 control register B (clock select, CTC mode) |
| `TIMSK1` | Timer 1 interrupt mask (enable overflow or compare) |
| `OCR1A`  | Output compare register A (target value for CTC)    |
| `OCR1B`  | Output compare register B (second compare channel)  |
| `TIFR1`  | Timer 1 interrupt flag register                     |

---

## Overflow Mode

In overflow mode the counter counts from a reload value up to 65535 (0xFFFF), then triggers an interrupt as it wraps back to 0. The ISR reloads TCNT1 with the reload value and repeats.

### Time calculation

```text
overflow time = (65536 - reload) / f_clock
```

Rearranging for the reload value:

```text
reload = 65536 - (f_clock × time)
```

Example: 250 µs at 20 MHz with no prescaler:

```text
reload = 65536 - (20000000 × 0.000250) = 65536 - 5000 = 60536
```

### Overflow mode code

```c
#include <avr/io.h>
#include <avr/interrupt.h>

#define RED_LED PB0
#define RELOAD  60536       // 250 µs at 20 MHz

int main(void)
{
    DDRB |= (1<<RED_LED);

    TCNT1  = RELOAD;            // Load initial reload value
    TIMSK1 = (1<<TOIE1);        // Enable Timer 1 overflow interrupt
    TCCR1B = (1<<CS10);         // Start timer, no prescaler

    sei();

    while (1) { }               // CPU free to do other work
}

ISR(TIMER1_OVF_vect)
{
    TCNT1 = RELOAD;             // Reload the counter
    PORTB ^= (1<<RED_LED);      // Toggle LED
}
```

### Overflow mode limitation

The software reload in the ISR introduces a small timing error on every overflow because several clock cycles pass between the overflow occurring and the ISR writing the new TCNT1 value. Output-compare mode eliminates this error with hardware.

---

## Prescalers

When the required time is longer than the maximum overflow time, a prescaler divides the system clock before it reaches the timer. This extends the maximum time at the cost of reduced resolution.

Timer 1 prescaler options (CS12:CS10 bits in TCCR1B):

| CS12 | CS11 | CS10 | Clock source          |
| ---- | ---- | ---- | --------------------- |
| 0    | 0    | 0    | Timer stopped         |
| 0    | 0    | 1    | clk (no prescale)     |
| 0    | 1    | 0    | clk / 8               |
| 0    | 1    | 1    | clk / 64              |
| 1    | 0    | 0    | clk / 256             |
| 1    | 0    | 1    | clk / 1024            |

The prescaled frequency feeds the timer, so the effective resolution is `prescale / f_clock`.

Maximum time with each prescaler at 20 MHz:

| Prescaler | Max time    |
| --------- | ----------- |
| 1         | 3.28 ms     |
| 8         | 26.2 ms     |
| 64        | 209 ms      |
| 256       | 838 ms      |
| 1024      | 3.36 s      |

---

## Output-Compare Mode (CTC)

Output-compare mode (also called Clear Timer on Compare, CTC) counts from 0 up to the value stored in OCR1A, resets to 0 in hardware at the next clock edge, and optionally triggers an interrupt. There is no software reload, so timing is exact.

### CTC time calculation

```text
time = (OCR1A + 1) / (f_clock / prescaler)
```

Rearranging for the compare value:

```text
OCR1A = (f_clock / prescaler × time) - 1
```

Example: 250 µs at 20 MHz with no prescaler:

```text
OCR1A = (20000000 × 0.000250) - 1 = 5000 - 1 = 4999
```

Example: 500 ms at 20 MHz, choose prescaler 256:

```text
OCR1A = (20000000 / 256 × 0.500) - 1 = 39062 - 1 = 39061
```

Check: 39061 < 65535, so prescaler 256 works. (With prescaler 64 the result would exceed 65535 and the next prescaler up must be used.)

### CTC mode code

```c
#include <avr/io.h>
#include <avr/interrupt.h>

#define RED_LED  PB0
#define COMPARE  4999           // 250 µs at 20 MHz, no prescaler

int main(void)
{
    DDRB |= (1<<RED_LED);

    OCR1A  = COMPARE;           // Set the compare target
    TCCR1B = (1<<WGM12) | (1<<CS10);   // CTC mode, no prescaler
    TIMSK1 = (1<<OCIE1A);       // Enable compare A interrupt

    sei();

    while (1) { }
}

ISR(TIMER1_COMPA_vect)
{
    PORTB ^= (1<<RED_LED);      // Toggle LED every 250 µs
}
```

`WGM12` in TCCR1B selects CTC mode. `CS10` starts the timer with no prescaler.

### CTC interrupt vectors

| Interrupt       | Vector name          |
| --------------- | -------------------- |
| Timer 1 compare A | `TIMER1_COMPA_vect` |
| Timer 1 compare B | `TIMER1_COMPB_vect` |
| Timer 1 overflow  | `TIMER1_OVF_vect`   |

---

## XOR for Toggling

The exclusive-OR compound assignment `^=` toggles specific bits. XORing a bit with 1 flips its state; XORing with 0 leaves it unchanged:

```c
PORTB ^= (1<<PB0);      // Toggle PB0 every time this line executes
```

XOR truth table for one bit:

| Current | Mask | Result |
| ------- | ---- | ------ |
| 0 | 1 | 1 |
| 1 | 1 | 0 |
| 0 | 0 | 0 |
| 1 | 0 | 1 |

---

## Choosing Between Overflow and CTC

| Feature           | Overflow mode             | CTC mode                       |
| ----------------- | ------------------------- | ------------------------------ |
| Reload mechanism  | Software (ISR)            | Hardware (automatic)           |
| Timing accuracy   | Slight jitter from ISR    | Exact                          |
| Setup complexity  | Simpler                   | Slightly more registers to set |
| Typical use       | Learning, rough timing    | Precise periodic events        |

For any timing requirement that needs to be repeatable and accurate, use CTC mode.

---

## Practical Checklist

1. Calculate OCR1A before writing any code.
2. If OCR1A exceeds 65535, move to the next larger prescaler and recalculate.
3. Set OCR1A before starting the timer (before setting CS bits).
4. Enable the specific interrupt in TIMSK1 and call `sei()`.
5. In overflow mode, write TCNT1 as the first line of the ISR.
