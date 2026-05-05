# PCB and Hardware Design Notes

## PCB Overview

The AVR Project PCB is a professional-grade development board designed by Richard Reeves, lab technician at Aston University, in 2018. This board supports a family of ATmega microcontrollers with identical pin configurations.

### Supported Microcontrollers

The PCB accepts the following ATmega processors via DIP-40 socket:
- ATmega164P (16 KB flash, 1 KB SRAM)
- ATmega324P (32 KB flash, 2 KB SRAM)
- ATmega644P (64 KB flash, 4 KB SRAM) - used in this project
- ATmega1284P (128 KB flash, 16 KB SRAM)

All processors run at identical pin configurations, allowing firmware to be compiled for any device.

## Voltage Regulation

The PCB features an LM317T adjustable voltage regulator with the following specifications:
- Input voltage: 12V DC (via 2.1 mm barrel connector)
- Adjustable output: selectable between 5V and 3.3V via jumper JP2
- Current capacity: 1.5 A continuous
- Default: 5V for logic operations

The jumper JP2 bridges pins that change the feedback network resistors, adjusting the reference voltage:
- 5V position: standard logic operations
- 3.3V position: reduced voltage for power-sensitive devices

## ISP Programming

The PCB includes a 6-pin ISP header (J1) connected to:
- MOSI (Master Out Slave In) on PB5
- MISO (Master In Slave Out) on PB6
- SCK (Serial Clock) on PB7
- RESET on the reset line
- 5V and GND for programmer power

The Pololu USB AVR Programmer v2.1 connects via this header using STK500v2 protocol. The connection is isolated by 100Ω series resistors (R3, R4, R5) to prevent signal conflicts during programming.

## I/O Headers

All 32 I/O pins of the ATmega644P are broken out onto four 10-way headers:
- J3: PORTA pins PA0 through PA7
- J4: PORTB pins PB0 through PB7
- J5: PORTC pins PC0 through PC7
- J6: PORTD pins PD0 through PD7

Each header includes dedicated 5V and GND pins for convenient breadboard connections.

## UART Interface

The PCB includes UART0 transmission and reception pins (TX and RX from pins PD1 and PD0) broken out on header J7 for serial debugging and data logging. This allows real-time monitoring of firmware execution via a serial terminal on a computer.

## Crystal Oscillator

A 20MHz external crystal oscillator (Y1) is permanently installed on the PCB. The crystal is buffered through standard AVR crystal load capacitors.
