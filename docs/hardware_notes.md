# Hardware Notes

Technical reference for the ATmega644P setup used in avr-zac projects.

---

## Fuse Settings

These fuse bytes configure the ATmega644P for a 20MHz external crystal with no clock division,
SPI programming enabled and JTAG disabled.

| Fuse | Value | Meaning |
|------|-------|---------|
| LOW | `0xFF` | External crystal oscillator, maximum frequency, no clock divide, no clock output |
| HIGH | `0xD1` | SPI programming enabled, JTAG disabled, watchdog off, EEPROM not preserved |
| EXTENDED | `0xFF` | Brown-out detection disabled |

### Restore Fuses with avrdude

```
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -F -U lfuse:w:0xFF:m -U hfuse:w:0xD1:m -U efuse:w:0xFF:m
```

Use the `-F` flag to force the operation even if the device signature check fails. This is
required when restoring fuses after a bad fuse write that may have changed the clock source.

---

## ISP Clock

The Pololu USB AVR Programmer v2.1 communicates via the STK500v2 protocol. The ISP clock
must be slowed to avoid timeout errors, particularly after a fuse change has altered the CPU
clock source.

Always use the `-B 10` flag with avrdude to set a slower ISP bit clock:

```
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -B 10 -V -U flash:w:firmware.hex:i
```

The `-B 10` flag sets the ISP bit clock period to 10 microseconds (approximately 50kHz). Without
this flag, programming may fail with a timeout error.

---

## Crystal

| Parameter | Value |
|-----------|-------|
| Crystal frequency | 20MHz |
| Load capacitors C1 and C2 | 22pF (on PCB) |
| `F_CPU` definition | `20000000UL` |
| Maximum rated frequency (ATmega644P) | 20MHz at 4.5V to 5.5V |

The crystal X1 and both 22pF load capacitors C1 and C2 are soldered on the PCB. No external
crystal circuit is required on the breadboard.

In PlatformIO (`platformio.ini`):
```ini
build_flags = -DF_CPU=20000000UL
board_build.f_cpu = 20000000UL
```

In Microchip Studio (top of source file):
```c
#define F_CPU 20E6
```

---

## Power Supply

| Parameter | Value |
|-----------|-------|
| Input voltage | 12V DC via J8 terminal block |
| Regulator | LM317T adjustable voltage regulator (U2) |
| Output voltage (5V mode) | 5V; JP2 not fitted |
| Output voltage (3.3V mode) | 3.3V; JP2 fitted |
| Reverse polarity protection | D2 (1N4007) on DC input |
| Power indicator | D3 LED with 1kΩ series resistor R9; always on when powered |

The LM317T requires its input to be at least 2V above the output voltage (dropout voltage),
not including the voltage dropped across D2. A 12V input is therefore suitable for both 5V
and 3.3V operating modes.

---

## ADC Reference

With jumper JP1 fitted, AVCC filtered by R2 (100Ω) and C3 (100nF) is the ADC reference
(AREF). This is the standard lab configuration.

To use an external precision reference (such as a bandgap diode), remove JP1 and connect
the external reference voltage to J2.

---

## UART0

UART0 is available on J7, a 4-way Molex KK header. The FTDI lead (TTL-232RG-VIP-WE) is
used for serial communication. Only four of the six lead wires are needed:

| Wire colour | Signal | Direction |
|-------------|--------|-----------|
| Black | GND (0V) | Common ground |
| Orange | RXD | Lead output to AVR; AVR receives on PD0 |
| Yellow | TXD | AVR output to lead; AVR transmits on PD1 |
| Red | VCC | Power from PCB to FTDI lead |

The brown and green wires (flow control) are not used and can be trimmed. Solder the header
with the plastic back facing towards J6.

---

## Key Registers (ATmega644P)

| Register | Purpose |
|----------|---------|
| `DDRB` | Data direction for PORTB; 1 = output, 0 = input |
| `PORTB` | Output register for PORTB |
| `PINB` | Input register for PORTB; read GPIO state |
| `DDRD` | Data direction for PORTD |
| `PORTD` | Output register for PORTD |
| `PIND` | Input register for PORTD |
| `EICRA` | External interrupt control register A; configures INT0 and INT1 edge |
| `EIMSK` | External interrupt mask register; enables INT0 and INT1 |
| `ADMUX` | ADC multiplexer selection; selects channel and reference |
| `ADCSRA` | ADC control and status register A; enables and starts conversions |
| `ADCL` | ADC result low byte |
| `ADCH` | ADC result high byte |

### INT0 Configuration (falling edge trigger)

```c
EICRA |= (1<<ISC01);     // Falling edge triggers INT0
EICRA &= ~(1<<ISC00);    // Clear ISC00
EIMSK |= (1<<INT0);      // Enable INT0
sei();                   // Enable global interrupts
```

| ISC01 | ISC00 | Trigger condition |
|-------|-------|------------------|
| 0 | 0 | Low level on INT0 |
| 0 | 1 | Any logical change on INT0 |
| 1 | 0 | Falling edge on INT0 |
| 1 | 1 | Rising edge on INT0 |

### ADC Noise Read for Pseudo Random Number

```c
ADMUX = 0x00;           // Select ADC0, AVCC reference
ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
while (ADCSRA & (1<<ADSC));   // Wait for conversion
return ADCL;            // Low byte as random seed
```
