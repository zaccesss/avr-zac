# Session 5 Lab - Basic State Machine Practice

Hardware target: ATmega644P with five LEDs, button on PD2 and buzzer on PD3.
Code target: four-mode state machine.

## Lab Goal

Validate deterministic mode switching with interrupt-driven transitions.

## Software Setup

1. Select environment `05_state_machine_basic`.
2. Build and upload.

Optional comparison:

- Also test `05_state_machine` if you want to compare file variants.

## Mode Map

1. Mode 0: Chase
2. Mode 1: Blink All
3. Mode 2: Alternate
4. Mode 3: PWM Fade

## Part A - Transition Sequence Test

1. Power cycle board.
2. Observe default mode is Mode 0.
3. Press button once per second.
4. Confirm mode order is 0, 1, 2, 3, then wraps to 0.

Pass criteria:

- No skipped mode.
- No duplicate extra step per single clean press.

## Part B - Feedback Consistency

1. On each press, listen for buzzer confirmation.
2. Confirm LEDs clear briefly on transition.

Pass criteria:

- Exactly one confirmation beep per accepted press.

## Part C - Mode Stability

1. Stay in each mode for 20 seconds.
2. Confirm behaviour remains stable.
3. Press to next mode and repeat.

Pass criteria:

- No frozen output.
- No random reset.

## Fault Isolation

If mode jumps by two steps:

1. Increase debounce delay slightly.
2. Check button mechanical bounce.

If wraparound fails:

1. Confirm modulo expression uses `MODE_COUNT`.
2. Confirm enum order ends with `MODE_COUNT`.

## Lab Record

- Date:
- Environments tested:
- Transition accuracy:
- Beep consistency:
- Notes:
