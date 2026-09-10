# Session 7 Lab - Analogue to Digital Conversion

Hardware: ATmega644P on Richard Reeves AVR PCB 2018.
Programmer: Pololu USB AVR Programmer v2.1 on COM4.
Clock: 20 MHz external crystal.
ADC inputs: PORTA (J3), AVCC reference via JP1.

---

## Lab Goal

Initialise the ADC, perform single-shot conversions and use the result to control LEDs and transmit readings via UART. Combine the ADC with the timer interrupt from session 4 and the UART from session 6.

---

## Wiring for This Lab

Connect a potentiometer to J3:

- Outer leg 1 to VCC (J3 pin 1)
- Outer leg 2 to GND (J3 pin 10)
- Wiper to PA0 (J3 pin 2, ADC0)

If no potentiometer is available, leave PA0 unconnected to use ADC noise as a floating signal. The reading will not be stable but the code will still function.

Confirm JP1 is fitted before powering up.

---

## Pre-Lab: Resolution Calculation

At 5V with a 10-bit ADC:

```text
resolution = 5.000 / 1024 = ____________ mV per step
```

Write the answer here: ____________

---

## Breadboard Setup

This session uses the UART header for output and a potentiometer on PA0 for ADC input. No LEDs are required.

**Components required:**
- 1x 10 kΩ potentiometer
- 4-wire connection from J7 (Molex KK UART header) to the Pololu USB AVR Programmer v2.1 UART port
- JP1 jumper fitted on the PCB

**Potentiometer connections (J3, PORTA/ADC header):**
- Outer leg 1 → VCC (J3 pin 1)
- Outer leg 2 → GND (J3 pin 10)
- Wiper → PA0 (J3 pin 2, ADC0)

**UART connections (J7 Molex KK header to Pololu programmer UART port):**
- Black wire (GND): J7 GND → Pololu programmer GND
- Orange wire: J7 PD0 (RXD into AVR) → Pololu programmer UART TX output
- Yellow wire: J7 PD1 (TXD from AVR) → Pololu programmer UART RX input
- Red wire (VCC): J7 VCC → Pololu programmer VCC

**JP1 must be fitted** to connect AVCC as the ADC reference voltage. Do not remove it.

---

## Task 1 - Initialise the ADC and Read ADC0

Write the `adc_init` and `adc_read` functions:

```c
#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>

void adc_init(void)
{
    ADMUX  = (1<<REFS0);                                            // AVCC ref, ADC0
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);    // Enable, /128
    DIDR0  = 0xFF;
}

uint16_t adc_read(void)
{
    ADCSRA |= (1<<ADSC);
    while (ADCSRA & (1<<ADSC));
    return ADC;
}

int main(void)
{
    DDRB |= 0x1F;
    adc_init();

    while (1)
    {
        uint16_t val = adc_read();
        PORTB = (val >> 7) & 0x1F;     // Map top 3 bits to 5 LEDs roughly
        _delay_ms(100);
    }
}
```

Build and flash. Adjust the potentiometer and observe the LEDs.

Expected result: more LEDs light as the potentiometer moves toward VCC.

Pass criteria: LEDs respond smoothly to the full potentiometer range.

---

## Task 2 - LED Threshold Control

Replace the LED mapping with threshold comparisons matching the ADC ranges:

```c
uint16_t val = adc_read();

if (val < 256)
    PORTB = 0x00;
else if (val < 512)
    PORTB = (1<<PB0);
else if (val < 768)
    PORTB = (1<<PB0) | (1<<PB3);
else
    PORTB = 0x1F;
```

Build and test with the potentiometer.

Pass criteria: four distinct LED states visible across the full potentiometer range.

---

## Task 3 - Transmit ADC Readings via UART

Add the UART functions from session 6 and transmit the ADC result as a formatted string:

```c
#include <stdio.h>
#include <string.h>

char uart_buffer[30];

// uart_init, transmit_char and transmit_string functions here
```

In the while loop:

```c
uint16_t val = adc_read();
sprintf(uart_buffer, "ADC0: %u\r\n", val);
transmit_string(uart_buffer);
_delay_ms(200);
```

Build, connect the terminal at 9600 8-N-1 on the serial COM port and observe.

Expected result: ADC readings appear in the terminal, updating five times per second.

Alter the format string to also display the voltage:

```c
sprintf(uart_buffer, "ADC0: %u  (%.3fV)\r\n", val, val * 5.0 / 1023.0);
```

Pass criteria: both raw count and voltage appear correctly as the potentiometer moves.

---

## Task 4 - Timer-Triggered ADC Transmission

Combine all three sessions: ADC, UART and Timer 1 CTC interrupt. The main loop must be empty.

Structure:

```c
#include <avr/interrupt.h>

volatile uint16_t adc_result = 0;

ISR(TIMER1_COMPA_vect)
{
    adc_result = adc_read();
    sprintf(uart_buffer, "ADC: %u\r\n", adc_result);
    transmit_string(uart_buffer);
}

int main(void)
{
    DDRB |= 0x1F;
    uart_init();
    adc_init();

    OCR1A  = 19530;                             // 250 ms at 20 MHz, /256
    TCCR1B = (1<<WGM12) | (1<<CS12);
    TIMSK1 = (1<<OCIE1A);

    sei();
    while (1) { }
}
```

The while loop in main stays empty. All ADC reads and UART transmits occur in the ISR at the timer rate.

Pass criteria: readings appear in terminal at 250ms intervals with no delays in main.

---

## Quick Fault Isolation

- **ADC always returns 0**: ADEN not set in ADCSRA or REFS0 not set in ADMUX.
- **ADC always returns 1023**: PA0 floating to VCC; confirm potentiometer wiring.
- **No UART output**: check TXEN0 and UBRR0 as in session 6.
- **ISR never fires**: TIMSK1 or sei() missing; confirm both present.
- **Readings jump randomly**: DIDR0 not set; enable it to reduce digital input noise.

---

## Lab Record

- Date:
- Resolution calculated:
- ADC channel used:
- Potentiometer connected (yes / no):
- Tasks completed:
- Voltage formula verified (Task 3):
- Timer interval used in Task 4:
- Issues found:
- Fix applied:
