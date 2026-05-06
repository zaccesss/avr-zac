# Session 2 - GPIO Outputs and LED Control

Source: AVR project code and lab progression in this repo.
Adapted for: ATmega644P, 20 MHz external crystal, Richard Reeves AVR PCB 2018.

---

## Session Aim

Build confidence with digital outputs by driving single and multiple LEDs on PORTB.

Learning outcomes:

1. Configure output pins with DDR registers.
2. Set, clear and toggle output bits safely.
3. Build repeatable LED patterns with clear timing.
4. Understand why bit masks are safer than full register overwrites.

---

## Hardware Mapping Used Here

| Signal | MCU pin     | Use        |
| ------ | ----------- | ---------- |
| PB0    | PORTB bit 0 | Red LED    |
| PB1    | PORTB bit 1 | Yellow LED |
| PB2    | PORTB bit 2 | White LED  |
| PB3    | PORTB bit 3 | Green LED  |
| PB4    | PORTB bit 4 | Blue LED   |

All LEDs are active HIGH in this setup.

---

## Register Refresher

| Register | Purpose                                           |
| -------- | ------------------------------------------------- |
| DDRB     | Data direction for PORTB pins (1 output, 0 input) |
| PORTB    | Output latch for PORTB pins                       |
| PINB     | Reads current logic levels on PORTB pins          |

To make PB0 to PB4 outputs:

```c
DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4);
```

This only sets those bits and leaves other bits unchanged.

---

## Pattern 1 - Single LED Blink

A minimal output test is a blink on one LED.

```c
PORTB |= (1<<PB0);     // LED on
_delay_ms(500);
PORTB &= ~(1<<PB0);    // LED off
_delay_ms(500);
```

Why this matters:

1. Confirms clock and delay timing are sensible.
2. Confirms wiring and resistor path are correct.
3. Confirms your toolchain can build and flash cleanly.

---

## Pattern 2 - Sequential LED Cycle

Move one lit LED across PB0 to PB4 then repeat.

```c
PORTB = (1<<PB0); _delay_ms(300);
PORTB = (1<<PB1); _delay_ms(300);
PORTB = (1<<PB2); _delay_ms(300);
PORTB = (1<<PB3); _delay_ms(300);
PORTB = (1<<PB4); _delay_ms(300);
PORTB = 0x00;     _delay_ms(300);
```

This introduces ordered state changes and basic animation logic.

---

## Bitwise Operators Used Most

| Operator | Meaning    | Typical GPIO use              |
| -------- | ---------- | ----------------------------- | -------------------------- |
| `        | `          | OR                            | Combine multiple pin masks |
| `&`      | AND        | Keep only selected bits       |
| `~`      | NOT        | Invert mask for clearing bits |
| `<<`     | Left shift | Build a bit mask for a pin    |

Examples:

```c
PORTB |= (1<<PB2);      // Set PB2
PORTB &= ~(1<<PB2);     // Clear PB2
PORTB ^= (1<<PB2);      // Toggle PB2
```

---

## Timing Notes

`_delay_ms()` uses `F_CPU` to calculate delays.

In this repository, PlatformIO injects `F_CPU=20000000UL` via build flags, so source files do not need a local `#define F_CPU` when built through PlatformIO.

---

## Common Mistakes and Fixes

| Issue                | Cause                                     | Fix                       |
| -------------------- | ----------------------------------------- | ------------------------- |
| LED never lights     | Pin still input                           | Set DDR bit to 1          |
| Wrong LED responds   | Pin map mismatch                          | Recheck wiring table      |
| Very fast blinking   | Wrong CPU frequency assumption            | Confirm `F_CPU` is 20 MHz |
| Multiple LEDs glitch | Full register overwrite in mixed-use port | Use set and clear masks   |

---

## Practical Checklist

1. Build the active environment.
2. Flash and verify one LED blink.
3. Flash and verify full five-LED cycle.
4. Change delays and observe timing effect.
5. Confirm no warnings are ignored in build output.

---

## Linked Project Files

- `projects/01_blink/01_blink.c`
- `projects/02_led_cycle/02_led_cycle.c`
- `platformio/src/01_blink.c`
- `platformio/src/02_led_cycle.c`
