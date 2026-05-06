# Session 8 - UART Serial Reception

---

## Overview

Session 6 covered UART transmission (AVR to PC). This session covers reception (PC to AVR). With both directions working, the AVR becomes a bidirectional serial device that can receive commands from a PC and respond with data.

The receive path uses an interrupt so the CPU does not need to poll. When a character arrives, the USART0 receive-complete interrupt fires, the ISR reads the character from UDR0 and acts on it immediately.

---

## Enabling the Receiver

The transmitter and receiver are enabled independently via UCSR0B:

```c
void uart_init(void)
{
    UBRR0  = 129;                               // 9600 baud at 20 MHz
    UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);
}
```

- `TXEN0` — enable transmitter
- `RXEN0` — enable receiver
- `RXCIE0` — enable the receive-complete interrupt

The receive interrupt fires automatically each time a full character has been received and is ready in UDR0. Global interrupts must be enabled with `sei()` for the ISR to fire.

---

## The Receive ISR

The interrupt vector for USART0 receive complete is `USART0_RX_vect`:

```c
#include <avr/interrupt.h>

ISR(USART0_RX_vect)
{
    char received = UDR0;       // Read character from receive buffer

    // Process the received character
}
```

UDR0 must be read in the ISR. If it is not read, the receive buffer stays full and subsequent characters are lost. The hardware clears the interrupt flag automatically when UDR0 is read.

---

## Controlling LEDs via Serial Commands

A simple command set uses single characters as commands. Uppercase switches an LED on; lowercase switches it off:

```c
ISR(USART0_RX_vect)
{
    char c = UDR0;

    switch (c)
    {
        case 'R': PORTB |= (1<<PB0);   break;  // Red on
        case 'r': PORTB &= ~(1<<PB0);  break;  // Red off
        case 'G': PORTB |= (1<<PB3);   break;  // Green on
        case 'g': PORTB &= ~(1<<PB3);  break;  // Green off
        case 'B': PORTB |= (1<<PB4);   break;  // Blue on
        case 'b': PORTB &= ~(1<<PB4);  break;  // Blue off
        case 'Y': PORTB |= (1<<PB1);   break;  // Yellow on
        case 'y': PORTB &= ~(1<<PB1);  break;  // Yellow off
        case 'W': PORTB |= (1<<PB2);   break;  // White on
        case 'w': PORTB &= ~(1<<PB2);  break;  // White off
    }
}
```

`switch` is cleaner than a chain of `if/else if` statements when matching a single variable against many fixed values.

---

## Echoing Received Characters

It is good practice to echo received characters back to the terminal so the user can see what was processed:

```c
void transmit_char(char c)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

ISR(USART0_RX_vect)
{
    char c = UDR0;
    transmit_char(c);       // Echo back
    // Process c
}
```

Note that calling `transmit_char()` from inside an ISR is safe as long as the transmit path is not itself interrupt-driven. Since `transmit_char()` uses a polling wait on UDRE0, it will block inside the ISR until the character is sent. Keep this in mind for time-sensitive ISRs.

---

## Combining Reception, ADC and Timer

The three ISR sources can run simultaneously. Each fires independently on its own event:

- `TIMER1_COMPA_vect` — fires at the CTC rate, reads ADC and transmits result
- `USART0_RX_vect` — fires on each received character, controls LEDs

The main loop stays empty. The `volatile` keyword is essential for any variable written in one ISR and read in another or in the main loop:

```c
volatile uint16_t adc_result = 0;

ISR(TIMER1_COMPA_vect)
{
    adc_result = adc_read();
    PORTB ^= (1<<PB4);                          // Toggle blue LED
    sprintf(uart_buffer, "ADC: %u\r\n", adc_result);
    transmit_string(uart_buffer);
}

ISR(USART0_RX_vect)
{
    char c = UDR0;

    if (c == 'A')                               // 'A' triggers immediate ADC read
    {
        adc_result = adc_read();
        sprintf(uart_buffer, "Manual: %u\r\n", adc_result);
        transmit_string(uart_buffer);
    }
}
```

---

## Full Programme Structure

```c
#define F_CPU 20000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

char uart_buffer[30];
volatile uint16_t adc_result = 0;

// --- UART functions ---
void uart_init(void)
{
    UBRR0  = 129;
    UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);
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

// --- ADC functions ---
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

// --- Interrupt handlers ---
ISR(TIMER1_COMPA_vect)
{
    PORTB ^= (1<<PB4);                          // Blue LED heartbeat
}

ISR(USART0_RX_vect)
{
    char c = UDR0;
    switch (c)
    {
        case 'R': PORTB |= (1<<PB0);   break;
        case 'r': PORTB &= ~(1<<PB0);  break;
        case 'G': PORTB |= (1<<PB3);   break;
        case 'g': PORTB &= ~(1<<PB3);  break;
        case 'A':
            adc_result = adc_read();
            sprintf(uart_buffer, "ADC: %u\r\n", adc_result);
            transmit_string(uart_buffer);
            break;
    }
}

// --- Main ---
int main(void)
{
    DDRB |= 0x1F;

    uart_init();
    adc_init();

    OCR1A  = 19530;
    TCCR1B = (1<<WGM12) | (1<<CS12);
    TIMSK1 = (1<<OCIE1A);

    sei();
    while (1) { }
}
```

---

## Practical Checklist

1. Enable RXEN0 and RXCIE0 in UCSR0B alongside TXEN0.
2. Always read UDR0 inside the ISR — failure to do so blocks future receives.
3. Declare shared variables as `volatile`.
4. Call `sei()` after all peripheral initialisation.
5. Keep ISRs short — move heavy processing out via flags if needed.
6. Test with a known-good terminal at 9600 8-N-1 before debugging the AVR code.
