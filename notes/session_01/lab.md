# Session 1 Lab - Introduction Setup and First Programme

Hardware: ATmega644P on Richard Reeves AVR PCB 2018.
Programmer: Pololu USB AVR Programmer v2.1 on COM4.
Clock: 20 MHz external crystal.

---

## Lab Goal

Verify the toolchain, programmer and hardware are all working correctly by building and flashing a known-good programme.

---

## Safety Checks Before Starting

1. Power off before wiring anything.
2. Confirm JP2 is not fitted (5V output).
3. Confirm JP1 is fitted (AVCC as ADC reference).
4. Confirm the LED resistors are 220R and wired to J4.
5. Confirm the ISP ribbon cable is connected between J1 and the Pololu programmer.
6. Confirm common GND between the PCB and breadboard.

---

## Task 1 - Confirm Programmer Is Detected

1. Connect the Pololu programmer to USB.
2. Open Device Manager in Windows.
3. Expand the **Ports (COM and LPT)** section.
4. Confirm two COM ports appear for the Pololu programmer.
5. Note the lower-numbered port (this is COM4, the programming port).

Pass criteria: both COM ports appear without error symbols.

---

## Task 2 - Build and Flash the Blink Programme

1. Open the `platformio/` folder in VS Code.
2. Select environment `01_blink` from the status bar at the bottom left.
3. Run **Terminal** then **Run Task** then **Build and Upload**.
4. Observe the terminal output.

Expected output (final lines):

```text
avrdude: 1 bytes of flash verified
avrdude done. Thank you.
```

Pass criteria: build completes with no errors and the red LED on PB0 blinks at a steady rhythm.

---

## Task 3 - Read the Programme and Explain Each Line

Open [projects/01_blink/01_blink.c](../../projects/01_blink/01_blink.c) and answer the following for each line in main():

| Line                              | What it does |
| --------------------------------- | ------------ |
| `#define F_CPU 20E6`              |              |
| `#include <avr/io.h>`             |              |
| `DDRB = (1<<PB0);`                |              |
| `PORTB \|= (1<<PB0);`             |              |
| `_delay_ms(2000);`                |              |
| `PORTB &= ~(1<<PB0);`             |              |

---

## Task 4 - Modify the Blink Timing

1. Open `platformio/src/01_blink.c`.
2. Change the ON delay to 500ms and the OFF delay to 100ms.
3. Build and upload.
4. Observe the LED.

Expected result: the LED is on for noticeably longer than it is off.

Pass criteria: the visible asymmetric blink matches your delay values.

---

## Task 5 - Test the Five-LED Cycle

1. Select environment `02_led_cycle` from the status bar.
2. Build and upload.
3. Observe the LEDs on PB0 to PB4.

Expected result: one LED lights at a time and the sequence moves from red to yellow to white to green to blue, then repeats.

Pass criteria: correct colour order, no skipped states over 10 cycles.

---

## Quick Fault Isolation

| Symptom                     | Likely cause                         | Fix                                            |
| --------------------------- | ------------------------------------ | ---------------------------------------------- |
| Upload times out            | Wrong COM port or ISP clock too fast | Confirm COM4 and `-B 10` in platformio.ini     |
| No LED activity after flash | DDR bit not set or wiring wrong      | Check DDRB mask and J4 wiring                  |
| Wrong LED lights            | Pin map mismatch                     | Cross-check colour against wiring table        |
| Build error on `F_CPU`      | Missing define in Microchip Studio   | Add `#define F_CPU 20E6` before includes       |

---

## Lab Record

- Date:
- Environment tested:
- Blink timing values used:
- Result:
- Issues found:
- Fix applied:
