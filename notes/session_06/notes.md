# Session 6 - Advanced State Machine Modes

Source: AVR project code and lab progression in this repo.
Adapted for: ATmega644P nine-mode controller in the full build.

---

## Session Aim

Expand the basic state machine into richer behaviours while preserving clean structure.

Learning outcomes:

1. Scale from four modes to nine modes.
2. Split complex behaviour into dedicated functions.
3. Keep transitions and user input handling consistent.
4. Validate each mode independently.

---

## Nine-Mode Layout

| Mode ID | Mode name      | Key concept                       |
| ------- | -------------- | --------------------------------- |
| 0       | Chase          | Sequential LED output             |
| 1       | Blink All      | Group output timing               |
| 2       | Alternate      | Pattern grouping                  |
| 3       | PWM Fade       | Software PWM brightness control   |
| 4       | Knight Rider   | Bidirectional sweep logic         |
| 5       | Binary Counter | Integer to bit-pattern mapping    |
| 6       | Random         | Pseudo-random LED patterns        |
| 7       | Reaction Game  | Event timing and user interaction |
| 8       | Tetris Melody  | Audio playback with LED sync      |

---

## Function-Based Mode Structure

Each advanced mode is implemented in a focused function:

- `pwm_fade()`
- `knight_rider()`
- `binary_counter()`
- `random_mode()`
- `reaction_game()`
- `music_mode()`

This keeps `main()` readable and lowers coupling between behaviours.

---

## Transition Consistency

Mode switching still uses the same interrupt pattern:

1. Debounce and confirm button press.
2. Increment `current_mode` with wraparound.
3. Clear LEDs.
4. Provide short buzzer confirmation.

This consistency keeps user interaction predictable even as features grow.

---

## Validation Strategy

For each mode:

1. Enter mode via button presses.
2. Observe expected LED pattern.
3. Confirm button can exit mode immediately.
4. Confirm no lockups or stale outputs.

For full system:

1. Step through all nine modes in order.
2. Confirm wrap from mode 8 back to mode 0.
3. Confirm startup animation runs only at power-up.

---

## Linked Project Files

- `projects/06_state_machine/06_state_machine.c`
- `platformio/src/06_state_machine.c`
