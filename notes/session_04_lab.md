# Session 4 Lab - External Interrupts with INT0

Hardware target: ATmega644P, button on PD2, buzzer on PD3.

## Lab Goal

Use INT0 to respond to button events without constant polling in the main loop.

## Register Setup to Verify

1. `EICRA` configured for falling edge:

- `ISC01 = 1`
- `ISC00 = 0`

2. `EIMSK` has `INT0` enabled.
3. Global interrupts enabled with `sei()`.

## Software Setup

1. Select environment `04_interrupt_buzzer`.
2. Build and upload.

## Part A - Interrupt Trigger Validation

1. Leave button unpressed.
2. Confirm buzzer state remains idle.
3. Press button and release.
4. Confirm buzzer follows input transitions through ISR logic.

Pass criteria:

- Response is immediate and repeatable.

## Part B - Main Loop Independence

1. Observe LEDs still running sequence.
2. Press button at random times.
3. Confirm ISR action occurs even during LED delays.

Pass criteria:

- Button response is noticeably better than Session 3 polling.

## Part C - Debounce Confirmation

1. Perform slow deliberate press.
2. Perform rapid noisy half-press.
3. Confirm no extra false triggers.

If false triggers occur:

1. Keep debounce delay near 20 ms.
2. Recheck button contacts and wiring.

## Fault Isolation

If interrupt does not fire:

1. Confirm PD2 wiring and pull-down.
2. Confirm `ISR(INT0_vect)` signature exactly matches.
3. Confirm `sei()` is called after setup.
4. Confirm environment `04_interrupt_buzzer` is selected.

## Lab Record

- Date:
- Edge mode configured:
- Debounce value:
- Observed responsiveness vs Session 3:
- Issues and fixes:
