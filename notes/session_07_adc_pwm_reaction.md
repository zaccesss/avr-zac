# Session 7 - ADC Noise, PWM Concepts and Reaction Logic

Source: AVR project code and lab progression in this repo.
Adapted for: ATmega644P full state machine project.

---

## Session Aim

Connect three practical concepts used in the full build: software PWM, ADC-based pseudo-random values and reaction-game event handling.

Learning outcomes:

1. Explain software PWM behaviour and trade-offs.
2. Read ADC noise as a simple random seed source.
3. Build event-driven game logic using interrupt flags.
4. Identify where timing and responsiveness can conflict.

---

## Software PWM in This Project

PWM fade is implemented in software loops by varying effective LED duty behaviour over time.

Conceptual flow:

1. Turn LEDs on for a short period.
2. Turn LEDs off for a short period.
3. Repeat with changing ratio to create perceived fade.

Trade-offs:

- Easy to understand.
- CPU-intensive compared with hardware timer PWM.
- Long loops can reduce responsiveness unless interrupts are used.

---

## ADC Noise as Pseudo-Random Source

The project reads ADC0 and uses low-byte noise as a random-like value:

```c
ADMUX = 0x00;
ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
while (ADCSRA & (1<<ADSC));
return ADCL;
```

This is suitable for simple LED pattern randomisation and game variation.

---

## Random Mode Mapping

Random value is masked to five bits so it maps cleanly onto five LEDs:

```c
uint8_t random_val = get_random() & 0x1F;
PORTB = random_val;
```

Masking prevents writes outside intended LED bits.

---

## Reaction Game Logic

High-level behaviour:

1. Random LED shown each round.
2. Interrupt sets a `button_pressed` flag.
3. Main logic checks whether pressed LED is the target (green).
4. Correct press gives success feedback.
5. Incorrect press gives error feedback.

This is a clean ISR plus main-loop cooperation pattern.

---

## Concurrency Pattern Used

| ISR side               | Main loop side              |
| ---------------------- | --------------------------- |
| Set a lightweight flag | Read and clear the flag     |
| Keep runtime short     | Perform full decision logic |

This reduces ISR complexity and improves maintainability.

---

## Practical Checklist

1. Confirm random mode shows varied patterns over time.
2. Confirm reaction game detects press events reliably.
3. Confirm correct and incorrect outcomes are distinguishable.
4. Confirm mode button still exits game mode consistently.

---

## Linked Project Files

- `projects/06_state_machine/06_state_machine.c`
- `platformio/src/06_state_machine.c`
