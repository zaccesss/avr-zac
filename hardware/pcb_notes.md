# AVR Project PCB Full Reference

PCB designed by Richard Reeves, Aston University, 2018.
Source documents: AVR_PCB_2019.pdf, schematic circuit notes and soldering reference.

---

## Overview

The AVR Project PCB is a general-purpose AVR development board designed for use at
Aston University. It supports four DIP-40 ATmega variants and breaks
all 32 I/O pins out on 10-way headers.

**Compatible devices:** ATmega164P, ATmega324P, ATmega644P and ATmega1284P.

---

## Memory Specifications

| Device      | Flash (KB) | RAM (KB) | EEPROM (KB) |
| ----------- | ---------- | -------- | ----------- |
| ATmega164P  | 16         | 1        | 0.5         |
| ATmega324P  | 32         | 2        | 1           |
| ATmega644P  | 64         | 4        | 2           |
| ATmega1284P | 128        | 16       | 4           |

---

## Connectors and Headers

| Reference | Type                 | Function                                                               |
| --------- | -------------------- | ---------------------------------------------------------------------- |
| J1        | 6-way box header     | AVR ISP programming header; connects to Pololu USB AVR Programmer v2.1 |
| J2        | 2-way header         | External ADC reference (AREF); remove JP1 and connect here             |
| J3        | 10-way header        | PORTA (PA0 to PA7, VCC, GND)                                           |
| J4        | 10-way header        | PORTB (PB0 to PB7, VCC, GND)                                           |
| J5        | 10-way header        | PORTC (PC0 to PC7, VCC, GND)                                           |
| J6        | 10-way header        | PORTD (PD0 to PD7, VCC, GND)                                           |
| J7        | 4-way Molex KK       | UART0 serial header; Black=GND, Orange=RXD, Yellow=TXD, Red=VCC        |
| J8        | 2-way terminal block | DC power input; unregulated 12V                                        |
| J9        | Test pin             | VCC test point                                                         |
| J10       | Test pin             | GND (0V) test point                                                    |
| JP1       | 2-way jumper         | AREF jumper; fit to use AVCC as ADC reference (default)                |
| JP2       | 2-way jumper         | VCC jumper; fit for 3.3V, leave open for 5V                            |

---

## Component Bill of Materials

| Reference | Value            | Category          | Notes                                                    |
| --------- | ---------------- | ----------------- | -------------------------------------------------------- |
| R1        | 100k             | Resistor          | Reset pull-up                                            |
| R2        | 100R             | Resistor          | AVCC/AREF filter                                         |
| R3        | 100R             | Resistor          | ISP MOSI current limiting on PB5                         |
| R4        | 100R             | Resistor          | ISP MISO current limiting on PB6                         |
| R5        | 100R             | Resistor          | ISP SCK current limiting on PB7                          |
| R6        | 240R             | Resistor          | LM317T output resistor                                   |
| R7        | 330R             | Resistor          | LM317T ADJ; sets 5V output                               |
| R8        | 390R             | Resistor          | LM317T ADJ; sets 3.3V output                             |
| R9        | 1k               | Resistor          | Power indicator LED current limiting                     |
| D1        | 1N4007           | Diode             | Reverse voltage protection; note polarity                |
| D2        | 1N4007           | Diode             | Reverse voltage protection on DC input; note polarity    |
| D3        | LED (R, Y or G)  | Diode             | Power indicator; note polarity                           |
| X1        | User choice      | Crystal           | Clock source; maximum 20MHz for ATmega644P               |
| C1        | 22p              | Capacitor         | Crystal load capacitor                                   |
| C2        | 22p              | Capacitor         | Crystal load capacitor                                   |
| C3        | 100n             | Capacitor         | AVCC bypass capacitor                                    |
| C4        | 10u              | Capacitor         | Bulk decoupling; note polarity (may explode if reversed) |
| C5        | 100n             | Capacitor         | Power supply bypass                                      |
| SW1       | FSM10JH          | Reset switch      | Active low reset                                         |
| U1        | 40-way IC socket | IC socket         | Solder socket in; insert IC after all soldering complete |
| U2        | LM317T           | Voltage regulator | Lies flat; secured with M3x6mm bolt                      |

---

## Soldering Order

Solder components in order of ascending height (lowest profile first) to keep the board flat
during soldering. Solder resistors and diodes first, then capacitors, then headers and connectors.

| Order | Reference | Value                 | Notes                                   |
| ----- | --------- | --------------------- | --------------------------------------- |
| 1     | R1        | 100k                  |                                         |
| 2     | R2        | 100R                  |                                         |
| 3     | R3        | 100R                  |                                         |
| 4     | R4        | 100R                  |                                         |
| 5     | R5        | 100R                  |                                         |
| 6     | R6        | 240R                  |                                         |
| 7     | R7        | 330R                  |                                         |
| 8     | R8        | 390R                  |                                         |
| 9     | R9        | 1k                    |                                         |
| 10    | D1        | 1N4007                | Note polarity                           |
| 11    | D2        | 1N4007                | Note polarity                           |
| 12    | X1        | Crystal               | User choice; 20MHz fitted here          |
| 13    | U2        | LM317T                | Lies flat; secured with M3x6mm bolt     |
| 14    | J9        | VCC test pin          |                                         |
| 15    | J10       | 0V test pin           |                                         |
| 16    | U1        | 40-way IC socket      | Solder the socket, not the IC           |
| 17    | C1        | 22p                   |                                         |
| 18    | C2        | 22p                   |                                         |
| 19    | C3        | 100n                  |                                         |
| 20    | C5        | 100n                  |                                         |
| 21    | SW1       | FSM10JH               |                                         |
| 22    | J2        | 2-way header          | AREF                                    |
| 23    | J3        | 10-way header         | PORTA                                   |
| 24    | J4        | 10-way header         | PORTB                                   |
| 25    | J5        | 10-way header         | PORTC                                   |
| 26    | J6        | 10-way header         | PORTD                                   |
| 27    | J1        | 6-way box header      | ISP; ensure correct orientation         |
| 28    | JP1       | 2-way jumper pins     | AREF                                    |
| 29    | JP2       | 2-way jumper pins     | VCC                                     |
| 30    | D3        | LED                   | Note polarity                           |
| 31    | J8        | 2-way terminal block  | DC input                                |
| 32    | J7        | 4-way Molex KK header | UART0; plastic back towards J6          |
| 33    | C4        | 10u                   | Note polarity; will explode if reversed |

---

## Power Supply Circuit

The LM317T (U2) is an adjustable voltage regulator. Output voltage is set by R6 (240Ω) and
the jumper JP2, which selects between R7 (330Ω) for 5V and R8 (390Ω) for 3.3V.

| JP2 state           | Output voltage |
| ------------------- | -------------- |
| Not fitted          | 5V             |
| Fitted (R7 shorted) | 3.3V           |

The LM317T input must be at least 2V above the output (dropout voltage), not including the
drop across D2. A 12V input supply is suitable for both modes. D1 and D2 (1N4007) provide
reverse polarity protection on the power rails.

---

## ISP Programming Header (J1)

J1 is a 6-way box header connected to the Pololu USB AVR Programmer v2.1 using the
AVR-ISP-6 connector. PB5 (MOSI), PB6 (MISO) and PB7 (SCK) are shared with the ISP
lines via 100Ω current limiting resistors R3, R4 and R5.

Programming will not work if J1 is soldered in the wrong orientation. If peripherals are on
PB5, PB6 or PB7, the 100Ω resistors provide some isolation but heavily loaded pins may
still cause programming failures.

---

## UART0 Serial Header (J7)

UART0 (PD0 = RXD, PD1 = TXD) is on J7, a 4-way Molex KK connector. The FTDI lead
TTL-232RG-VIP-WE has six wires; only four are needed. Brown and green (flow control) can
be trimmed.

| Wire   | Signal         | Notes                                                   |
| ------ | -------------- | ------------------------------------------------------- |
| Black  | GND            | Common ground                                           |
| Orange | RXD (to AVR)   | Orange = Output from lead                               |
| Yellow | TXD (from AVR) | AVR serial transmit                                     |
| Red    | VCC            | PCB power to FTDI; lead runs at same voltage as circuit |

Molex KK crimps are required on each wire. Crimping tools and housings are kept in Richard
Reeves's office. The latch on the crimp housing locks into place when mated with the header.
Wires push in from above and click into position.

---

## ADC Reference (JP1, J2)

With JP1 fitted, AVCC filtered by R2 (100Ω) and C3 (100nF) is the ADC reference (AREF).
This is the standard lab configuration.

To use an external precision reference, remove JP1 and connect the reference to J2.
