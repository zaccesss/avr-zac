# Session 3 Lab - Inputs, Bit Masking and Interrupts

Hardware: ATmega644P on Richard Reeves AVR PCB 2018.
Programmer: Pololu USB AVR Programmer v2.1 on COM4.
Clock: 20 MHz external crystal.

---

## Lab Goal

Read a digital input via polling and via an external interrupt, compare the two approaches and confirm that the interrupt responds even during a blocking delay in the main loop.

---

## Wiring Checks

1. Button connected between VCC and PD2 (J6 pin 4) with 10kR pull-down to GND.
2. Active buzzer positive terminal connected to PD3 (J6 pin 5).
3. Buzzer negative terminal connected to GND rail.
4. Confirm PD2 reads LOW with button open using a meter before powering the board.

---

## Task 1 - Read Port D and Confirm Button State

Select environment `03_button_polling` and open [platformio/src/03_button_polling.c](../../platformio/src/03_button_polling.c).

Trace through the code and answer:

1. Which DDR register configures PD2 as an input?
2. Which register and which bit are read to detect the button?
3. Why does the code use `PIND & (1<<BUTTON)` rather than just `PIND == 1`?

Build and upload. Press and release the button several times.

Expected result: buzzer sounds while button is held, silences on release.

Pass criteria: buzzer follows button state reliably over 20 presses.

---

## Task 2 - Masking the Input

Write a small test that only lights the red LED when the button is pressed, leaving all other LEDs unchanged:

```c
if (PIND & (1<<PD2))
{
    PORTB |= (1<<PB0);      // Red LED on
}
else
{
    PORTB &= ~(1<<PB0);     // Red LED off
}
```

Add this block inside the existing `while(1)` loop and rebuild.

Pass criteria: red LED follows button state; other LEDs are not disturbed.

---

## Task 3 - Polling Limitation

Reduce the LED cycle delay from 500ms to 150ms in `03_button_polling.c` and rebuild. Tap the button quickly several times.

Observation questions:

- Does the buzzer always respond on the first tap?
- How does the response time compare to when you hold the button?

Record your observations. The purpose of this task is to demonstrate the fundamental limitation of polling when the CPU is occupied with other work.

---

## Task 4 - External Interrupt on INT0

Select environment `04_interrupt_buzzer` and open [platformio/src/04_interrupt_buzzer.c](../../platformio/src/04_interrupt_buzzer.c).

Trace through the initialisation code line by line using the notes and the EICRA table:

1. What does `EICRA |= (1<<ISC01)` set INT0 to respond to?
2. What does `EIMSK |= (1<<INT0)` do?
3. What does `sei()` do and why must it come after the peripheral configuration?

Build and upload. Repeat the quick-tap test from Task 3.

Expected result: buzzer responds to the button press immediately, even during the LED cycle delay.

Pass criteria: buzzer response is perceptibly faster than in the polling version.

---

## Task 5 - Conditional Logic Based on Button State

Extend the ISR to implement this behaviour:

| Button action | Buzzer action |
| ------------- | ------------- |
| Pressed       | Toggle on     |
| Released      | Toggle off    |

In the ISR, read `PIND` after the debounce delay and use an `if/else` to drive the buzzer:

```c
ISR(INT0_vect)
{
    _delay_ms(20);

    if (PIND & (1<<BUTTON))
    {
        PORTD |= (1<<BUZZER);
    }
    else
    {
        PORTD &= ~(1<<BUZZER);
    }
}
```

Change `EICRA` to trigger on any logic change (ISC01=0, ISC00=1) so the ISR fires on both press and release. Build and test.

Pass criteria: buzzer tracks button state correctly with any-edge triggering.

---

## Quick Fault Isolation

| Symptom                        | Likely cause                    | Fix                                     |
| ------------------------------ | ------------------------------- | --------------------------------------- |
| Button has no effect at all    | EIMSK or sei() missing          | Check both are present after EICRA      |
| Buzzer fires without pressing  | Pull-down missing or wrong edge | Confirm 10kR to GND and ISC01/ISC00     |
| ISR fires multiple times       | Button bounce                   | Add 20ms debounce delay in ISR          |
| Main loop LEDs freeze          | Long code in ISR                | Move processing to main loop via flag   |

---

## Lab Record

- Date:
- Tasks completed:
- Polling response observations (Task 3):
- Interrupt response observations (Task 4):
- Edge mode used in Task 5:
- Issues found:
- Fix applied:
