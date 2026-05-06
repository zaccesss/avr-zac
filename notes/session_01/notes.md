# Session 1 - Introduction to Microcontrollers and AVR I/O

---

## Why Microcontrollers

A purely hardware circuit must be physically redesigned to change its behaviour. A microcontroller replaces that hardware complexity with code. Changing the behaviour requires only a software edit and a reflash, not a new PCB.

---

## What is a Microcontroller

A microcontroller is a self-contained IC that integrates a CPU core, program memory (flash), data memory (SRAM) and a set of peripherals in one package. This reduces board area, cost and power consumption compared to a discrete microprocessor system.

### AVR CPU Core

| Block                                  | Function                                                         |
| -------------------------------------- | ---------------------------------------------------------------- |
| Flash program memory                   | Stores the compiled programme                                    |
| General-purpose registers (32 x 8-bit) | Working storage whilst executing instructions                    |
| ALU (Arithmetic and Logic Unit)        | Performs all arithmetic, logic and comparison operations         |
| Data SRAM                              | Stores variables and intermediate data when not in the registers |
| Instruction register and decoder       | Fetches instructions from flash and sequences execution          |
| Program counter                        | Tracks which instruction executes next                           |

When the compiler generates code for `Z = X + Y`, the sequence is:

1. X and Y are loaded from SRAM into general-purpose registers.
2. The ALU adds them.
3. The result is placed into a register.
4. Z is written back to SRAM.

The CPU core is a black box from the C programmer's perspective. The avr-gcc compiler generates the correct instruction sequences automatically.

---

## Peripherals on the ATmega644P

| Peripheral            | Description                                    |
| --------------------- | ---------------------------------------------- |
| General I/O           | Four 8-bit ports (PORTA, PORTB, PORTC, PORTD)  |
| USART0 / USART1       | Serial communication (UART)                    |
| Timer/Counter 0, 1, 2 | 8-bit and 16-bit timers for PWM and timing     |
| I2C (TWI)             | Two-wire serial interface                      |
| SPI                   | Four-wire serial interface                     |
| ADC                   | 8-channel 10-bit analogue to digital converter |
| EEPROM                | 2KB non-volatile data storage                  |

Most peripherals share physical pins with the general I/O ports. Only one function can be active on a shared pin at a time.

---

## AVR I/O Ports

Each port has three 8-bit registers:

| Register | Full name               | Purpose                                           |
| -------- | ----------------------- | ------------------------------------------------- |
| `DDRn`   | Data Direction Register | 1 = output, 0 = input for each bit                |
| `PORTn`  | Port output register    | Sets the output level when the pin is an output   |
| `PINn`   | Port input register     | Reads the current logic level on the physical pin |

`PINn` does not mean a single pin. It means Port INput; PINB is eight bits wide.

At reset all DDRn registers are 0x00, so all pins default to inputs.

### Setting Pin Direction

To make bits 0, 1, 2, 3 and 4 of PORTB outputs (LEDs) and leave bits 5, 6, 7 as inputs:

| DDRB bit  | 7     | 6     | 5     | 4      | 3      | 2      | 1      | 0      |
| --------- | ----- | ----- | ----- | ------ | ------ | ------ | ------ | ------ |
| Direction | Input | Input | Input | Output | Output | Output | Output | Output |
| Bit value | 0     | 0     | 0     | 1      | 1      | 1      | 1      | 1      |

`DDRB = 0b00011111;` or equivalently `DDRB = 0x1F;`

Using bit-shift notation (preferred because it is self-documenting):

```c
DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4);
```

### Signal Path

When `DDRn` bit = 0 (input): the output driver is disconnected; the physical pin floats or is driven externally; PINn reads the external voltage.

When `DDRn` bit = 1 (output): the PORTn value is driven to the physical pin; PINn also reflects the driven value.

---

## Current Hardware Pin Assignment

The Richard Reeves AVR PCB 2018 with ATmega644P breaks all I/O out on 10-way headers. The components listed below are a temporary breadboard configuration assembled for learning purposes.

| Pin | Header   | Connected to                                  |
| --- | -------- | --------------------------------------------- |
| PB0 | J4 pin 2 | Red LED via 220R                              |
| PB1 | J4 pin 3 | Yellow LED via 220R                           |
| PB2 | J4 pin 4 | White LED via 220R                            |
| PB3 | J4 pin 5 | Green LED via 220R                            |
| PB4 | J4 pin 6 | Blue LED via 220R                             |
| PD2 | J6 pin 4 | Push button (INT0) with 10kR pull-down to GND |
| PD3 | J6 pin 5 | Active buzzer positive terminal               |

---

## Initial C AVR Programme Structure

### Required preamble

Always define `F_CPU` before any includes, as delay libraries use it to calculate accurate times:

```c
#define F_CPU 20E6          // 20 MHz external crystal on this PCB
#include <avr/io.h>         // AVR register and bit name definitions
#include <util/delay.h>     // _delay_ms() and _delay_us()
```

`#define` is a preprocessor directive. Every occurrence of `F_CPU` is replaced with `20000000` before compilation.

`avr/io.h` maps register names (`DDRB`, `PORTB`, `PINB`, `PB0`, etc.) to their memory addresses so you can refer to them by the names used in the datasheet.

In PlatformIO, `F_CPU` is injected via build flags in platformio.ini so the `#define` is not needed in source files when building through PlatformIO. It is still needed when building in Microchip Studio.

### Programme skeleton

```c
#define F_CPU 20E6
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // Initialisation - runs once after reset
    DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3)|(1<<PB4); // Set PB0-PB4 as outputs for the five LEDs

    // Main loop - must never exit
    while (1)
    {
        // Repeat forever
    }
}
```

`while(1)` loops because 1 is always true. The CPU must never reach the closing brace of `main()`; on a microcontroller that causes a crash or reset.

### Setting and clearing individual bits

```c
PORTB |= (1<<PB0);              // Set PB0 HIGH (red LED on)
PORTB &= ~(1<<PB0);             // Set PB0 LOW (red LED off)
PORTB ^= (1<<PB0);              // Toggle PB0

if (PIND & (1<<PD2)) { }        // Read button on PD2
```

The bit-shift `(1<<PBx)` creates a mask with only bit x set. OR-assign sets the bit without disturbing others. AND-assign with the inverted mask clears the bit without disturbing others.

### Blink example

```c
#define F_CPU 20E6
#include <avr/io.h>
#include <util/delay.h>

#define Blue_LED PB4

int main(void)
{
    DDRB = (1<<Blue_LED);   // Set the blue LED pin as an output

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

## Peripheral Registers as Variables

Peripheral registers such as PORTB and PIND are variables. They can be copied like any other variable:

```c
unsigned char temp;
temp = PIND;    // Read Port D input register into temp
PORTB = temp;   // Write temp to Port B output register
```

The expression `PORTB = PIND;` does the same thing in one line.

---

## Building and Flashing

### PlatformIO (VS Code)

1. Open the `platformio/` folder in VS Code.
2. Select the active environment from the status bar at the bottom left.
3. **Terminal** then **Run Task** then **Build and Upload** to compile and flash.

### Microchip Studio 7

1. Open the `.atsln` solution file.
2. Right-click the project in Solution Explorer and set as StartUp Project.
3. Click the green play button (Ctrl + Alt + F5) to build and flash in one step.
4. For the Pololu programmer, select STK500 as the tool and ISP as the interface.

Manual avrdude command:

```bash
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -B 10 -V -U flash:w:.pio\build\ATmega644P\firmware.hex:i
```

The `-B 10` flag slows the ISP clock to approximately 50kHz, which is required with the Pololu programmer to avoid timeout errors.

---

## Programme Planning

Always start with a specification before writing code, then translate it into a flowchart. The flowchart defines the logic; code implements it.

Example: "Counter that cycles from 1 to 6, repeating until power is removed."

```text
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
int counter = 0;        // Initialise counter to 0
while (1)
{
    counter++;          // Advance the counter by 1
    if (counter > 6) counter = 1;  // Wrap back to 1 once past 6
}
```

---

## Simulation (Atmel Studio Simulator)

The Atmel Studio built-in simulator runs code without hardware connected.

1. Select the hammer icon in the toolbar.
2. Set **Selected debugger/programmer** to **Simulator**.
3. **Debug** then **Start Debugging and Break** (Alt + F5).
4. Set breakpoints with F9.
5. Step through one line at a time with F11; run to next breakpoint with F5.
6. View port registers via **Debug** then **Windows** then **I/O**.

In the I/O window, an empty box is logic 0 and a filled box is logic 1. Anything that turns red has changed on the most recent step.

Simulation is useful for checking register state and logic flow. It does not simulate real timing or external hardware. Full on-chip debugging with breakpoints on the actual PCB requires the Atmel ICE programmer.

---

## Reference

- [Hardware reference](../general/hardware.md) - PCB connectors, power supply, fuse settings
- [C operators](../general/c_operators.md) - full operator reference and precedence table
- [Wiring reference](../../docs/wiring.md) - current breadboard connections
- [Atmel Studio workflow](../../docs/atmel_studio_workflow.md) - full project setup guide
