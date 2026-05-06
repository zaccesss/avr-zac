# Session 2 Lab - GPIO Outputs and LED Control

Hardware target: ATmega644P on Richard Reeves AVR PCB 2018.
Programmer: Pololu USB AVR Programmer v2.1 on COM4.
Clock: 20 MHz external crystal.

## Lab Goal

Configure PORTB outputs and verify deterministic LED patterns on PB0 to PB4.

## Safety and Pre-Lab Checks

1. Power off before rewiring.
2. Confirm LED resistors are 220R.
3. Confirm common GND between PCB and breadboard.
4. Confirm VCC is 5V with JP2 open.
5. Confirm LEDs are on J4 pins:

- PB0 red
- PB1 yellow
- PB2 white
- PB3 green
- PB4 blue

## Software Setup

1. Open the `platformio/` folder in VS Code.
2. Select environment `01_blink` from the status bar.
3. Run task `Build and Upload`.
4. Confirm flash succeeds with stk500v2 and COM4.

## Part A - Single LED Timing Check

File: `platformio/src/01_blink.c`

Steps:

1. Verify PB0 is configured as output via DDRB.
2. Set ON and OFF delays to known values, for example 1000 ms and 1000 ms.
3. Build and upload.
4. Observe red LED on PB0.

Expected result:

- LED toggles at stable 1 second ON and 1 second OFF rhythm.
- No other LEDs flicker.

Pass criteria:

- Timing is visually stable for 30 seconds.
- Reflash works on first attempt.

## Part B - Five LED Cycle

1. Select environment `02_led_cycle`.
2. Build and upload.
3. Observe sequence red, yellow, white, green, blue.
4. Confirm each LED is active for about 500 ms.

Expected result:

- Exactly one LED is on at a time.
- Sequence restarts cleanly after blue.

Pass criteria:

- Correct colour order matches wiring table.
- No skipped states over 10 cycles.

## Part C - Timing Variation Exercise

1. Edit delay values to 200 ms.
2. Build and upload.
3. Observe faster cycle.
4. Change to 800 ms.
5. Build and upload again.

Expected result:

- Sequence speed changes proportionally.

Pass criteria:

- Both modified timings run as expected.

## Quick Fault Isolation

If one LED never turns on:

1. Check resistor seating and LED polarity.
2. Check jumper wire to correct J4 pin.
3. Check `DDRB` mask includes the pin bit.
4. Check code is running `02_led_cycle` environment not another one.

If upload times out:

1. Confirm COM4 is correct.
2. Confirm upload uses `-B 10`.
3. Reconnect programmer and retry.

## Lab Record

- Date:
- Environment tested:
- Timing values tested:
- Result summary:
- Issues found:
- Fix applied:
