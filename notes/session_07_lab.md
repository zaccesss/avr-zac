# Session 7 Lab - ADC Noise, Random Patterns and Reaction Timing

Hardware target: ATmega644P, floating or noise-prone ADC source for pseudo-random seed.
Code target: random mode and reaction mode in full state machine build.

## Lab Goal

Test pseudo-random LED output quality and reaction mode correctness using ISR flag logic.

## Software Setup

1. Select environment `06_state_machine`.
2. Build and upload.
3. Step to mode 6 for random test, then mode 7 for reaction test.

## Part A - Random Mode Distribution Check

1. Enter mode 6.
2. Observe 50 consecutive pattern updates.
3. Tally how often each LED appears ON.

Simple table to fill:

| LED | Count ON in 50 updates |
| --- | ---------------------- |
| PB0 |                        |
| PB1 |                        |
| PB2 |                        |
| PB3 |                        |
| PB4 |                        |

Pass criteria:

- No LED is permanently ON or permanently OFF.
- Pattern appears varied to the eye.

## Part B - Reaction Mode Correctness

1. Enter mode 7.
2. Press only when green LED appears.
3. Confirm success feedback.
4. Repeat and intentionally press on wrong colour.
5. Confirm failure feedback.

Pass criteria:

- Success path and failure path are clearly distinct.
- One press triggers one evaluation.

## Part C - Timing Sensitivity Exercise

1. Perform 10 attempts aiming for green only.
2. Record success count.
3. Repeat with quicker reaction approach.
4. Compare counts.

Pass criteria:

- Game outcome correlates with press timing.

## Fault Isolation

If button presses are ignored in reaction mode:

1. Confirm ISR sets `button_pressed` flag.
2. Confirm reaction loop checks and clears flag.
3. Confirm PD2 input state is valid.

If random patterns are fixed:

1. Confirm ADC conversion starts and completes.
2. Confirm returned value is masked and written to PORTB.

## Lab Record

- Date:
- Random mode observations:
- Reaction attempts:
- Success count:
- Faults and fixes:
