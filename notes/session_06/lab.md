# Session 6 Lab - UART Serial Transmission

Hardware: ATmega644P on Richard Reeves AVR PCB 2018.
Programmer: Pololu USB AVR Programmer v2.1 on COM4.
Clock: 20 MHz external crystal.
UART: USART0, PD1 (TX), PD0 (RX), J7 Molex KK header.

---

## Lab Goal

Initialise USART0 and transmit characters, strings and formatted numbers to a PC terminal. Build a reusable `transmit_char` and `transmit_string` function pair that will be used in later sessions.

---

## Pre-Lab: Identify the Serial COM Port

1. Connect the Pololu programmer to USB.
2. Open Device Manager and expand **Ports (COM and LPT)**.
3. Note both COM ports that appear. The lower-numbered is the programming port (COM4). The higher-numbered is the UART serial port.
4. Write the serial COM port number here: ____________

---

## Task 1 - Initialise USART0

Create a new programme. Add the following initialisation:

```c
#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>

void uart_init(void)
{
    UBRR0  = 129;           // 9600 baud at 20 MHz
    UCSR0B = (1<<TXEN0);    // Enable transmitter
}

int main(void)
{
    uart_init();

    while (1) { }
}
```

Build and check there are no errors or warnings. Open Atmel Studio's Data Visualizer (or any terminal at 9600 8-N-1 on the serial COM port) and connect before moving to Task 2.

---

## Task 2 - Transmit a Single Character

Add the `transmit_char` function and send a single character every second:

```c
void transmit_char(char c)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
}
```

In `main`, change the while loop to:

```c
while (1)
{
    UDR0 = '1';
    _delay_ms(1000);
}
```

Build, flash and observe the terminal.

Expected result: the character `1` appears once per second.

Now change the transmitted character to `A`, then `Z`, then `9`. Rebuild each time.

Pass criteria: the correct character appears for each rebuild.

---

## Task 3 - Use transmit_char Instead of Direct UDR0 Write

Replace the direct `UDR0 = '1'` line with `transmit_char('Q')` and remove the 1000ms delay. Rebuild and observe.

Expected result: the terminal fills rapidly with `Q` characters because there is no delay — only the UDRE0 wait between each.

Add a 500ms delay after the `transmit_char` call. Rebuild and confirm the rate drops to two characters per second.

---

## Task 4 - Transmit a String

Add `transmit_string` and `strlen` support:

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

Change the while loop to:

```c
while (1)
{
    transmit_string("Hello\r\n");
    _delay_ms(500);
}
```

Build and observe the terminal.

Expected result: `Hello` appears on a new line twice per second.

Change the string to your name followed by `\r\n`. Rebuild and confirm.

Pass criteria: the string appears at the correct rate with a line break after each instance.

---

## Task 5 - Dynamic String with sprintf

Add `<stdio.h>` to the includes and declare a buffer and counter:

```c
#include <stdio.h>

char uart_buffer[30];
int count = 0;
```

Change the while loop to:

```c
while (1)
{
    sprintf(uart_buffer, "Count: %d\r\n", count);
    transmit_string(uart_buffer);
    count++;
    if (count > 15) count = 0;
    _delay_ms(500);
}
```

Build and observe.

Expected result: `Count: 0` through `Count: 15` appear in sequence, then restart.

Now change the format string to `"Hex: 0x%02X\r\n"` and rebuild. The same values should now appear in two-digit uppercase hexadecimal.

Pass criteria: values count from 0 to 15 in both decimal and hex formats.

---

## Task 6 - Timer-Triggered Transmission

Replace the main loop delay with a Timer 1 CTC interrupt using the techniques from session 4. The while loop must be empty; all transmission happens in the ISR.

Calculate OCR1A for a 500ms interval with prescaler 1024:

```text
OCR1A = ((20000000 / 1024) x 0.500) - 1 = ____________
```

Structure:

```c
#include <avr/interrupt.h>

volatile int count = 0;

ISR(TIMER1_COMPA_vect)
{
    sprintf(uart_buffer, "Count: %d\r\n", count);
    transmit_string(uart_buffer);
    count++;
    if (count > 15) count = 0;
}

int main(void)
{
    uart_init();
    // Timer 1 CTC init here
    sei();

    while (1) { }
}
```

Build and confirm the output rate matches the timer interval.

Pass criteria: count increments at exactly 500ms intervals regardless of the main loop.

---

## Quick Fault Isolation

- **Nothing in terminal** — check the serial COM port number and baud rate (9600 8-N-1).
- **Garbled characters** — baud rate mismatch. Confirm UBRR0 = 129 and terminal is set to 9600.
- **First character always missing** — UDRE0 check missing. Confirm the while loop in `transmit_char`.
- **sprintf produces nothing** — `<stdio.h>` not included or buffer too small.
- **ISR not firing** — TIMSK1 or sei() missing. Check both are present.

---

## Lab Record

- Date:
- Serial COM port used:
- Tasks completed:
- UBRR0 value used:
- OCR1A calculated for Task 6:
- Issues found:
- Fix applied:
