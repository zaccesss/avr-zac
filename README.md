<!-- Header -->
<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=gradient&height=120&section=header&text=AVR%20Embedded%20C&fontSize=38&fontAlignY=32&fontColor=ffffff&animation=bounce" />
</p>

<p align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=22&pause=1200&color=0066CC&center=true&vCenter=true&width=600&height=50&lines=ATmega644P;Bare+Metal+C;Microcontroller+Development;State+Machines+and+Interrupts;PlatformIO+and+Microchip+Studio" />
</p>

---

## Project Badges

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-A8B9CC?style=for-the-badge&logo=c&logoColor=white" />
  <img src="https://img.shields.io/badge/MCU-ATmega644P-FF9E64?style=for-the-badge&logo=microchip&logoColor=white" />
  <img src="https://img.shields.io/badge/Tool-PlatformIO-FF7F50?style=for-the-badge&logo=platformio&logoColor=white" />
  <img src="https://img.shields.io/badge/IDE-Microchip_Studio-0071C5?style=for-the-badge&logo=visualstudio&logoColor=white" />
  <img src="https://img.shields.io/badge/Editor-VS_Code-007ACC?style=for-the-badge&logo=visualstudiocode&logoColor=white" />
  <img src="https://img.shields.io/badge/University-Aston-6C2E7F?style=for-the-badge&logo=academia&logoColor=white" />
</p>

---

## Overview

This repository contains a collection of bare metal AVR C projects developed during Year 1 of the Electronic Engineering and Computer Science BEng at Aston University. The projects progress from a simple LED blink to a full 9-mode state machine incorporating interrupts, debouncing, software PWM, ADC random number generation, a reaction game and a Tetris melody. All projects target the ATmega644P microcontroller running at 20MHz on a custom PCB designed by Richard Reeves at Aston University.

---

## Hardware Specification

The AVR Project PCB is a professional development board with the following key components:

- **MCU:** ATmega644P DIP-40 running at 20MHz external crystal
- **Regulator:** LM317T adjustable voltage regulator (5V default, 3.3V selectable via JP2)
- **Programmer:** Pololu USB AVR Programmer v2.1 via STK500v2 protocol on COM4
- **I/O:** All 32 GPIO pins broken out on four 10-way headers (PORTA through PORTD)
- **LEDs:** Five LEDs on PB0 to PB4 (red, yellow, white, green and blue) each with 220Ω series resistor
- **Buzzer:** Active buzzer on PD3 for audio feedback
- **Button:** Push button on PD2 (INT0 interrupt pin) with 10kΩ pull-down to GND
- **Resistors:** 220Ω for LED current limiting and 10kΩ for button pull-down
- **Crystal:** 20MHz external oscillator with standard AVR load capacitors
- **UART:** Serial interface on J7 for debugging and data logging

---

## Project Progression

| Project | File | Description | Key Concepts |
|---------|------|-------------|--------------|
| 01_blink | 01_blink.c | Double blink on PB0 | DDRB, PORTB, _delay_ms |
| 02_led_cycle | 02_led_cycle.c | Five LEDs cycling sequentially | Multi-pin output control |
| 03_button_polling | 03_button_polling.c | Button controls buzzer via polling | PIND, input reading |
| 04_interrupt_buzzer | 04_interrupt_buzzer.c | Button controls buzzer via INT0 | ISR, EICRA, EIMSK, sei() |
| 05_state_machine | 05_state_machine.c | Full 9-mode state machine | enum, ISR, debounce, PWM, ADC |
| fuse_test | fuse_test.c | Fuse configuration reference | Fuse bits, clock configuration |

---

## Test_5 State Machine Modes

The final project implements a comprehensive state machine with nine modes accessible via button press. Each mode demonstrates different microcontroller capabilities and programming techniques.

| Mode | Name | Description |
|------|------|-------------|
| 0 | Chase | LEDs light up one by one in sequence left to right |
| 1 | Blink All | All five LEDs blink on and off together in unison |
| 2 | Alternate | Odd and even LEDs alternate between on and off |
| 3 | PWM Fade | All LEDs fade in and out smoothly using software PWM |
| 4 | Knight Rider | Single LED sweeps left to right and back in a smooth wave |
| 5 | Binary Counter | LEDs count from 0 to 31 in binary using 5-bit display |
| 6 | Random | LEDs light in random patterns using ADC noise as seed |
| 7 | Reaction Game | Press button when green LED lights up to win |
| 8 | Tetris Melody | Tetris theme plays with LEDs synced to notes |

---

## Build and Flash

### Microchip Studio Workflow

1. Open the project file (.cproj) in Microchip Studio 7
2. Press F7 to build the project
3. Go to Tools, then select Program with Pololu to flash via avrdude

### PlatformIO Workflow

1. Open the platformio folder in VS Code
2. Go to Terminal, select Run Task, then Build to compile
3. Go to Terminal, select Run Task, then Build and Upload to compile and flash

### Manual avrdude Command

```bash
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -B 10 -V \
  -U flash:w:.pio\build\ATmega644P\firmware.hex:i
```

### Fuse Restoration Command

If fuses are corrupted and the device cannot be programmed:

```bash
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -F \
  -U lfuse:w:0xFF:m -U hfuse:w:0xD1:m -U efuse:w:0xFF:m
```

---

## Key Embedded Systems Concepts

This project set covers fundamental bare metal programming techniques essential for embedded systems development:

- **GPIO Control:** Direct register manipulation using DDRX and PORTX
- **Input Reading:** Reading pin states using PINX registers
- **External Interrupts:** Hardware interrupt handling via INT0, EICRA, EIMSK
- **Interrupt Service Routines:** ISR() macro for interrupt-driven programming
- **Software Debouncing:** Eliminating mechanical switch bouncing with timing
- **State Machines:** Structured enum-based mode switching
- **Volatile Variables:** Shared state between ISR and main loop
- **Software PWM:** Bit-banging to create variable duty cycle outputs
- **ADC Noise:** Using ADC quantisation noise for pseudo-random number generation
- **Buzzer Tone Generation:** PWM-based frequency generation for audio
- **Project-based Learning:** Incremental complexity building towards real applications

---

## Tech Stack

<div align="center">

### Languages and Tools

| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" width="65" /> | <img src="https://raw.githubusercontent.com/zaccesss/zaccesss/main/icons/atmelavr.png" width="65" /> | <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/arduino/arduino-original.svg" width="65" /> |
| :------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------: | :------------------------------------------------------------------------------------------------------: |
|                                            **C**                                             |                                            **Atmel AVR**                                             |                                               **Arduino**                                                |

| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/vscode/vscode-original.svg" width="65" /> | <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/visualstudio/visualstudio-plain.svg" width="65" /> | <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/git/git-original.svg" width="65" /> |
| :----------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------: |
|                                              **VS Code**                                               |                                                **Visual Studio**                                                |                                             **Git**                                              |

| <img src="https://skillicons.dev/icons?i=bash" width="65" /> | <img src="https://skillicons.dev/icons?i=powershell" width="65" /> | <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/linux/linux-original.svg" width="65" /> |
| :----------------------------------------------------------: | :-----------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------: |
|                           **Bash**                           |                           **PowerShell**                            |                                              **Linux**                                               |

</div>

---

## Documentation

- [Wiring Guide](docs/wiring.md) - Complete breadboard wiring reference
- [Hardware Notes](docs/hardware_notes.md) - Fuse settings, power supply and crystal configuration
- [PCB Design](hardware/pcb_notes.md) - PCB overview, regulator, ISP programming and I/O headers

---

## Acknowledgements

**Richard Reeves**, lab technician and AVR mentor at Aston University, designed the AVR Project PCB, provided all components and resistors, and guided the entire project from initial setup through to interrupt-driven state machine development. His expertise and availability made professional bare metal programming possible.

**Andy Tanner**, lab technician at Aston University, stopped by during the session and shared knowledge about Nixie tube displays and microcontroller interfacing techniques.

**Petro**, lecturer at Aston University, provided advice during the session and will be teaching Analogue and Power Electronics in Year 2.

---

<p align="center">
  Made with dedication at Aston University | Electronic Engineering and Computer Science BEng 2028
</p>
