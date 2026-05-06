# Session 8 - Music Output and Full System Integration

Source: AVR project code and lab progression in this repo.
Adapted for: ATmega644P full nine-mode state machine.

---

## Session Aim

Complete the learning sequence by integrating LED control, mode switching, buzzer tones and user interaction into one cohesive embedded system.

Learning outcomes:

1. Generate note patterns on an active buzzer.
2. Synchronise visual output with audio events.
3. Validate complete multi-mode system behaviour.
4. Plan next improvements using timers and UART.

---

## Tone Generation Approach

The full build uses a tone helper with cycle count derived from note frequency and duration:

```c
uint16_t cycles = (uint32_t)duration_ms * freq / 1000;
```

Then each cycle toggles buzzer output with a fixed half-period delay.

This is simple for learning, though timing precision is limited by software delay overhead.

---

## Melody Table Pattern

Music mode stores notes as frequency and duration pairs:

```c
{659, 300}, {494, 150}, ...
```

A frequency of zero is treated as a rest.

LEDs are stepped alongside note playback for visible rhythm feedback.

---

## Integration Checkpoints

Full system integration should verify:

1. Startup animation runs once at boot.
2. Button cycles through all nine modes.
3. Every mode has distinct output behaviour.
4. No mode locks up button handling.
5. Mode wrap works from last mode to first mode.

---

## Troubleshooting Focus

| Symptom                     | Likely cause                               | First check              |
| --------------------------- | ------------------------------------------ | ------------------------ |
| Music sounds wrong          | Delay assumptions and tone loop overhead   | Tone helper logic        |
| Button ignored in busy mode | Long blocking loops                        | ISR flag behaviour       |
| LEDs flicker unexpectedly   | Port writes conflicting between behaviours | Mode-local output writes |
| Upload fails                | Programmer timing too fast                 | `-B 10` upload flag      |

---

## Recommended Next Steps

1. Move software timing paths to hardware timers for better precision.
2. Add UART debug prints for mode and event tracing.
3. Extract mode framework into reusable helpers.
4. Add per-mode test checklist to project docs.

---

## Linked Project Files

- `projects/06_state_machine/06_state_machine.c`
- `platformio/src/06_state_machine.c`
