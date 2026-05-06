# C Operators Reference

---

## Arithmetic Operators

| Operator | Name           | Example | Description                 |
| -------- | -------------- | ------- | --------------------------- |
| `*`      | Multiplication | `x*y`   | Multiply x times y          |
| `/`      | Division       | `x/y`   | Divide x by y               |
| `%`      | Modulo         | `x%y`   | Remainder of x divided by y |
| `+`      | Addition       | `x+y`   | Add x and y                 |
| `-`      | Subtraction    | `x-y`   | Subtract y from x           |
| `++`     | Increment      | `x++`   | Increment x after using it  |
| `--`     | Decrement      | `--x`   | Decrement x before using it |
| `-`      | Negation       | `-x`    | Multiply x by -1            |
| `+`      | Unary Plus     | `+x`    | Show x is positive          |

---

## Relational and Logical Operators

| Operator | Name                     | Example  | Description                                       |
| -------- | ------------------------ | -------- | ------------------------------------------------- |
| `>`      | Greater than             | `x>y`    | 1 if x is greater than y, otherwise 0             |
| `>=`     | Greater than or equal to | `x>=y`   | 1 if x is greater than or equal to y, otherwise 0 |
| `<`      | Less than                | `x<y`    | 1 if x is less than y, otherwise 0                |
| `<=`     | Less than or equal to    | `x<=y`   | 1 if x is less than or equal to y, otherwise 0    |
| `==`     | Equal to                 | `x==y`   | 1 if x equals y, otherwise 0                      |
| `!=`     | Not equal to             | `x!=y`   | 1 if x is not equal to y, otherwise 0             |
| `!`      | Logical NOT              | `!x`     | 1 if x is 0, otherwise 1                          |
| `&&`     | Logical AND              | `x&&y`   | 0 if either x or y is 0, otherwise 1              |
| `\|\|`   | Logical OR               | `x\|\|y` | 0 if both x and y are 0, otherwise 1              |

---

## Bitwise Operators

These are used extensively in AVR C for setting, clearing and toggling individual register bits.

| Operator | Name               | Example | Description                         |
| -------- | ------------------ | ------- | ----------------------------------- |
| `~`      | Bitwise Complement | `~x`    | Changes 1 bits to 0 and 0 bits to 1 |
| `&`      | Bitwise AND        | `x&y`   | Bitwise AND of x and y              |
| `\|`     | Bitwise OR         | `x\|y`  | Bitwise OR of x and y               |
| `^`      | Bitwise XOR        | `x^y`   | Bitwise XOR of x and y              |
| `<<`     | Left Shift         | `x<<2`  | Bits in x shifted left 2 positions  |
| `>>`     | Right Shift        | `x>>3`  | Bits in x shifted right 3 positions |

### Common AVR bit manipulation patterns

```c
DDRB |= (1<<PB0);           // Set bit - configure PB0 as output
PORTB |= (1<<PB0);          // Set bit - turn on PB0
PORTB &= ~(1<<PB0);         // Clear bit - turn off PB0
PORTB ^= (1<<PB0);          // Toggle bit - flip PB0 state
if (PIND & (1<<PD2)) { }    // Read bit - test if PD2 is high
```

---

## Data Access and Size Operators

| Operator | Name             | Example      | Description                                     |
| -------- | ---------------- | ------------ | ----------------------------------------------- |
| `[]`     | Array Element    | `x[6]`       | Seventh element of array x                      |
| `.`      | Member Selection | `PORTD.2`    | Bit 2 of Port D                                 |
| `->`     | Member Selection | `pStruct->x` | Member x of the structure pointed to by pStruct |
| `*`      | Indirection      | `*p`         | Contents of memory at address p                 |
| `&`      | Address of       | `&x`         | Address of variable x                           |

---

## Assignment Operators

| Operator | Name                | Example | Equivalent             |
| -------- | ------------------- | ------- | ---------------------- |
| `=`      | Assignment          | `x=y`   | Assign value of y to x |
| `+=`     | Compound Assignment | `x+=y`  | `x=x+y`                |
| `-=`     | Compound Assignment | `x-=y`  | `x=x-y`                |
| `*=`     | Compound Assignment | `x*=y`  | `x=x*y`                |
| `/=`     | Compound Assignment | `x/=y`  | `x=x/y`                |
| `%=`     | Compound Assignment | `x%=y`  | `x=x%y`                |
| `&=`     | Compound Assignment | `x&=y`  | `x=x&y`                |
| `^=`     | Compound Assignment | `x^=y`  | `x=x^y`                |
| `\|=`    | Compound Assignment | `x\|=y` | `x=x\|y`               |
| `<<=`    | Compound Assignment | `x<<=y` | `x=x<<y`               |
| `>>=`    | Compound Assignment | `x>>=y` | `x=x>>y`               |

---

## Miscellaneous Operators

| Operator | Name                  | Example     | Description                                    |
| -------- | --------------------- | ----------- | ---------------------------------------------- |
| `()`     | Function call         | `Wait(10)`  | Call Wait function with argument 10            |
| `(type)` | Type cast             | `(double)x` | Convert x to a double                          |
| `?:`     | Conditional           | `x?y:z`     | If x is not 0 evaluate y, otherwise evaluate z |
| `,`      | Sequential evaluation | `x++,y++`   | Increment x first, then increment y            |

---

## Operator Precedence

Operators are evaluated in the order listed from highest to lowest precedence (1 = highest).
When more than one operator appears in a statement, higher precedence operators are evaluated first.

| Precedence  | Operators                                                |
| ----------- | -------------------------------------------------------- |
| 1 (highest) | `()` `[]` `.` `->`                                       |
| 2           | `++` `--` `!` `~` `+` `-` `*` `&` `sizeof`               |
| 3           | Cast operator `(type)`                                   |
| 4           | `*` `/` `%`                                              |
| 5           | `+` `-`                                                  |
| 6           | `<<` `>>`                                                |
| 7           | `<` `<=` `>` `>=`                                        |
| 8           | `==` `!=`                                                |
| 9           | `&`                                                      |
| 10          | `^`                                                      |
| 11          | `\|`                                                     |
| 12          | `&&`                                                     |
| 13          | `\|\|`                                                   |
| 14          | `?:` (conditional)                                       |
| 15          | `=` `+=` `-=` `*=` `/=` `%=` `&=` `^=` `\|=` `<<=` `>>=` |
| 16 (lowest) | `,` (comma)                                              |
