# Session 3 - Inputs, Bit Masking and Interrupts

---

## Digital Inputs

All AVR pins default to inputs at reset (DDRn = 0x00). To read the state of an input pin, read the PINn register.

### Configuring an input pin

```c
DDRD &= ~(1<<PD2);      // Set PD2 as input (clear the direction bit)
```

### Reading an input pin

```c
if (PIND & (1<<PD2))    // True when PD2 is HIGH (button pressed)
{
    PORTD |= (1<<PD3);  // Turn buzzer on
}
```

On this hardware, the button connects between VCC and PD2 with a 10kR pull-down resistor to GND. When the button is open, PD2 is held LOW by the resistor. When the button is pressed, PD2 is pulled HIGH.

---

## Bit Masking on Input Registers

Masking isolates specific bits in a wide register so the rest can be ignored.

### Isolating one bit

```c
uint8_t y = PIND & (1<<PD2);
```

If PD2 is 0, `y` is 0 (false). If PD2 is 1, `y` is non-zero (true).

In C, zero is false and any non-zero value is true. The numerical value of `y` does not matter in a conditional — only whether it is zero or not.

### Inverting a reading with NOT

The tilde `~` operator inverts every bit. Applied to an input:

```c
PORTB = ~PIND;          // PORTB mirrors the opposite state of every PIND bit
```

Combined with AND to isolate and invert one bit:

```c
PORTB = ~PIND & (1<<PD2);
```

This sets bit 2 of PORTB (PB2) when the button is NOT pressed.

### Masking multiple bits

To test whether both PD2 AND PD3 are low simultaneously:

```c
if (!(PIND & ((1<<PD2) | (1<<PD3))))
{
    // Both PD2 and PD3 are low
}
```

---

## Conditional Statements

### if()

```c
if (condition)
{
    // Executes when condition is true (non-zero)
}
```

### if() / else

```c
if (PIND & (1<<PD2))
{
    PORTD |= (1<<PD3);      // Buzzer on
}
else
{
    PORTD &= ~(1<<PD3);     // Buzzer off
}
```

### if() / else if()

Multiple conditions can be chained. Only the first matching block executes:

```c
if (PIND & (1<<PD2))
{
    PORTD |= (1<<PD3);
}
else if (!(PIND & (1<<PD2)))
{
    PORTD &= ~(1<<PD3);
}
```

### Using a mask in a conditional

```c
if (PIND & (1<<PD2))        // True when PD2 is HIGH
if (!(PIND & (1<<PD2)))     // True when PD2 is LOW
```

The logical NOT `!` (exclamation mark) inverts the truth value of the whole expression. It is not the same as the bitwise NOT `~` which inverts individual bits.

---

## Polling

Polling checks the state of an input repeatedly in the main loop. It is the simplest approach but ties up the CPU while waiting:

```c
while (1)
{
    if (PIND & (1<<PD2))
    {
        PORTD |= (1<<PD3);
    }
    else
    {
        PORTD &= ~(1<<PD3);
    }

    PORTB = (1<<PB0);
    _delay_ms(500);
    PORTB = (1<<PB1);
    _delay_ms(500);
}
```

The problem with polling is that a button press occurring during a `_delay_ms()` call may be missed, because the button state is only checked once per loop iteration.

---

## Interrupts

An interrupt pauses the current code, runs a separate handler (the Interrupt Service Routine), then resumes the original code from where it stopped. The CPU does not need to poll; the peripheral raises its hand when it needs attention.

### Why interrupts matter

With polling, the CPU must reach the button-check line in the loop before it can respond. With an interrupt, the button press is responded to immediately regardless of where the CPU is in the main loop.

### Interrupt flow

```text
Main loop running
  -> Interrupt event occurs
  -> CPU saves its state
  -> ISR executes
  -> CPU restores its state
  -> Main loop resumes where it left off
```

### Global interrupt enable

Global interrupts must be enabled with `sei()` before any interrupt can fire:

```c
#include <avr/interrupt.h>

sei();          // Enable global interrupts
cli();          // Disable global interrupts
```

---

