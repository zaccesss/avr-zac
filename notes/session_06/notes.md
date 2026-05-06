# Session 6 - UART Serial Transmission

---

## What is UART

UART (Universal Asynchronous Receiver Transmitter) is a serial communication protocol that transmits data one bit at a time over a single wire. It is asynchronous because there is no shared clock line — both ends agree on the baud rate in advance.

Standard settings for this project are **9600 8-N-1**:

- **9600** — baud rate (bits per second)
- **8** — data bits per frame
- **N** — no parity bit
- **1** — one stop bit

The ATmega644P has two hardware UART peripherals: USART0 and USART1. This session uses USART0, whose TX pin is PD1 and RX pin is PD0. These pins connect to J7 (the Molex KK UART header).

---

## Connecting to a PC

The Pololu USB AVR Programmer v2.1 includes a built-in USB-to-UART bridge. When connected, Windows creates two COM ports:

- The lower-numbered port is for ISP programming (used in `platformio.ini` as the upload port).
- The higher-numbered port is the serial communication port used for UART data.

Connect the UART header J7 to the Pololu programmer:

- Black (GND) — GND
- Orange — RXD into AVR (output from lead)
- Yellow — TXD out of AVR (input to lead)
- Red (VCC) — VCC from PCB

To view received data, use a serial terminal such as Atmel Studio's Data Visualizer or any terminal application set to 9600 8-N-1 on the correct COM port.

---

## Baud Rate Register

The baud rate is set by writing to the 16-bit UBRR0 register (split into UBRR0H and UBRR0L). In C this is accessed as a single 16-bit write:

```text
UBRR0 = (f_clock / (16 x baud)) - 1
```

At 20 MHz, 9600 baud:

```text
UBRR0 = (20000000 / (16 x 9600)) - 1 = 130.2 - 1 = 129
```

Actual baud rate with UBRR0 = 129:

```text
actual = 20000000 / (16 x 130) = 9615 baud   (error: 0.16%, within tolerance)
```

---

## USART0 Registers

| Register | Purpose                                              |
| -------- | ---------------------------------------------------- |
| `UBRR0`  | Baud rate register (16-bit, set once during init)    |
| `UCSR0A` | Status register — check UDRE0 before transmitting    |
| `UCSR0B` | Control register — enable TX, RX and interrupts      |
| `UCSR0C` | Frame format — defaults to 8-N-1 at reset, leave it  |
| `UDR0`   | Data register — write to transmit, read to receive   |

`UCSR0C` is configured correctly for 8-N-1 at reset. Do not write to it.

---

## Initialising USART0 for Transmission

```c
#include <avr/io.h>

void uart_init(void)
{
    UBRR0  = 129;               // 9600 baud at 20 MHz
    UCSR0B = (1<<TXEN0);        // Enable transmitter only
}
```

`TXEN0` enables the transmit path. The pin PD1 (TXD) is automatically controlled by the UART hardware once TXEN0 is set.

---

## Transmitting One Character

Before writing to UDR0, check that the data register is empty (UDRE0 bit in UCSR0A is high). Writing when the register is full corrupts the current transmission:

```c
void transmit_char(char c)
{
    while (!(UCSR0A & (1<<UDRE0)));     // Wait until transmit buffer is empty
    UDR0 = c;                           // Write character to transmit buffer
}
```

This function blocks until the hardware is ready, then queues the character.

---

## Transmitting a String

A string in C is an array of characters terminated by a null byte (value 0). Transmit every character until the null is reached:

```c
#include <string.h>

void transmit_string(char str[])
{
    uint8_t len = strlen(str);
    for(uint8_t i = 0; i < len; i++)
    {
        transmit_char(str[i]);
    }
}
```

Do not modify `transmit_char()` or `transmit_string()` after they are written and tested. Treat them as library functions.

---

## Formatting Strings with sprintf

`sprintf()` formats a string into a character array using printf-style format specifiers. This is the standard way to convert numbers to text for UART output:

```c
#include <stdio.h>

char uart_buffer[30];
int value = 42;

sprintf(uart_buffer, "Value: %d\r\n", value);
transmit_string(uart_buffer);
```

Common format specifiers:

| Specifier | Type    | Example output |
| --------- | ------- | -------------- |
| `%d`      | int     | `42`           |
| `%u`      | uint    | `42`           |
| `%x`      | hex     | `2a`           |
| `%f`      | float   | `3.14`         |
| `%s`      | string  | `hello`        |
| `%c`      | char    | `A`            |

The escape sequences `\r\n` (carriage return, line feed) move the terminal cursor to a new line. Without them, successive transmissions appear on the same line with no gap.

---

## Complete Transmission Example

```c
#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

char uart_buffer[30];
int count = 0;

void transmit_char(char c)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}

void transmit_string(char str[])
{
    uint8_t len = strlen(str);
    for(uint8_t i = 0; i < len; i++)
    {
        transmit_char(str[i]);
    }
}

void uart_init(void)
{
    UBRR0  = 129;
    UCSR0B = (1<<TXEN0);
}

int main(void)
{
    uart_init();

    while (1)
    {
        sprintf(uart_buffer, "Count: %d\r\n", count);
        transmit_string(uart_buffer);
        count++;
        if (count > 15) count = 0;
        _delay_ms(500);
    }
}
```

---

## Timer-Triggered Transmission

Transmitting from the main loop with `_delay_ms()` is simple but inaccurate. A Timer 1 CTC interrupt can trigger transmission at a precise, known interval — combining the techniques from session 4 with the UART from this session.

The ISR increments a counter and formats a string. The main loop stays empty:

```c
#include <avr/interrupt.h>

volatile int adc_result = 0;    // volatile because ISR writes it

ISR(TIMER1_COMPA_vect)
{
    sprintf(uart_buffer, "Reading: %d\r\n", adc_result);
    transmit_string(uart_buffer);
    adc_result++;
    if (adc_result > 15) adc_result = 0;
}
```

The variable must be `volatile` because the ISR writes it and the compiler cannot see that it changes outside the main loop.

---

## Practical Checklist

1. Set UBRR0 before enabling TXEN0.
2. Enable only the peripherals being used — do not enable RXEN0 if not receiving.
3. Always wait for UDRE0 before writing to UDR0.
4. Keep UCSR0C at its default value — do not write to it.
5. Declare variables shared between an ISR and the main loop as `volatile`.
6. Use `\r\n` at the end of each transmitted line for terminal compatibility.
