# Hardware Notes

## Fuse Settings

The ATmega644P on the custom PCB requires the following fuse configuration:

```
Extended Fuse (EFUSE): 0xFF
High Fuse (HFUSE):    0xD1
Low Fuse (LFUSE):     0xFF
```

To restore these fuse bits using avrdude:

```bash
avrdude -c stk500v2 -p m644p -P COM4 -F \
  -U lfuse:w:0xFF:m \
  -U hfuse:w:0xD1:m \
  -U efuse:w:0xFF:m
```

## ISP Clock Speed

When programming via ICSP (In-Circuit Serial Programming), the Pololu USB AVR Programmer v2.1 requires a reduced bit clock speed to avoid timeouts on the ATmega644P. All avrdude commands must use the `-B 10` flag to set the ISP clock prescaler to 10.

This reduces the ISP clock from 200 kHz to 20 kHz, providing sufficient time for internal operations.

## Power Supply

The PCB uses an LM317T adjustable voltage regulator:

- Input: 12V from DC jack barrel connector
- Default output: 5V (selectable to 3.3V via jumper JP2)
- All logic runs at 5V for this configuration

Never exceed 12V input or the LM317T will be damaged.

## Crystal and Timing

The ATmega644P uses a 20MHz external crystal oscillator for all time-critical operations:

- F_CPU = 20000000UL (20 MHz)
- Crystal is mounted directly on the PCB (Y1)
- All software delay calculations are based on this frequency
- Changing F_CPU requires recompilation and fresh firmware upload

## Pin Configuration Notes

- Pins PB5, PB6 and PB7 are shared with the ISP lines (MOSI, MISO, SCK)
- 100Ω resistors (R3, R4, R5) isolate these pins from the ISP header
- Do not drive these pins HIGH continuously during programming
- All other pins on headers J3, J4, J5 and J6 are safe for general-purpose I/O