## External Interrupts on the ATmega644P

The ATmega644P has three external interrupts: INT0 (PD2), INT1 (PD3) and INT2 (PB2). This setup uses INT0 on the button pin PD2.

### Trigger modes (EICRA register)

The External Interrupt Control Register A selects the event that triggers each interrupt:

| ISCn1 | ISCn0 | Trigger condition        |
| ----- | ----- | ------------------------ |
| 0     | 0     | Low level on INTn        |
| 0     | 1     | Any logic change on INTn |
| 1     | 0     | Falling edge on INTn     |
| 1     | 1     | Rising edge on INTn      |

n = 0, 1 or 2 for INT0, INT1 or INT2.

The button on this hardware produces a falling edge (HIGH to LOW) when pressed. Configure INT0 for falling edge:

```c
EICRA |= (1<<ISC01);    // ISC01=1, ISC00=0 = falling edge on INT0
EICRA &= ~(1<<ISC00);
```

### Enabling the interrupt (EIMSK register)

```c
EIMSK |= (1<<INT0);     // Enable INT0
```

The interrupt must be individually enabled in EIMSK and global interrupts must be enabled via `sei()` before it will fire.

### Writing an ISR

```c
#include <avr/interrupt.h>

ISR(INT0_vect)
{
    // This code runs when INT0 fires
    PORTD ^= (1<<PD3);  // Toggle buzzer
}
```

The ISR name for each external interrupt is fixed:

| Interrupt | Vector name |
| --------- | ----------- |
| INT0      | `INT0_vect` |
| INT1      | `INT1_vect` |
| INT2      | `INT2_vect` |

### Full INT0 setup example

```c
#define F_CPU 20E6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUZZER PD3
#define BUTTON PD2

int main(void)
{
    DDRD |= (1<<BUZZER);
    DDRD &= ~(1<<BUTTON);

    EICRA |= (1<<ISC01);
    EICRA &= ~(1<<ISC00);
    EIMSK |= (1<<INT0);

    sei();

    while (1)
    {
        // Main loop runs freely
    }
}

ISR(INT0_vect)
{
    PORTD ^= (1<<BUZZER);   // Toggle buzzer on each press
}
```

---

## Debounce

Mechanical buttons bounce — the contacts make and break rapidly for a few milliseconds after a press before settling. This can generate multiple interrupt edges from a single physical press.

A simple software debounce adds a short delay after the interrupt and then re-checks the pin:

```c
ISR(INT0_vect)
{
    _delay_ms(20);

    if (!(PIND & (1<<BUTTON)))  // Confirm still pressed
    {
        // Process the button press
    }
}
```

Calling `_delay_ms()` inside an ISR blocks other interrupts for the duration. For production code a timer-based debounce is preferable, but the delay approach is acceptable for learning.

---

## volatile Variables Shared with an ISR

Any variable written in an ISR and read in the main loop (or vice versa) must be declared `volatile`. Without it the compiler may cache the value in a register and never re-read memory, so the main loop never sees the ISR's update:

```c
volatile uint8_t button_pressed = 0;

ISR(INT0_vect)
{
    button_pressed = 1;
}

int main(void)
{
    while (1)
    {
        if (button_pressed)
        {
            button_pressed = 0;
            // Handle event
        }
    }
}
```

---

## Practical Checklist

1. Set DDRn correctly: 0 for input, 1 for output.
2. Enable the specific interrupt in EIMSK and call `sei()`.
3. Configure EICRA before enabling the interrupt in EIMSK.
4. Declare shared variables as `volatile`.
5. Keep ISRs short — move long processing into the main loop via a flag.

---

## Linked Project Files

- [projects/03_button_polling/03_button_polling.c](../../projects/03_button_polling/03_button_polling.c)
- [projects/04_interrupt_buzzer/04_interrupt_buzzer.c](../../projects/04_interrupt_buzzer/04_interrupt_buzzer.c)
- [platformio/src/03_button_polling.c](../../platformio/src/03_button_polling.c)
- [platformio/src/04_interrupt_buzzer.c](../../platformio/src/04_interrupt_buzzer.c)
