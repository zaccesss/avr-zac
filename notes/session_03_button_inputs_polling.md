# Session 3 - Digital Inputs and Button Polling

Source: AVR project code and lab progression in this repo.
Adapted for: ATmega644P, INT0 button on PD2, active buzzer on PD3.

---

## Session Aim

Read a button input in software and control an output by polling the input state.

Learning outcomes:

1. Configure input and output pins on different ports.
2. Read pin state using `PIND`.
3. Drive a buzzer from software logic.
4. Understand limitations of polling loops.

---

## Hardware Mapping Used Here

| Signal  | MCU pin        | Use                  |
| ------- | -------------- | -------------------- |
| PD2     | PORTD bit 2    | Push button input    |
| PD3     | PORTD bit 3    | Active buzzer output |
| PB0-PB4 | PORTB bits 0-4 | LED outputs          |

Button is read as HIGH when pressed in this wiring arrangement.

---

## Input Configuration

```c
DDRD &= ~(1<<PD2);    // PD2 as input
DDRD |=  (1<<PD3);    // PD3 as output
```

Read the button:

```c
if (PIND & (1<<PD2)) {
    // Pressed
}
```

`PIND` is sampled repeatedly in the main loop.

---

## Polling Control Pattern

Basic buzzer control logic:

```c
if (PIND & (1<<PD2))
    PORTD |= (1<<PD3);
else
    PORTD &= ~(1<<PD3);
```

This is simple and deterministic, but the CPU must keep checking the pin.

---

## Polling vs Interrupts

| Polling                                  | Interrupts                                |
| ---------------------------------------- | ----------------------------------------- |
| Main loop checks input repeatedly        | Hardware event triggers ISR automatically |
| Easy to understand                       | More scalable for event-driven designs    |
| Can miss short events during long delays | Better for asynchronous inputs            |
| CPU time is consumed checking            | CPU can do other work until event occurs  |

Because LED patterns often use `_delay_ms()`, pure polling may feel unresponsive.

---

## Debounce Consideration

Mechanical buttons bounce for a short time when pressed and released. Polling without debounce can produce repeated false triggers.

Simple software debounce approach:

1. Detect a state change.
2. Wait about 20 ms.
3. Confirm the state is still valid.

---

## Practical Checklist

1. Verify button press enables buzzer.
2. Verify release disables buzzer.
3. Confirm LEDs continue independent patterning.
4. Test quick taps and long holds.
5. Note any bounce behaviour before moving to interrupts.

---

## Linked Project Files

- `projects/03_button_polling/03_button_polling.c`
- `platformio/src/03_button_polling.c`
