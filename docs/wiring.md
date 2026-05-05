# Wiring Reference

AVR Project PCB (Richard Reeves, Aston University, 2018) connected to breadboard components.
MCU: ATmega644P running at 20MHz on a 12V DC supply regulated to 5V by the LM317T.

---

## Breadboard Wiring

| Signal | PCB Header | Pin | Breadboard Component |
|--------|------------|-----|----------------------|
| PB0 | J4 Pin 2 | PORTB0 | Red LED anode via 220Ω |
| PB1 | J4 Pin 3 | PORTB1 | Yellow LED anode via 220Ω |
| PB2 | J4 Pin 4 | PORTB2 | White LED anode via 220Ω |
| PB3 | J4 Pin 5 | PORTB3 | Green LED anode via 220Ω |
| PB4 | J4 Pin 6 | PORTB4 | Blue LED anode via 220Ω |
| PD2 | J6 Pin 4 | PORTD2 (INT0) | Button with 10kΩ pull-down to GND |
| PD3 | J6 Pin 5 | PORTD3 | Active buzzer positive terminal |
| VCC | J4 Pin 1 | VCC | Breadboard positive rail |
| GND | J4 Pin 10 | GND | Breadboard negative rail |

---

## Header Pin Reference

### J4 (PORTB, 10-way)

| Pin | Signal |
|-----|--------|
| 1 | VCC |
| 2 | PB0 |
| 3 | PB1 |
| 4 | PB2 |
| 5 | PB3 |
| 6 | PB4 |
| 7 | PB5 (shared with ISP MOSI via R3 100Ω) |
| 8 | PB6 (shared with ISP MISO via R4 100Ω) |
| 9 | PB7 (shared with ISP SCK via R5 100Ω) |
| 10 | GND |

### J6 (PORTD, 10-way)

| Pin | Signal |
|-----|--------|
| 1 | GND |
| 2 | PD0 (UART0 RXD) |
| 3 | PD1 (UART0 TXD) |
| 4 | PD2 (INT0) |
| 5 | PD3 (INT1) |
| 6 | PD4 |
| 7 | PD5 |
| 8 | PD6 |
| 9 | PD7 |
| 10 | VCC |

### J3 (PORTA, 10-way)

| Pin | Signal |
|-----|--------|
| 1 | VCC |
| 2 | PA0 (ADC0) |
| 3 | PA1 (ADC1) |
| 4 | PA2 (ADC2) |
| 5 | PA3 (ADC3) |
| 6 | PA4 (ADC4) |
| 7 | PA5 (ADC5) |
| 8 | PA6 (ADC6) |
| 9 | PA7 (ADC7) |
| 10 | GND |

### J5 (PORTC, 10-way)

| Pin | Signal |
|-----|--------|
| 1 | GND |
| 2 | PC0 |
| 3 | PC1 |
| 4 | PC2 |
| 5 | PC3 |
| 6 | PC4 |
| 7 | PC5 |
| 8 | PC6 |
| 9 | PC7 |
| 10 | VCC |

---

## Component Values

| Component | Value | Purpose |
|-----------|-------|---------|
| LED resistors | 220Ω | Current limiting for 5 LEDs on PB0 to PB4 |
| Button pull-down | 10kΩ | Pull-down holds PD2 LOW when button is open |
| Buzzer | Active | Driven directly HIGH/LOW by PD3; no oscillator required |

---

## Notes

- All LED cathodes and the buzzer negative terminal connect to the breadboard GND rail.
- The button connects between VCC and PD2. The 10kΩ pull-down holds PD2 LOW when the
  button is open and HIGH when pressed.
- PB5, PB6 and PB7 are shared with ISP programming lines via 100Ω resistors R3, R4 and R5
  on the PCB. Avoid driving these pins heavily when the programmer is connected.
- Do not connect peripherals that sink more than a few milliamps to PB5, PB6 or PB7 during
  programming, as the 100Ω resistors are the only isolation.
