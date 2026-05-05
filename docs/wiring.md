# Wiring Guide

This document provides a complete wiring reference for the AVR project breadboard setup using the ATmega644P microcontroller and custom PCB designed by Richard Reeves.

## Breadboard Wiring Table

| Signal | PCB Header | Pin           | Breadboard Component              |
| ------ | ---------- | ------------- | --------------------------------- |
| PB0    | J4 Pin 2   | PORTB0        | Red LED anode via 220Ω            |
| PB1    | J4 Pin 3   | PORTB1        | Yellow LED anode via 220Ω         |
| PB2    | J4 Pin 4   | PORTB2        | White LED anode via 220Ω          |
| PB3    | J4 Pin 5   | PORTB3        | Green LED anode via 220Ω          |
| PB4    | J4 Pin 6   | PORTB4        | Blue LED anode via 220Ω           |
| PD2    | J6 Pin 4   | PORTD2 (INT0) | Button with 10kΩ pull-down to GND |
| PD3    | J6 Pin 5   | PORTD3        | Active buzzer positive terminal   |
| VCC    | J4 Pin 1   | VCC           | Breadboard positive rail          |
| GND    | J4 Pin 10  | GND           | Breadboard negative rail          |

## Component Details

### LEDs

- All LEDs are 5mm standard brightness red, yellow, white, green and blue
- Each LED has a 220Ω current limiting resistor in series between the LED anode and the PCB header
- LED cathodes connect directly to the breadboard negative (GND) rail

### Button

- Push button (momentary contact) wired to PD2 (INT0 pin)
- 10kΩ pull-down resistor connected between button and GND
- Button output goes HIGH when pressed

### Buzzer

- Active buzzer connected to PD3
- Positive terminal from buzzer to PD3 via 100Ω resistor
- Negative terminal to GND

### Power

- 5V positive rail from PCB J4 Pin 1
- Ground connections from PCB J4 Pin 10 to breadboard GND rail
