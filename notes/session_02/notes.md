# Session 2 - Bit Shifting, Arrays and Data Types

---

## Why Avoid Magic Numbers

When setting peripheral registers directly with raw values the code becomes unreadable and fragile:

```c
TIMSK0 = 0x06;      // What does this actually do?
```

This is a magic number. If the bit positions change on a different AVR or the code is read by someone unfamiliar with the datasheet, the intent is completely unclear.

The correct approach uses bit names and the shift operator so the code is self-documenting:

```c
TIMSK0 = (1<<OCIE0B) | (1<<OCIE0A);
```

The compiler substitutes the correct numerical values at compile time. If the bit positions were different on another device, only the header file would change.

---

## Bit Shifting

### The left-shift operator

The left-shift operator `<<` moves all bits in a value to the left by a given number of positions. Bits shifted off the left end are lost and zeros fill from the right.

```c
1 << 2          // 0b00000001 becomes 0b00000100
1 << OCIE0B     // Puts a 1 in the OCIE0B position (position 2)
```

### Setting a single bit with OR

OR-assigning a mask sets the target bit without changing any other bits:

```c
PORTB |= (1<<PB0);      // Set PB0, leave all other PORTB bits unchanged
```

How it works:

```text
1<<PB0    0 0 0 0 0 0 0 1
PORTB     X X X X X X X X
ORed      X X X X X X X 1
```

### Setting multiple bits with OR

Combine multiple masks by ORing them inside the shift expression:

```c
DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4);
```

### Clearing a single bit with AND and NOT

To clear a bit, construct a mask with a 0 in the target position and 1s everywhere else. The tilde `~` operator inverts all bits:

```c
PORTB &= ~(1<<PB0);     // Clear PB0, leave all other PORTB bits unchanged
```

How it works:

```text
1<<PB0       0 0 0 0 0 0 0 1
~(1<<PB0)    1 1 1 1 1 1 1 0
PORTB        X X X X X X X X
ANDed        X X X X X X X 0
```

The parentheses around `(1<<PB0)` before the tilde are essential. Without them the operator precedence inverts the 1 first, producing the wrong mask:

```c
PORTB &= ~(1<<PB0);     // Correct
PORTB &= ~1<<PB0;       // Wrong — inverts 1 first, then shifts
```

### Clearing multiple bits

OR the individual bits inside the parentheses before applying the tilde:

```c
PORTB &= ~((1<<PB0) | (1<<PB1));    // Clear PB0 and PB1 together
```

### Toggling a bit with XOR

XOR-assigning with a mask toggles the target bit and leaves all others unchanged:

```c
PORTB ^= (1<<PB0);      // Toggle PB0
```

XOR truth: 0 XOR 1 = 1, 1 XOR 1 = 0. ORing with 0 leaves a bit unchanged; ORing with 1 toggles it.

---

## Data Types

AVR programmes work with 8-bit registers, so matching variable sizes to those registers matters for efficiency and correctness.

| Type             | Alias     | Width   | Range                       |
| ---------------- | --------- | ------- | --------------------------- |
| `unsigned char`  | `uint8_t` | 8-bit   | 0 to 255                    |
| `char`           | `int8_t`  | 8-bit   | -128 to +127                |
| `unsigned int`   | `uint16_t`| 16-bit  | 0 to 65535                  |
| `int`            | `int16_t` | 16-bit  | -32768 to +32767            |
| `unsigned long`  | `uint32_t`| 32-bit  | 0 to 4294967295             |
| `long`           | `int32_t` | 32-bit  | -2147483648 to +2147483647  |

The aliases `uint8_t`, `int8_t` etc. come from `<stdint.h>` which is included automatically by `<avr/io.h>`. Prefer these aliases because the width is explicit in the name.

As the ATmega644P is an 8-bit processor, limit variables to 8-bit types where possible. A `uint16_t` takes two instructions to load where a `uint8_t` takes one.

Do not use floating-point numbers on the AVR. They are software-emulated, slow and use a large amount of flash.

---

## Arrays

An array is a block of consecutive memory locations that all hold the same data type. The declaration syntax is:

```c
data_type array_name[array_size];
```

Examples:

```c
uint8_t pattern[5];         // Five 8-bit values, uninitialised
uint8_t led_mask[5] = {0x01, 0x02, 0x04, 0x08, 0x10};  // Initialised
char message[] = {"Hello"};  // Compiler sets the size automatically
```

Array elements are accessed with square brackets. The first element is index 0:

```c
PORTB = led_mask[0];        // Write first element to PORTB
uint8_t val = led_mask[3];  // Read fourth element into val
```

A string in C is an array of `char` ending in a null terminator (value 0). The array must be at least one element longer than the visible text to hold the null:

```c
char name[6] = {"Hello"};   // 5 characters plus null at name[5]
```

### LED pattern array example

Storing five single-LED patterns in an array allows them to be cycled with a loop index:

```c
uint8_t patterns[5] = {
    (1<<PB0),       // Red only
    (1<<PB1),       // Yellow only
    (1<<PB2),       // White only
    (1<<PB3),       // Green only
    (1<<PB4)        // Blue only
};
```

---

## The for() Loop

A `for` loop repeats a block of code while a condition holds, automatically managing a loop variable:

```c
for(initialiser; condition; modifier)
{
    // Code repeated while condition is true
}
```

Example: cycle through the five LED patterns:

```c
uint8_t patterns[5] = {
    (1<<PB0), (1<<PB1), (1<<PB2), (1<<PB3), (1<<PB4)
};

for(int8_t i = 0; i < 5; i++)
{
    PORTB = patterns[i];
    _delay_ms(300);
}
PORTB = 0x00;
```

The loop variable `i` is initialised to 0 (argument 1), the loop repeats while `i < 5` (argument 2) and `i` increments by 1 after each pass (argument 3).

Use `int8_t` for loop variables even when the range is positive, because signed types detect when a decremented variable goes below 0. An unsigned 8-bit variable decremented from 0 wraps to 255, which is still positive and the loop would not exit.

---

## Enumerations

An enumeration assigns named integer constants automatically, starting from 0:

```c
typedef enum {
    MODE_CHASE,         // Value 0
    MODE_BLINK_ALL,     // Value 1
    MODE_ALTERNATE,     // Value 2
    MODE_PWM_FADE,      // Value 3
    MODE_COUNT          // Value 4 — useful as a total count
} LED_Mode;
```

Using `typedef enum` allows the type to be used directly:

```c
LED_Mode current_mode = MODE_CHASE;
current_mode = (current_mode + 1) % MODE_COUNT;   // Wrap to next mode
```

Enumerations make `switch` statements readable and let the compiler warn if a case is missing.

### Using an enumeration with a for loop

Because enumeration values are integers they can drive a `for` loop index directly:

```c
typedef enum {LED_RED, LED_YELLOW, LED_WHITE, LED_GREEN, LED_BLUE} LEDs;

for(int8_t led = LED_RED; led <= LED_BLUE; led++)
{
    PORTB = (1<<led);       // Light one LED at a time
    _delay_ms(250);
}
PORTB = 0x00;
```

This works because LED_RED = 0 through LED_BLUE = 4, which matches PB0 through PB4.

---

## Registers Are Variables

Peripheral registers such as PORTB and PIND are variables in memory. They can be copied, compared and used in expressions just like any `uint8_t`:

```c
uint8_t snapshot = PINB;    // Copy current pin states into snapshot
PORTB = snapshot;           // Copy snapshot to output register
PORTB = PIND;               // Short form: copy PIND directly to PORTB
```

---

## Practical Checklist

1. Confirm all LED pins are set as outputs in DDRB.
2. Use `|=` and `&= ~()` rather than direct assignment when only some bits should change.
3. Always add parentheses around the shift expression before applying `~`.
4. Choose `int8_t` for loop variables when the loop may decrement.
5. Keep arrays sized correctly: a 5-element pattern array has valid indices 0 to 4.

---

## Linked Project Files

- [projects/01_blink/01_blink.c](../../projects/01_blink/01_blink.c)
- [projects/02_led_cycle/02_led_cycle.c](../../projects/02_led_cycle/02_led_cycle.c)
- [platformio/src/01_blink.c](../../platformio/src/01_blink.c)
- [platformio/src/02_led_cycle.c](../../platformio/src/02_led_cycle.c)
