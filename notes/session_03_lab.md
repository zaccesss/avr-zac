# Session 3 Lab - Button Inputs with Polling

Hardware target: ATmega644P on Richard Reeves AVR PCB 2018.
Input: PD2 button (INT0 pin used as GPIO input here).
Output: PD3 active buzzer.

## Lab Goal

Read button state from `PIND` and control buzzer while LEDs continue pattern output.

## Wiring Verification

1. Button connected between VCC and PD2.
2. 10k pull-down from PD2 to GND.
3. Buzzer positive to PD3.
4. Buzzer negative to GND.

## Software Setup

1. Open `platformio/` in VS Code.
2. Select environment `03_button_polling`.
3. Build and upload.

## Part A - Input Read Validation

1. Do not press the button.
2. Confirm buzzer is OFF.
3. Press and hold the button.
4. Confirm buzzer is ON continuously.
5. Release the button.
6. Confirm buzzer returns OFF.

Pass criteria:

- Buzzer follows button state every time.

## Part B - Independent Concurrent Behaviour

1. Observe LEDs cycling on PB0 to PB4.
2. Press button during each LED state.
3. Confirm buzzer behaviour does not stop LED cycle.

Pass criteria:

- LED sequence continues while buzzer changes with button input.

## Part C - Polling Limitation Demonstration

1. Tap button very quickly during long `_delay_ms(500)` sections.
2. Count missed taps over 20 attempts.

Expected learning:

- Some brief taps may be missed due to polling plus blocking delays.

## Improvement Exercise

Reduce LED delay to `_delay_ms(150)` and repeat quick taps.

Pass criteria:

- Missed taps reduce compared with 500 ms version.

## Quick Fault Isolation

If buzzer is always ON:

1. Check pull-down resistor to GND.
2. Check button wiring is not shorted.
3. Confirm conditional uses `if (PIND & (1<<BUTTON))`.

If buzzer never turns ON:

1. Check `DDRD |= (1<<PD3)`.
2. Check buzzer polarity.
3. Check PD2 line reaches HIGH when pressed.

## Lab Record

- Date:
- Build environment:
- Missed tap count at 500 ms:
- Missed tap count at 150 ms:
- Notes:
