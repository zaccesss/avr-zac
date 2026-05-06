# Hardware Reference

PCB design: Richard Reeves, Aston University, 2018.
Compatible devices: ATmega164P, ATmega324P, ATmega644P and ATmega1284P.
This setup uses the ATmega644P at 20 MHz.

---

## Board Overview

The PCB integrates a voltage regulator, ISP programming header, four 10-way I/O port headers, a UART header, a reset switch and power input. All 32 I/O pins of the ATmega644P are broken out on headers J3 to J6.

---

## Power Supply

| JP2 state | Output voltage |
| --------- | -------------- |
| Not fitted | 5V            |
| Fitted     | 3.3V          |

DC input enters through J8. The LM317T voltage regulator requires its input to be at least 2V above the output, so a 12V unregulated supply works for both modes.

D1 and D2 provide reverse polarity protection on the input. D3 is the power indicator LED.

---

## ISP Programming Header (J1)

6-way box header. Connects to the Pololu USB AVR Programmer v2.1 using a 6-way ribbon cable. The header must be oriented correctly; a keyed connector prevents reversal.

R3, R4 and R5 (100R each) provide current limiting on the shared ISP lines PB5 (MOSI), PB6 (MISO) and PB7 (SCK). Peripherals connected to those pins will still work during normal operation, but avoid driving them heavily during programming.

---

## UART Header (J7)

4-way Molex KK connector. Brings out USART0 for serial communication with a PC.

The Pololu USB AVR Programmer v2.1 includes a built-in USB-to-UART bridge. When the programmer is connected, Windows creates two COM ports. The lower-numbered port is the programming port (used with `-P COM4`); the higher-numbered port is the UART serial port.

| Wire colour | Signal                                  |
| ----------- | --------------------------------------- |
| Black       | GND                                     |
| Orange      | UART RX into AVR (TXD out of lead)      |
| Yellow      | UART TX out of AVR (RXD into lead)      |
| Red         | VCC from PCB                            |

The header must be installed with the plastic back facing towards J6. Wire the Pololu programmer to J7 with Black to the GND pin, Orange to the PD0 (RXD) side and Yellow to the PD1 (TXD) side.

AREF jumper JP1 must be fitted to use AVCC as the ADC reference. Do not remove JP1 when using the ADC.

---

## Port Headers

| Header | Port  | Pins                              |
| ------ | ----- | --------------------------------- |
| J3     | PORTA | PA0 (ADC0) to PA7 (ADC7), VCC, GND |
| J4     | PORTB | PB0 to PB7, VCC, GND              |
| J5     | PORTC | PC0 to PC7, GND, VCC              |
| J6     | PORTD | GND, PD0 (RXD) to PD7, VCC        |

All headers are 10-way. Check the silkscreen for the exact pin-1 orientation before wiring.

---

## Clock

| Parameter   | Value                        |
| ----------- | ---------------------------- |
| Crystal X1  | 20 MHz (user choice, max 20 MHz) |
| Load caps   | C1 and C2, 22pF, soldered on PCB |
| F_CPU       | 20000000UL                   |

C1 and C2 are on the PCB. No external crystal circuit is needed on the breadboard.

---

## Fuse Settings for ATmega644P at 20 MHz

| Fuse     | Value  | Meaning                                                              |
| -------- | ------ | -------------------------------------------------------------------- |
| LOW      | `0xFF` | External crystal, full-swing oscillator, no clock divide, no output  |
| HIGH     | `0xD1` | SPI enabled, JTAG disabled, watchdog off, EEPROM not preserved       |
| EXTENDED | `0xFF` | Brown-out detection disabled                                         |

Fuse restoration command (use only if fuses are corrupted):

```
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -F -U lfuse:w:0xFF:m -U hfuse:w:0xD1:m -U efuse:w:0xFF:m
```

The `-F` flag overrides the signature check and should only be used for fuse recovery.

---

## AREF Configuration

JP1 fitted (default): AVCC (filtered through R2 and C3) is the ADC reference.

To use an external voltage reference, remove JP1 and connect the reference to J2.

---

## Memory Specifications by Device

| Device      | Flash (KB) | RAM (KB) | EEPROM (KB) |
| ----------- | ---------- | -------- | ----------- |
| ATmega164P  | 16         | 1        | 0.5         |
| ATmega324P  | 32         | 2        | 1           |
| ATmega644P  | 64         | 4        | 2           |
| ATmega1284P | 128        | 16       | 4           |

---

## Component BOM

| Reference | Value           | Notes                                          |
| --------- | --------------- | ---------------------------------------------- |
| R1        | 100k            | Reset pull-up                                  |
| R2        | 100R            | AVCC/AREF filter                               |
| R3        | 100R            | ISP MOSI current limiting (PB5)                |
| R4        | 100R            | ISP MISO current limiting (PB6)                |
| R5        | 100R            | ISP SCK current limiting (PB7)                 |
| R6        | 240R            | LM317T output resistor                         |
| R7        | 330R            | LM317T ADJ, sets 5V output                     |
| R8        | 390R            | LM317T ADJ, sets 3.3V output                   |
| R9        | 1k              | Power indicator LED current limiting           |
| D1        | 1N4007          | Reverse voltage protection, note polarity       |
| D2        | 1N4007          | Reverse polarity on DC input, note polarity     |
| D3        | LED             | Power indicator, note polarity                  |
| X1        | 20 MHz crystal  | User choice, max 20 MHz for ATmega644P          |
| C1        | 22pF            | Crystal load capacitor                         |
| C2        | 22pF            | Crystal load capacitor                         |
| C3        | 100nF           | AVCC bypass capacitor                          |
| C4        | 10µF            | Bulk decoupling, note polarity                  |
| C5        | 100nF           | Power supply bypass                            |
| SW1       | FSM10JH         | Active low reset switch                        |
| U1        | 40-way IC socket | Solder socket in, insert IC after all soldering |
| U2        | LM317T          | Lies flat, secured with M3x6mm bolt            |

---

## Soldering Order

Solder lowest-profile components first to prevent taller components obstructing the soldering iron.

| Order | Reference | Notes                                      |
| ----- | --------- | ------------------------------------------ |
| 1-9   | R1-R9     | Resistors first                            |
| 10    | D1        | Note polarity                              |
| 11    | D2        | Note polarity                              |
| 12    | X1        | Crystal                                   |
| 13    | U2        | LM317T, lies flat, M3x6mm bolt            |
| 14    | J9        | VCC test pin                               |
| 15    | J10       | GND test pin                               |
| 16    | U1        | 40-way socket, insert IC after all soldering |
| 17-20 | C1-C3, C5 | Capacitors                                |
| 21    | SW1       | Reset switch                               |
| 22    | J2        | AREF 2-way header                          |
| 23-26 | J3-J6     | Port headers                               |
| 27    | J1        | ISP 6-way box header, check orientation    |
| 28-29 | JP1, JP2  | Jumper pins                                |
| 30    | D3        | Power LED, note polarity                   |
| 31    | J8        | DC input terminal block                    |
| 32    | J7        | UART Molex KK, plastic back towards J6     |
| 33    | C4        | 10µF electrolytic, note polarity           |

C4 will explode if installed backwards.

---

## Current Breadboard Wiring

This is a temporary configuration assembled for learning purposes.

| Pin | Header    | Connected to                                   |
| --- | --------- | ---------------------------------------------- |
| PB0 | J4 pin 2  | Red LED via 220R                               |
| PB1 | J4 pin 3  | Yellow LED via 220R                            |
| PB2 | J4 pin 4  | White LED via 220R                             |
| PB3 | J4 pin 5  | Green LED via 220R                             |
| PB4 | J4 pin 6  | Blue LED via 220R                              |
| PD2 | J6 pin 4  | Push button (INT0) with 10kR pull-down to GND  |
| PD3 | J6 pin 5  | Active buzzer positive terminal                |
| VCC | J4 pin 1  | Breadboard positive rail                       |
| GND | J4 pin 10 | Breadboard negative rail                       |

All LED cathodes and the buzzer negative connect to the breadboard GND rail. The button connects between VCC and PD2; the pull-down holds PD2 LOW when the button is open and HIGH when pressed.
