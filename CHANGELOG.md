# Changelog

All notable changes to this project are documented in this file.

---

## [Unreleased]

### Changed
- Documentation Hub in README restructured into a session notes table and a general reference section
- `docs/c_operators.md` cleaned up: removed stale attribution line

### Added
- `CHANGELOG.md` (this file)

---

## [2026-05-06] — Session notes full rewrite (PR #20)

### Added
- `notes/general/c_operators.md` — full C operator reference with precedence table, AVR bit manipulation patterns and GPIO mask warnings
- `notes/general/hardware.md` — PCB overview, power supply (JP1/JP2), all port headers (J1–J7), crystal, fuse summary and component list

### Changed
All eight session notes and labs rewritten to match actual hardware (ATmega644P, Pololu USB AVR Programmer v2.1, Richard Reeves PCB 2018) and reorganised into the correct topic order:

| Session | Topic |
| ------- | ----- |
| 1 | Introduction to AVR C |
| 2 | Bit Shifting, Arrays and Data Types |
| 3 | Inputs, Bit Masking and Interrupts |
| 4 | Timers: Overflow and Output Compare |
| 5 | Hardware PWM (Timer 0, OC0A/OC0B) |
| 6 | UART Serial Transmission |
| 7 | Analogue to Digital Conversion |
| 8 | UART Serial Reception |

Each session note now covers the topic theory with correct register names, pin assignments and code examples. Each lab has ordered tasks, expected results, pass criteria and a fault isolation section.

### Removed
- Old flat-file session notes that referenced incorrect hardware, a different MCU (ATmega164P), Proteus simulation and university course context

---

## [2025-12] — Session notes initial pass (PRs #4–#18)

### Added
- `notes/session_01/` through `notes/session_08/` — first pass of session notes and lab guides. Topics at this stage reflected an earlier project structure (GPIO outputs, state machines, reaction game, Tetris melody) rather than the final session topic order.
- `WORKFLOW.md` — complete build and flash guide for PlatformIO in VS Code and Microchip Studio 7, including environment switching, manual avrdude commands, fuse restoration and troubleshooting
- `docs/wiring.md` — breadboard connections, header pin tables for J3 to J6
- `docs/hardware_notes.md` — fuse settings, ISP clock reference, crystal parameters, power supply, UART0 header wiring
- `docs/c_operators.md` — C operator reference table
- `docs/atmel_studio_workflow.md` — step-by-step Microchip Studio 7 first-time setup

### Changed
- README expanded with clickable links, Getting Started section, project table and Documentation Hub (PRs #2, #4)

---

## [2025-11] — Initial commit

### Added
- `projects/01_blink/` — double blink on PB0
- `projects/02_led_cycle/` — five LEDs cycling sequentially
- `projects/03_button_polling/` — button drives buzzer via polling
- `projects/04_interrupt_buzzer/` — button drives buzzer via INT0 interrupt
- `projects/05_state_machine_basic/` — four-mode state machine
- `projects/06_state_machine/` — nine-mode state machine (chase, blink all, alternate, PWM fade, Knight Rider, binary counter, random, reaction game, Tetris melody)
- `projects/00_fuse_test/` — fuse configuration and restoration reference
- `platformio/` — PlatformIO project with one environment per source file and shared `[common]` settings
- `hardware/pcb_notes.md` — full PCB reference
- GitHub issue and PR templates
- MIT licence
