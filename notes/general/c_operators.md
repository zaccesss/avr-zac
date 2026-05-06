# C Operators Reference

---

## Arithmetic Operators

| Operator | Name        | Example  | Meaning                               |
| -------- | ----------- | -------- | ------------------------------------- |
| `*`      | Multiply    | `x * y`  | Multiply x by y                       |
| `/`      | Divide      | `x / y`  | Divide x by y                         |
| `%`      | Modulo      | `x % y`  | Remainder of x divided by y           |
| `+`      | Addition    | `x + y`  | Add x and y                           |
| `-`      | Subtraction | `x - y`  | Subtract y from x                     |
| `++`     | Increment   | `x++`    | Increment x after using its value     |
| `--`     | Decrement   | `--x`    | Decrement x before using its value    |
| `-`      | Negation    | `-x`     | Multiply x by negative one            |
| `+`      | Unary plus  | `+x`     | Show x is positive                    |

---

## Relational and Logical Operators

| Operator | Name                  | Example   | Meaning                                     |
| -------- | --------------------- | --------- | ------------------------------------------- |
| `>`      | Greater than          | `x > y`   | 1 if x is greater than y, otherwise 0       |
| `>=`     | Greater than or equal | `x >= y`  | 1 if x is greater than or equal to y        |
| `<`      | Less than             | `x < y`   | 1 if x is less than y, otherwise 0          |
| `<=`     | Less than or equal    | `x <= y`  | 1 if x is less than or equal to y           |
| `==`     | Equal to              | `x == y`  | 1 if x equals y, otherwise 0                |
| `!=`     | Not equal to          | `x != y`  | 1 if x is not equal to y, otherwise 0       |
| `!`      | Logical NOT           | `!x`      | 1 if x is 0, otherwise 1                    |
| `&&`     | Logical AND           | `x && y`  | 0 if either x or y is 0, otherwise 1        |
| `\|\|`   | Logical OR            | `x \|\| y`| 0 if both x and y are 0, otherwise 1        |

---

## Bitwise Operators

| Operator | Name              | Example   | Meaning                                           |
| -------- | ----------------- | --------- | ------------------------------------------------- |
| `~`      | Bitwise NOT       | `~x`      | Inverts all bits: 1s become 0s and 0s become 1s  |
| `&`      | Bitwise AND       | `x & y`   | Bitwise AND of x and y                            |
| `\|`     | Bitwise OR        | `x \| y`  | Bitwise OR of x and y                             |
| `^`      | Bitwise XOR       | `x ^ y`   | Bitwise exclusive OR of x and y                   |
| `<<`     | Left shift        | `x << 2`  | Bits in x are shifted left 2 positions            |
| `>>`     | Right shift       | `x >> 3`  | Bits in x are shifted right 3 positions           |

### GPIO use summary

```c
PORTB |= (1<<PB0);      // Set bit - turn on LED
PORTB &= ~(1<<PB0);     // Clear bit - turn off LED
PORTB ^= (1<<PB0);      // Toggle bit - flip LED state
if (PIND & (1<<PD2))    // Test bit - check button state
```

---

## Data Access and Size Operators

| Operator | Name             | Example         | Meaning                                        |
| -------- | ---------------- | --------------- | ---------------------------------------------- |
| `[]`     | Array element    | `x[6]`          | Seventh element of array x                     |
| `.`      | Member select    | `struct.field`  | Access field of a struct                       |
| `->`     | Pointer member   | `p->x`          | Member x of the structure pointed to by p      |
| `*`      | Indirection      | `*p`            | Contents of memory at address p                |
| `&`      | Address of       | `&x`            | Memory address of variable x                   |

---

## Assignment Operators

| Operator | Name                | Example     | Meaning                                |
| -------- | ------------------- | ----------- | -------------------------------------- |
| `=`      | Assignment          | `x = y`     | Assign value of y to x                 |
| `+=`     | Add assign          | `x += y`    | x = x + y                              |
| `-=`     | Subtract assign     | `x -= y`    | x = x - y                              |
| `*=`     | Multiply assign     | `x *= y`    | x = x * y                              |
| `/=`     | Divide assign       | `x /= y`    | x = x / y                              |
| `%=`     | Modulo assign       | `x %= y`    | x = x % y                              |
| `&=`     | AND assign          | `x &= y`    | x = x & y                              |
| `^=`     | XOR assign          | `x ^= y`    | x = x ^ y                              |
| `\|=`    | OR assign           | `x \|= y`   | x = x \| y                             |
| `<<=`    | Left shift assign   | `x <<= y`   | x = x << y                             |
| `>>=`    | Right shift assign  | `x >>= y`   | x = x >> y                             |

---

## Miscellaneous Operators

| Operator | Name               | Example       | Meaning                                          |
| -------- | ------------------ | ------------- | ------------------------------------------------ |
| `()`     | Function call      | `wait(10)`    | Call the wait function with argument 10          |
| `(type)` | Type cast          | `(double)x`   | Convert x to a double                            |
| `?:`     | Conditional        | `x ? y : z`   | Evaluate y if x is non-zero, otherwise evaluate z|
| `,`      | Sequential eval    | `x++, y++`    | Increment x first, then increment y              |

---

## Operator Precedence

Higher precedence evaluates first.

| Precedence | Operators                                     |
| ---------- | --------------------------------------------- |
| 1          | `() [] . ->`                                  |
| 2          | `++ -- ! ~ + - * & sizeof`                    |
| 3          | Cast operator                                 |
| 4          | `* / %`                                       |
| 5          | `+ -`                                         |
| 6          | `<< >>`                                       |
| 7          | `< <= > >=`                                   |
| 8          | `== !=`                                       |
| 9          | `&`                                           |
| 10         | `^`                                           |
| 11         | `\|`                                          |
| 12         | `&&`                                          |
| 13         | `\|\|`                                        |
| 14         | `?:` (conditional)                            |
| 15         | `= += -= *= /= %= &= ^= \|= <<= >>=`          |
| 16         | `,` (comma)                                   |

When more than one operator appears in a statement they are evaluated from the top of this table downwards.

---

## Parentheses Warning for GPIO Masks

When using bitwise operators with bit-shift expressions, always group the shift first:

```c
PORTB &= ~(1<<PB0);      // Correct - shift happens before NOT
PORTB &= ~1<<PB0;        // Wrong  - NOT applies to 1 first, then shifts
```

The second line incorrectly inverts 1 to get 0b11111110, then shifts left, masking the wrong bits.
