# Session 1 — Introduction to Microcontrollers and AVR I/O

Source: EE1EPJ Lecture 1 and Lab 1, Richard Reeves, Aston University, January 2024.
Adapted for: ATmega644P, Pololu USB AVR Programmer v2.1, Richard Reeves AVR PCB 2018.

---

## Why Microcontrollers

A purely hardware circuit (op-amps, logic gates, counters) must be physically redesigned to change
its behaviour. A microcontroller replaces that hardware complexity with code. Changing the
behaviour requires only a software edit and a reflash, not a new PCB.

You have already used microcontrollers in the form of the Arduino. This module removes the
Arduino abstraction layer and works directly with the hardware registers.

---

## What is a Microcontroller

A microcontroller is a self-contained IC that integrates a CPU core, program memory (flash), data
memory (SRAM) and a set of peripherals in one package. This reduces board area, cost and power
consumption compared to a discrete microprocessor system.

### AVR CPU Core Components

| Block                                  | Function                                                         |
| -------------------------------------- | ---------------------------------------------------------------- |
| Flash program memory                   | Stores the compiled programme                                    |
| General-purpose registers (32 x 8-bit) | Working storage whilst executing instructions (accumulators)     |
| ALU (Arithmetic and Logic Unit)        | Performs all arithmetic, logic and comparison operations         |
| Data SRAM                              | Stores variables and intermediate data when not in the registers |
| Instruction register and decoder       | Fetches instructions from flash and sequences execution          |
| Program counter                        | Tracks which instruction executes next                           |

When the compiler generates code for `Z = X + Y`, the sequence is:

1. X and Y are read from SRAM into general-purpose registers.
2. The ALU adds them.
3. The result Z is placed into the register that held X.
4. Z is written back to SRAM.

As we write in C, the CPU core is a black box. The avr-gcc compiler generates the correct
instruction sequences automatically.

---

## Peripherals

The ATmega644P integrates the following peripherals alongside the CPU core:

| Peripheral            | Description                                    |
| --------------------- | ---------------------------------------------- |
| General I/O           | Four 8-bit ports (PORTA, PORTB, PORTC, PORTD)  |
| USART0 / USART1       | Serial communication (UART)                    |
| Timer/Counter 0, 1, 2 | 8-bit and 16-bit timers for PWM and timing     |
| I2C (TWI)             | Two-wire serial interface                      |
| SPI                   | Four-wire serial interface                     |
| ADC                   | 8-channel 10-bit analogue to digital converter |
| EEPROM                | 2KB non-volatile data storage                  |

Most peripherals share physical pins with the general I/O ports. Only one function can be active on
a shared pin at a time. The ATmega644P has four full 8-bit ports (PA, PB, PC, PD), which is larger
than the ATmega164P used in the lecture examples — all the theory applies identically.

---

## AVR I/O Ports

Each port has three 8-bit registers:

| Register | Full name               | Purpose                                           |
| -------- | ----------------------- | ------------------------------------------------- |
| `DDRn`   | Data Direction Register | 1 = output, 0 = input for each bit                |
| `PORTn`  | Port output register    | Sets the output level when the pin is an output   |
| `PINn`   | Port input register     | Reads the current logic level on the physical pin |

**PINn does not mean a single pin.** It means Port INput — PINB is eight bits wide.

At reset all DDRn registers are 0x00, so all pins default to inputs.

### Setting Pin Direction

To make bits 0, 1, 2, 3 and 4 of PORTB outputs (LEDs) and leave bits 5, 6, 7 as inputs:

| DDRB bit  | 7     | 6     | 5     | 4      | 3      | 2      | 1      | 0      |
| --------- | ----- | ----- | ----- | ------ | ------ | ------ | ------ | ------ |
| Direction | Input | Input | Input | Output | Output | Output | Output | Output |
| Bit value | 0     | 0     | 0     | 1      | 1      | 1      | 1      | 1      |

`DDRB = 0b00011111;` or equivalently `DDRB = 0x1F;`

Using bit-shift notation (preferred — self-documenting):

```c
DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4);
```

### Signal Path

When `DDRn` bit = 0 (input): the PORTn output driver is disconnected; the physical pin floats or
is driven externally; PINn reads the external voltage.

When `DDRn` bit = 1 (output): the PORTn value is driven to the physical pin; PINn also reflects
the driven value.

---

## Current Hardware Pin Assignment

The Richard Reeves AVR PCB 2018 with ATmega644P breaks all I/O out on 10-way headers. The
components listed below are a **temporary breadboard configuration** assembled for learning
purposes. The LEDs, buzzer and button may be added, removed or repositioned as projects
change. Only the PCB itself (crystal, regulator, ISP header, UART header) is the permanent
fixed hardware.

The breadboard wiring at the start of this course is:

| Pin | Header   | Connected to                                  |
| --- | -------- | --------------------------------------------- |
| PB0 | J4 pin 2 | Red LED via 220Ω                              |
| PB1 | J4 pin 3 | Yellow LED via 220Ω                           |
| PB2 | J4 pin 4 | White LED via 220Ω                            |
| PB3 | J4 pin 5 | Green LED via 220Ω                            |
| PB4 | J4 pin 6 | Blue LED via 220Ω                             |
| PD2 | J6 pin 4 | Push button (INT0) with 10kΩ pull-down to GND |
| PD3 | J6 pin 5 | Active buzzer positive terminal               |

> **Note:** The lecture examples use the ATmega164P with the blue LED on PD4. On this PCB
> the LEDs are on PORTB (PB0 to PB4) and the buzzer is on PD3. Always use the wiring
> table above, not the lecture pin references.

---

## Initial C AVR Programme Structure

### Required preamble

Always define `F_CPU` before any includes, as delay libraries use it to calculate accurate times:

```c
#define F_CPU 20E6          // 20 MHz external crystal on this PCB
#include <avr/io.h>         // AVR register and bit name definitions
#include <util/delay.h>     // _delay_ms() and _delay_us()
```

`#define` is a preprocessor directive. Every occurrence of `F_CPU` is replaced with `20000000`
before compilation. The `#` prefix marks all preprocessor instructions.

`avr/io.h` maps register names (`DDRB`, `PORTB`, `PINB`, `PB0`, etc.) to their memory addresses so
you can refer to them by the names used in the datasheet.

### Programme skeleton

```c
#define F_CPU 20E6
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // Initialisation — runs once after reset
    DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4);  // LEDs as outputs

    // Main loop — must never exit; if main() returns the CPU resets
    while (1)
    {
        // Repeat forever
    }
}
```

`while(1)` loops because 1 is always true (non-zero). The CPU must never reach the closing brace
of `main()` — on a microcontroller that causes a crash or reset.

### Setting and clearing individual bits

```c
PORTB |= (1<<PB0);              // Set PB0 HIGH (red LED on)
PORTB &= ~(1<<PB0);             // Set PB0 LOW (red LED off)
PORTB ^= (1<<PB0);              // Toggle PB0

if (PIND & (1<<PD2)) { ... }    // Read button on PD2
```

The bit-shift `(1<<PBx)` creates a mask with only bit x set. OR-assign sets the bit without
disturbing others. AND-assign with the inverted mask clears the bit without disturbing others.

### Blink example (adapted to this hardware)

The lecture example uses PD4. Adapted for the blue LED on PB4:

```c
#define F_CPU 20E6
#include <avr/io.h>
#include <util/delay.h>

#define Blue_LED PB4

int main(void)
{
    DDRB = (1<<Blue_LED);

    while (1)
    {
        PORTB |= (1<<Blue_LED);     // Blue LED on
        _delay_ms(200);
        PORTB &= ~(1<<Blue_LED);    // Blue LED off
        _delay_ms(200);
    }
}
```

---

## Building and Flashing

### Microchip Studio 7

1. Open `.atsln` solution file.
2. Right-click the project in Solution Explorer and set as StartUp Project.
3. Click the **green play button** (or **Debug** then **Start Without Debugging**, Ctrl + Alt + F5).
   This builds and flashes in a single step — no separate F7 build required.
4. Check the build output at the bottom for errors (red circle) and warnings (yellow triangle). Do not
   ignore warnings.

For the Pololu programmer, select **STK500** as the debugger/programmer and **ISP** as the
interface in the tool selection window (hammer icon). See `docs/atmel_studio_workflow.md` for
the full setup walkthrough.

### PlatformIO (VS Code)

1. Open the `platformio/` folder in VS Code.
2. **Terminal** then **Run Task** then **Build** to compile.
3. **Terminal** then **Run Task** then **Build and Upload** to compile and flash via the Pololu
   programmer on COM4.

Manual avrdude command (if needed):

```
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -B 10 -V -U flash:w:.pio\build\ATmega644P\firmware.hex:i
```

The `-B 10` flag slows the ISP clock to approximately 50kHz, which is required with the Pololu
programmer to avoid timeout errors.

---

## Programme Planning

Always start with a specification before writing code, then translate it into a flowchart. The
flowchart defines the logic; code implements it; the original specification is the test benchmark.

Example: "Counter that cycles from 1 to 6, repeating until power is removed."

```
Start
  |
Counter = 0
  |
  +---> Increment counter
  |          |
  |     Counter > 6?
  |      Y /    \ N
  |  Counter=1   |
  |<_____________|
```

```c
int counter = 0;
while (1)
{
    counter++;
    if (counter > 6) counter = 1;
}
```

Flowcharts are more valuable than they appear — planning before coding avoids wasted flash
cycles and catches logic errors early.

---

## Simulation (Atmel Studio Simulator)

The Atmel Studio built-in simulator runs code without hardware connected. To use it:

1. Select the hammer icon in the toolbar.
2. Set **Selected debugger/programmer** to **Simulator**.
3. **Debug** then **Start Debugging and Break** (Alt + F5).
4. Set breakpoints with **F9** (red circle appears on that line).
5. Step through one line at a time with **F11**; run to next breakpoint with **F5**.
6. View port registers via **Debug** then **Windows** then **I/O**.

In the I/O window, an empty box is logic 0 and a filled box is logic 1. Anything that turns red
has changed on the most recent step.

> Simulation is useful for checking register state and logic flow. It does not simulate real timing
> or external hardware. For full on-chip debugging with breakpoints running on the actual PCB,
> the Atmel ICE programmer (planned for later) is required.

---

## Reference

- [Wiring reference](../docs/wiring.md) — current breadboard connections and header pin tables
- [Hardware notes](../docs/hardware_notes.md) — fuse settings, ISP clock, registers, ADC
- [Atmel Studio workflow](../docs/atmel_studio_workflow.md) — full project setup guide
- [PCB full reference](../hardware/pcb_notes.md) — component list and connector pinout
