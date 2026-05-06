# Session 4 - External Interrupts with INT0

Source: AVR project code and lab progression in this repo.
Adapted for: ATmega644P, INT0 on PD2, buzzer on PD3.

---

## Session Aim

Move from polling to interrupt-driven input handling for faster and cleaner button response.

Learning outcomes:

1. Configure INT0 edge detection.
2. Write and use an ISR safely.
3. Share state between ISR and main loop with `volatile` variables.
4. Add simple debounce for reliable triggering.

---

## Why Interrupts

Polling waits until your loop checks the button again. Interrupts let hardware alert the CPU immediately when an event occurs.

This is ideal when the main loop contains delays or animation logic.

---

## INT0 Setup for Falling Edge

```c
EICRA |= (1<<ISC01);   // Falling edge trigger
EICRA &= ~(1<<ISC00);
EIMSK |= (1<<INT0);    // Enable INT0
sei();                 // Enable global interrupts
```

Falling edge is commonly used when a button press pulls the line from HIGH to LOW.

---

## ISR Structure

```c
ISR(INT0_vect)
{
    // Keep ISR short and deterministic
}
```

In this repo, ISR logic includes:

1. Debounce delay.
2. Button state confirmation.
3. Mode or output update.
4. Optional confirmation beep.

---

## Safe Shared State

Variables modified in ISR and read in main must be `volatile`.

Example:

```c
volatile uint8_t button_pressed = 0;
```

Without `volatile`, compiler optimisation can cache old values and break logic.

---

## Debounce in Interrupt Context

Simple debounce used in project code:

```c
void debounce_delay(void)
{
    _delay_ms(20);
}
```

Then confirm button state after delay before acting.

---

## Good ISR Practices

1. Keep ISR short.
2. Avoid long blocking work inside ISR.
3. Avoid heavy loops in ISR.
4. Use flags to request work in main loop.
5. Clear or reset outputs on state changes where needed.

---

## Practical Checklist

1. Press button and verify immediate response.
2. Confirm behaviour remains stable during LED animations.
3. Confirm no repeated unwanted triggers.
4. Confirm mode-change feedback occurs exactly once per press.

---

## Linked Project Files

- `projects/04_interrupt_buzzer/04_interrupt_buzzer.c`
- `platformio/src/04_interrupt_buzzer.c`
