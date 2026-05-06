# Session 6 Lab - Advanced State Machine Modes

Hardware target: ATmega644P with PB0 to PB4 LEDs, PD2 button and PD3 buzzer.
Code target: nine-mode full build.

## Lab Goal

Validate all advanced modes and ensure reliable transitions under load.

## Software Setup

1. Select environment `06_state_machine`.
2. Build and upload.
3. Confirm startup animation executes once.

## Mode Checklist

0. Chase
1. Blink All
2. Alternate
3. PWM Fade
4. Knight Rider
5. Binary Counter
6. Random
7. Reaction Game
8. Tetris Melody

## Part A - Full Walkthrough Test

1. Start in mode 0.
2. Press button once to move to next mode.
3. Record observed behaviour at each mode.
4. Continue until mode 8.
5. Press once more and confirm wrap back to mode 0.

Pass criteria:

- Every mode is reachable.
- Wraparound works correctly.

## Part B - Transition Stress Test

1. Press button rapidly but cleanly through many transitions.
2. Confirm no lockup and no stuck LED output.
3. Confirm each transition gives expected clear state and beep.

Pass criteria:

- System remains responsive for at least 30 transitions.

## Part C - Long-Run Stability

1. Leave mode 4 running for 2 minutes.
2. Leave mode 5 running for 2 minutes.
3. Leave mode 6 running for 2 minutes.
4. Confirm stable operation and no reset.

Pass criteria:

- Continuous operation remains stable.

## Fault Isolation

If a mode cannot be exited:

1. Check ISR still sets mode on button press.
2. Check debounce is not too long.
3. Confirm PD2 line transitions correctly.

If random mode looks repetitive:

1. Recheck ADC read path.
2. Confirm input source for noise is not hard-driven.

## Lab Record

- Date:
- Number of successful transitions:
- Modes with issues:
- Stability test notes:
- Fixes applied:
