# Changelog

All notable changes to this project are documented in this file.

---

## [Unreleased]

### Added
- `.editorconfig` at repo root: UTF-8, LF line endings, 4-space indent, trim trailing whitespace
- `projects/lab_projects/`, `projects/personal_projects/`, `projects/practice_projects/`, `projects/other_projects/` - empty category folders for future work
- Breadboard Setup section in every lab file (sessions 1-8) with PCB header pin, component and wiring instructions for replicating each circuit on breadboard
- `CONTRIBUTING.md` at repo root: branch naming, commit format, style guide, PR checklist and issue reporting guidance
- `hardware/AVR_PCB_2019.pdf` - original PCB schematic and layout for the Richard Reeves AVR Project PCB 2019
- `hardware/ATmega644P_datasheet.pdf` - official Microchip datasheet for the ATmega644P
- `docs/C_Operators.pdf` - printable C operator reference card

### Changed
- README: breadboard-is-temporary note and the avrdude `-B 10` flag explanation converted to markdown alert callouts (`[!NOTE]`, `[!IMPORTANT]`) so they stand out from surrounding prose
- README rewritten: removed the animated capsule-render header/footer banner and the readme-typing-svg text banner, dropped the tech stack icon table in favour of a plain sentence, removed the duplicate top badge row that repeated the Contact and Support section, linked `docs/atmel_studio_workflow.md` from the Documentation Hub, collapsed a double divider and rewrote prose sections in first person
- `projects/` restructured: all source projects moved into `projects/learning_projects/` (one folder per project)
- `platformio/src/` reduced to a single active file (`06_state_machine.c`); all other `.c` files removed
- `platformio/platformio.ini` rewritten to a single environment (`06_state_machine`) with a `[common]` base section; inline comments explain how to switch projects
- `WORKFLOW.md` switching guide updated for the single-file-in-src approach with project folder table
- README project table: `00_fuse_test` moved to row 0 (first), PCB year corrected to 2019, tech stack icons moved inline under Overview, PDF links added to Documentation Hub
- README project table links and source file description updated to reflect `projects/learning_projects/` layout
- All em and en dashes replaced across sessions 1-8 notes and labs, `notes/general/`, `docs/hardware_notes.md` and `docs/wiring.md`
- Inline code comments expanded across all session notes and lab task code blocks to fully explain register fields, bit manipulation and timing calculations
- `docs/hardware_notes.md` UART0 section updated: FTDI lead reference replaced with Pololu USB AVR Programmer v2.1 built-in UART bridge description
- `.gitignore` updated to exclude the local university source folder (`EE1EPJ - AVR - Microcontrollers/`)
- Documentation Hub in README restructured into a session notes table and a general reference section
- `notes/general/c_operators.md` cleaned up: removed stale attribution line

### Added (previous unreleased)
- `CHANGELOG.md` (this file)

---

## [2026-05-06] - Session notes full rewrite (PR #20)

### Added
- `notes/general/c_operators.md` - full C operator reference with precedence table, AVR bit manipulation patterns and GPIO mask warnings
- `notes/general/hardware.md` - PCB overview, power supply (JP1/JP2), all port headers (J1-J7), crystal, fuse summary and component list

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

## [2025-12] - Session notes initial pass (PRs #4-#18)

### Added
- `notes/session_01/` through `notes/session_08/` - first pass of session notes and lab guides. Topics at this stage reflected an earlier project structure (GPIO outputs, state machines, reaction game, Tetris melody) rather than the final session topic order.
- `WORKFLOW.md` - complete build and flash guide for PlatformIO in VS Code and Microchip Studio 7, including environment switching, manual avrdude commands, fuse restoration and troubleshooting
- `docs/wiring.md` - breadboard connections, header pin tables for J3 to J6
- `docs/hardware_notes.md` - fuse settings, ISP clock reference, crystal parameters, power supply, UART0 header wiring
- `docs/c_operators.md` - C operator reference table
- `docs/atmel_studio_workflow.md` - step-by-step Microchip Studio 7 first-time setup

### Changed
- README expanded with clickable links, Getting Started section, project table and Documentation Hub (PRs #2, #4)

---

## [2025-11] - Initial commit

### Added
- `projects/01_blink/` - double blink on PB0
- `projects/02_led_cycle/` - five LEDs cycling sequentially
- `projects/03_button_polling/` - button drives buzzer via polling
- `projects/04_interrupt_buzzer/` - button drives buzzer via INT0 interrupt
- `projects/05_state_machine_basic/` - four-mode state machine
- `projects/06_state_machine/` - nine-mode state machine (chase, blink all, alternate, PWM fade, Knight Rider, binary counter, random, reaction game, Tetris melody)
- `projects/00_fuse_test/` - fuse configuration and restoration reference
- `platformio/` - PlatformIO project with one environment per source file and shared `[common]` settings
- `hardware/pcb_notes.md` - full PCB reference
- GitHub issue and PR templates
- MIT licence
