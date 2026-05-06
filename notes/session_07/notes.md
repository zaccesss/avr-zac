# Session 7 - Analogue to Digital Conversion

---

## What is the ADC

The Analogue to Digital Converter (ADC) on the ATmega644P samples an analogue voltage on one of eight input pins (PA0 to PA7) and produces a 10-bit digital result. The result ranges from 0 (0V) to 1023 (VREF).

The ADC is used whenever the programme needs to measure a physical quantity that varies continuously, such as temperature, light level, position or sound.

---

## ADC Pins

The ADC inputs are on PORTA (J3 on the PCB):

| ADC channel | Pin | J3 header |
| ----------- | --- | --------- |
| ADC0        | PA0 | Pin 2     |
| ADC1        | PA1 | Pin 3     |
| ADC2        | PA2 | Pin 4     |
| ADC3        | PA3 | Pin 5     |
| ADC4        | PA4 | Pin 6     |
| ADC5        | PA5 | Pin 7     |
| ADC6        | PA6 | Pin 8     |
| ADC7        | PA7 | Pin 9     |

For input signals, connect the analogue source (potentiometer, sensor or signal generator) between an ADC pin and GND. VCC and GND are available on J3.

---

## ADC Reference Voltage

The reference voltage (VREF) sets the upper limit of the measurement range. JP1 fitted (default) connects AVCC (the analogue supply, filtered through R2 and C3) as the reference. At 5V this gives:

```text
resolution = VREF / 1024 = 5 / 1024 = 4.88 mV per step
```

Do not remove JP1 unless deliberately switching to an external reference on J2.

---

## Key ADC Registers

| Register | Purpose                                                 |
| -------- | ------------------------------------------------------- |
| `ADMUX`  | Select reference voltage and input channel              |
| `ADCSRA` | Enable ADC, start conversion, set prescaler             |
| `ADCSRB` | Extended control (leave at 0 for single-conversion)     |
| `ADCL`   | Low byte of 10-bit result (read first)                  |
| `ADCH`   | High byte of 10-bit result (read after ADCL)            |
| `DIDR0`  | Disable digital input buffers on ADC pins               |

Reading ADCL before ADCH is important. Reading ADCL locks the register pair; the high byte is then read from the snapshot. If ADCH is read first, the result is incorrect.

In C, the combined 16-bit result is read as:

```c
uint16_t result = ADC;      // Reads ADCL then ADCH automatically
```

---

## ADMUX Register

- **REFS1, REFS0** — reference selection; 0, 1 = AVCC reference (JP1 fitted)
- **ADLAR** — result adjustment; 0 = right-adjust (default), 1 = left-adjust
- **MUX4 to MUX0** — channel select; 00000 = ADC0, 00001 = ADC1, and so on

For AVCC reference on ADC0 (PA0):

```c
ADMUX = (1<<REFS0);         // AVCC reference, channel 0 (MUX bits all 0)
```

For ADC2 (PA2):

```c
ADMUX = (1<<REFS0) | (1<<MUX1);    // AVCC reference, channel 2
```

---

## ADCSRA Register

- **ADEN** — ADC enable; set to 1 to power on the ADC
- **ADSC** — start conversion; set to 1 to begin, hardware clears it when done
- **ADIF** — interrupt flag; set by hardware when conversion completes
- **ADPS2 to ADPS0** — prescaler bits; divide f_clock to produce the ADC clock

The ADC clock must be between 50kHz and 200kHz for full 10-bit accuracy. At 20 MHz:

| Prescaler | ADC clock  | Bits accurate |
| --------- | ---------- | ------------- |
| 16        | 1.25 MHz   | Reduced       |
| 32        | 625 kHz    | Reduced       |
| 64        | 312.5 kHz  | Acceptable    |
| 128       | 156.25 kHz | Full 10-bit   |

Use prescaler 128 (ADPS2=1, ADPS1=1, ADPS0=1) for full accuracy at 20 MHz.

---

## Single-Conversion Function

```c
#include <avr/io.h>

void adc_init(void)
{
    ADMUX  = (1<<REFS0);                            // AVCC reference, ADC0
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  // Enable, prescale 128
    DIDR0  = 0xFF;                                  // Disable digital input on ADC pins
}

uint16_t adc_read(void)
{
    ADCSRA |= (1<<ADSC);                            // Start conversion
    while (ADCSRA & (1<<ADSC));                     // Wait for ADSC to clear
    return ADC;                                     // Return 10-bit result
}
```

The `while` loop waits until the hardware clears ADSC, which signals completion. A conversion takes 13 ADC clock cycles. At 156.25kHz that is approximately 83 µs.

---

## Changing the ADC Channel

To read a different channel without reinitialising, change only the MUX bits in ADMUX:

```c
ADMUX = (ADMUX & 0xE0) | channel;  // Preserve upper 3 bits, set channel
```

Wait one dummy conversion after changing channels before trusting the result, as the input multiplexer takes time to settle.

---

## Using the ADC Result

### LED feedback based on ADC reading

```c
uint16_t reading = adc_read();

if (reading < 256)
{
    PORTB = 0x00;                   // All LEDs off (0 to 255)
}
else if (reading < 512)
{
    PORTB = (1<<PB0);               // Red only (256 to 511)
}
else if (reading < 768)
{
    PORTB = (1<<PB0) | (1<<PB3);    // Red and green (512 to 767)
}
else
{
    PORTB = 0x1F;                   // All LEDs on (768 to 1023)
}
```

### Transmitting ADC readings via UART

Combined with the UART functions from session 6:

```c
char uart_buffer[30];

uint16_t reading = adc_read();
sprintf(uart_buffer, "ADC: %u  (%.2fV)\r\n", reading, reading * 5.0 / 1023.0);
transmit_string(uart_buffer);
```

### ADC noise as a random seed

An unconnected ADC pin floats and picks up electrical noise. Reading it gives a pseudo-random low byte, useful as a seed or random source when a physical potentiometer is not available:

```c
uint8_t get_random(void)
{
    ADMUX  = (1<<REFS0);            // ADC0, unconnected or floating
    ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    while (ADCSRA & (1<<ADSC));
    return ADCL;                    // Low byte only, varies with noise
}
```

This technique is already used in the state machine project for the random LED mode.

---

## Complete ADC Example

```c
#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

char uart_buffer[30];

void uart_init(void)
{
    UBRR0  = 129;
    UCSR0B = (1<<TXEN0);
}

void transmit_char(char c)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

void transmit_string(char str[])
{
    uint8_t len = strlen(str);
    for(uint8_t i = 0; i < len; i++) transmit_char(str[i]);
}

void adc_init(void)
{
    ADMUX  = (1<<REFS0);
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
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
    DDRB |= 0x1F;       // PB0 to PB4 as outputs

    uart_init();
    adc_init();

    while (1)
    {
        uint16_t reading = adc_read();
        sprintf(uart_buffer, "ADC0: %u\r\n", reading);
        transmit_string(uart_buffer);
        _delay_ms(200);
    }
}
```

---

## Practical Checklist

1. JP1 must be fitted for AVCC reference.
2. Set ADEN before starting a conversion with ADSC.
3. Always read ADCL before ADCH, or use the combined `ADC` macro.
4. Use prescaler 128 for full 10-bit accuracy at 20 MHz.
5. Do not drive an analogue input pin as a digital output.
6. Disable digital input buffers on ADC pins with DIDR0 to reduce noise.
