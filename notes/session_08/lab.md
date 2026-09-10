# Session 8 Lab - UART Serial Reception

Hardware: ATmega644P on Richard Reeves AVR PCB 2018.
Programmer: Pololu USB AVR Programmer v2.1 on COM4.
Clock: 20 MHz external crystal.
UART: USART0, PD1 (TX), PD0 (RX), J7 Molex KK header.

---

## Lab Goal

Enable the USART0 receiver and use the receive-complete interrupt to control LEDs from single-character commands sent via the PC terminal. Then combine reception with the ADC and timer from sessions 4 and 7.

---

## Pre-Lab Checks

1. UART header J7 wired to Pololu programmer (Black=GND, Orange=RXD, Yellow=TXD, Red=VCC).
2. JP1 fitted for AVCC reference.
3. Serial COM port number noted from Device Manager.
4. Terminal application open at 9600 8-N-1 on the serial COM port.

---

## Breadboard Setup

This session uses UART reception, ADC input and LED outputs together.

**Components required:**
- 1x red LED with a 330 Ω current-limiting resistor (PB0)
- 1x yellow LED with a 330 Ω current-limiting resistor (PB1)
- 1x white LED with a 330 Ω current-limiting resistor (PB2)
- 1x green LED with a 330 Ω current-limiting resistor (PB3)
- 1x blue LED with a 330 Ω current-limiting resistor (PB4)
- 1x 10 kΩ potentiometer
- 4-wire connection from J7 to the Pololu USB AVR Programmer v2.1 UART port
- JP1 jumper fitted on the PCB

**LED connections (J4, PORTB header):**
- J4 pin 1 (VCC) → positive power rail on breadboard
- J4 pin 10 (GND) → negative power rail on breadboard
- J4 pin 2 (PB0) → anode of red LED → cathode → 330 Ω resistor → GND rail
- J4 pin 3 (PB1) → anode of yellow LED → cathode → 330 Ω resistor → GND rail
- J4 pin 4 (PB2) → anode of white LED → cathode → 330 Ω resistor → GND rail
- J4 pin 5 (PB3) → anode of green LED → cathode → 330 Ω resistor → GND rail
- J4 pin 6 (PB4) → anode of blue LED → cathode → 330 Ω resistor → GND rail

**Potentiometer connections (J3, PORTA/ADC header):**
- Outer leg 1 → VCC (J3 pin 1)
- Outer leg 2 → GND (J3 pin 10)
- Wiper → PA0 (J3 pin 2, ADC0)

**UART connections (J7 Molex KK header to Pololu programmer UART port):**
- Black wire (GND): J7 GND → Pololu programmer GND
- Orange wire: J7 PD0 (RXD into AVR) → Pololu programmer UART TX output
- Yellow wire: J7 PD1 (TXD from AVR) → Pololu programmer UART RX input
- Red wire (VCC): J7 VCC → Pololu programmer VCC

**JP1 must be fitted** to connect AVCC as the ADC reference voltage.

---

## Task 1 - Enable the Receiver and Test Reception

Start from the UART programme from session 6. Add RXEN0 and RXCIE0 to the uart_init function:

```c
UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);
```

Add the receive ISR:

```c
ISR(USART0_RX_vect)
{
    char c = UDR0;              // Must read UDR0 in every ISR call

    if (c == 'R')
    {
        PORTB |= (1<<PB0);      // Red LED on
    }
    else if (c == 'r')
    {
        PORTB &= ~(1<<PB0);     // Red LED off
    }
}
```

Add `sei()` in main after all initialisation. Build and flash.

In the terminal, type `R` and press enter (or just `R` if the terminal sends on keypress). Observe the red LED.

Expected result: `R` turns the red LED on, `r` turns it off.

Pass criteria: LED responds to each command within one or two character lengths of delay.

---

## Task 2 - Expand to All LEDs

Replace the `if/else if` chain with a `switch` statement covering all five LEDs:

```c
ISR(USART0_RX_vect)
{
    char c = UDR0;

    switch (c)
    {
        case 'R': PORTB |= (1<<PB0);   break;
        case 'r': PORTB &= ~(1<<PB0);  break;
        case 'Y': PORTB |= (1<<PB1);   break;
        case 'y': PORTB &= ~(1<<PB1);  break;
        case 'W': PORTB |= (1<<PB2);   break;
        case 'w': PORTB &= ~(1<<PB2);  break;
        case 'G': PORTB |= (1<<PB3);   break;
        case 'g': PORTB &= ~(1<<PB3);  break;
        case 'B': PORTB |= (1<<PB4);   break;
        case 'b': PORTB &= ~(1<<PB4);  break;
    }
}
```

Build and test all ten commands.

Pass criteria: each uppercase command lights its LED; each lowercase command extinguishes it.

---

## Task 3 - Echo Received Characters

Add a call to `transmit_char(c)` at the start of the ISR so the terminal echoes what was received:

```c
ISR(USART0_RX_vect)
{
    char c = UDR0;
    transmit_char(c);           // Echo back to terminal
    // Switch statement here
}
```

Build and test. Each character typed should now appear in the terminal output as well as controlling the LED.

Pass criteria: characters appear in the terminal and control LEDs simultaneously.

---

## Task 4 - Add an ADC Command

Add a case for `'A'` that reads the ADC and transmits the result:

```c
case 'A':
    {
        uint16_t val = adc_read();
        sprintf(uart_buffer, "ADC0: %u\r\n", val);
        transmit_string(uart_buffer);
    }
    break;
```

Add the ADC initialisation functions from session 7 and call `adc_init()` in main.

Build, flash and send `A` from the terminal. Adjust the potentiometer between readings.

Pass criteria: the ADC reading changes as the potentiometer is adjusted.

---

## Task 5 - Three ISRs Running Together

Combine the timer CTC interrupt, the UART receive interrupt and the ADC into one programme:

- Timer ISR: toggles the blue LED and transmits an ADC reading every 500ms.
- Receive ISR: controls LEDs via commands and responds to `'A'` on demand.
- Main loop: empty.

Full ISR structure:

```c
volatile uint16_t adc_result = 0;

ISR(TIMER1_COMPA_vect)
{
    adc_result = adc_read();
    PORTB ^= (1<<PB4);
    sprintf(uart_buffer, "Periodic ADC: %u\r\n", adc_result);
    transmit_string(uart_buffer);
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
            sprintf(uart_buffer, "On-demand ADC: %u\r\n", adc_result);
            transmit_string(uart_buffer);
            break;
    }
}
```

Build and test. The terminal should show periodic ADC readings from the timer and on-demand readings from `A`.

Pass criteria:

- Blue LED toggles at the timer rate without any delay in main.
- LED commands respond immediately.
- `A` returns the most recent ADC result.

---

## Quick Fault Isolation

- **No response to received characters**: RXEN0 or RXCIE0 not set or sei() missing.
- **Characters received but ISR seems to fire twice**: check the terminal is not sending CRLF after each character.
- **Blue LED stops blinking when typing**: ISR is taking too long; move processing out of the ISR via a flag.
- **ADC returns 0 in Task 4**: adc_init() not called or ADMUX not set.
- **Compile error on USART0_RX_vect**: include avr/interrupt.h.

---

## Lab Record

- Date:
- Tasks completed:
- Commands tested (Task 2):
- Timer interval used (Task 5):
- ADC reading range observed:
- Three ISRs confirmed working together:
- Issues found:
- Fix applied:
