# Session 5 - State Machines and Mode Switching

Source: AVR project code and lab progression in this repo.
Adapted for: ATmega644P multi-mode LED controller.

---

## Session Aim

Introduce finite state machine structure to organise multiple behaviours cleanly.

Learning outcomes:

1. Use `enum` to define named modes.
2. Switch modes safely from an interrupt.
3. Implement mode logic with `switch` and dedicated functions.
4. Reset outputs cleanly when changing mode.

---

## Why a State Machine

As features grow, long `if` chains become hard to read and maintain.
A state machine gives each behaviour a clear state and transition path.

---

## Core Pattern

```c
typedef enum {
    MODE_CHASE,
    MODE_BLINK_ALL,
    MODE_ALTERNATE,
    MODE_PWM_FADE,
    MODE_COUNT
} LED_Mode;

volatile LED_Mode current_mode = MODE_CHASE;
```

Main loop dispatch:

```c
switch (current_mode)
{
    case MODE_CHASE:      /* ... */ break;
    case MODE_BLINK_ALL:  /* ... */ break;
    case MODE_ALTERNATE:  /* ... */ break;
    case MODE_PWM_FADE:   /* ... */ break;
    default: current_mode = MODE_CHASE; break;
}
```

---

## Mode Change in ISR

```c
current_mode = (current_mode + 1) % MODE_COUNT;
PORTB = 0x00;
beep();
```

Modulo wrap ensures the mode returns to zero after the last mode.

---

## Four Basic Modes Covered

| Mode      | Behaviour                          |
| --------- | ---------------------------------- |
| Chase     | Single LED steps across PB0 to PB4 |
| Blink All | All LEDs on and off together       |
| Alternate | Odd and even groups alternate      |
| PWM Fade  | Brightness ramps up and down       |

---

## Design Benefits

1. Easy to add new modes.
2. Reduced coupling between mode logic blocks.
3. Better debug visibility because each mode has one clear code path.
4. Safer behaviour transitions when outputs are reset on change.

---

## Practical Checklist

1. Verify button cycles through every mode once per press.
2. Verify wraparound from last mode to first mode.
3. Verify buzzer confirmation on each mode change.
4. Verify outputs clear cleanly during transitions.

---

## Linked Project Files

- `projects/05_state_machine_basic/05_state_machine_basic.c`
- `platformio/src/05_state_machine_basic.c`
- `projects/05_state_machine/05_state_machine.c`
- `platformio/src/05_state_machine.c`
