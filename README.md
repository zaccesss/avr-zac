# AVR Zac

<p align="center">
  <a href="https://isaacadjei.me">
    <img src="https://img.shields.io/badge/Website-isaacadjei.me-111111?style=for-the-badge&logo=firefox&logoColor=white">
  </a>
  <a href="https://www.linkedin.com/in/isaacadjei">
    <img src="https://img.shields.io/badge/LinkedIn-Isaac_Adjei-0a66c2?style=for-the-badge&logo=linkedin&logoColor=white">
  </a>
  <a href="mailto:contact@zacess.com">
    <img src="https://img.shields.io/badge/Email-Contact-ff6f61?style=for-the-badge&logo=gmail&logoColor=white">
  </a>
  <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge">
</p>

<p align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=22&pause=1200&color=EE1C25&center=true&vCenter=true&width=600&height=55&lines=ATmega644P+Bare+Metal+C;State+Machines+%7C+Interrupts+%7C+PWM;ADC+%7C+Reaction+Game+%7C+Tetris+Melody;Aston+University+EE1EPJ+2025" />
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C-Language-A8B9CC?style=for-the-badge&logo=c&logoColor=black" />
  <img src="https://img.shields.io/badge/PlatformIO-Embedded-FF7F00?style=for-the-badge&logo=platformio&logoColor=white" />
  <img src="https://img.shields.io/badge/AVR-ATmega644P-EE1C25?style=for-the-badge&logo=microchip&logoColor=white" />
  <img src="https://img.shields.io/badge/VS_Code-IDE-007ACC?style=for-the-badge&logo=visualstudiocode&logoColor=white" />
  <img src="https://img.shields.io/badge/Microchip_Studio-7-0071C5?style=for-the-badge&logo=microchip&logoColor=white" />
  <img src="https://img.shields.io/badge/Aston_University-BEng_EECS_2028-6C2E7F?style=for-the-badge&logo=academia&logoColor=white" />
</p>

---

<p align="center">
  🔎 <b>Quick navigation:</b>
  <a href="#overview">Overview</a> •
  <a href="#hardware">Hardware</a> •
  <a href="#projects">Projects</a> •
  <a href="#state-machine-modes">State Machine</a> •
  <a href="#documentation-hub">Docs</a> •
  <a href="#tech-stack">Tech Stack</a> •
  <a href="#acknowledgements">Acknowledgements</a>
</p>

---

<a id="overview"></a>

## Overview

Bare metal AVR C projects developed during Year 1 of the BEng Electronic Engineering and Computer Science at Aston University (EE1EPJ). The work spans nine lab sessions and progresses from a basic LED blink through GPIO manipulation, polling, interrupt-driven input, software PWM and ADC, finishing with a full nine-mode state machine that includes a reaction game and a Tetris melody. Every project targets the ATmega644P running at 20 MHz on a custom PCB designed by Richard Reeves.

The `notes/` folder contains session-by-session reference notes adapted from the EE1EPJ lecture materials to match the exact hardware and programmer used here.

---

<a id="hardware"></a>

## Hardware

| Item       | Detail                                                    |
| ---------- | --------------------------------------------------------- |
| MCU        | ATmega644P DIP-40, 20 MHz external crystal                |
| PCB        | Richard Reeves AVR Project PCB 2018 with LM317T regulator |
| Programmer | Pololu USB AVR Programmer v2.1 via STK500v2 on COM4       |

The breadboard components (LEDs, button, buzzer) are a **temporary configuration** used for learning and change between sessions. See [docs/wiring.md](docs/wiring.md) for the current breadboard wiring.

---

<a id="projects"></a>

## Projects

| # | File                         | Description                             | Key Concepts                           |
| - | ---------------------------- | --------------------------------------- | -------------------------------------- |
| 1 | `01_blink.c`                 | Double blink on PB0                     | `DDRB`, `PORTB`, `_delay_ms`           |
| 2 | `02_led_cycle.c`             | Five LEDs cycling sequentially          | Multi-pin output                       |
| 3 | `03_button_polling.c`        | Button drives buzzer via polling        | `PIND`, input reading                  |
| 4 | `04_interrupt_buzzer.c`      | Button drives buzzer via INT0           | ISR, `EICRA`, `EIMSK`, `sei()`         |
| 5 | `05_state_machine_basic.c`   | Four-mode state machine (initial build) | `enum`, ISR, debounce, `switch`        |
| 6 | `06_state_machine.c`         | Nine-mode state machine (full build)    | PWM, ADC, reaction game, Tetris melody |
| — | `fuse_test.c`                | Fuse configuration reference            | Fuse bits, clock configuration         |

Source files live in both `projects/` (one folder per project) and `platformio/src/` (all files, multi-environment build). See [WORKFLOW.md](WORKFLOW.md) for how to switch between projects in VS Code.

---

<a id="state-machine-modes"></a>

## State Machine Modes

`06_state_machine.c` cycles through nine modes on each button press.

| Mode | Name           | Description                                      |
| ---- | -------------- | ------------------------------------------------ |
| 0    | Chase          | LEDs light one by one in sequence                |
| 1    | Blink All      | All five LEDs blink together                     |
| 2    | Alternate      | Odd and even LEDs alternate                      |
| 3    | PWM Fade       | All LEDs fade in and out via software PWM        |
| 4    | Knight Rider   | Single LED sweeps left to right and back         |
| 5    | Binary Counter | LEDs count 0–31 in binary                        |
| 6    | Random         | LEDs display random patterns seeded by ADC noise |
| 7    | Reaction Game  | Press button when green LED lights to win        |
| 8    | Tetris Melody  | Tetris theme plays with LEDs synced to each note |

---

<a id="documentation-hub"></a>

## Documentation Hub

<p align="center">
  <a href="WORKFLOW.md">⚙️ Build and Flash Workflow</a> &nbsp;•&nbsp;
  <a href="notes/">📖 Session Notes</a> &nbsp;•&nbsp;
  <a href="docs/wiring.md">🔌 Wiring Reference</a> &nbsp;•&nbsp;
  <a href="docs/hardware_notes.md">🔧 Hardware Notes</a> &nbsp;•&nbsp;
  <a href="docs/c_operators.md">📐 C Operators</a> &nbsp;•&nbsp;
  <a href="hardware/pcb_notes.md">🖥️ PCB Reference</a>
</p>

---

<a id="tech-stack"></a>

## Tech Stack

<div align="center">

| <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg" width="65" /> | <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/embeddedc/embeddedc-original.svg" width="65" /> | <img src="https://cdn.simpleicons.org/platformio" width="65" /> | <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/vscode/vscode-original.svg" width="65" /> | <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/visualstudio/visualstudio-plain.svg" width="65" /> | <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/git/git-original.svg" width="65" /> | <img src="https://techstack-generator.vercel.app/github-icon.svg" width="65" /> |
| :------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------: |
|                                            **C**                                             |                                                **Embedded C**                                                |                         **PlatformIO**                          |                                              **VS Code**                                               |                                              **Microchip Studio**                                               |                                             **Git**                                              |                                   **GitHub**                                    |

</div>

---

<a id="acknowledgements"></a>

## Acknowledgements

**Richard Reeves**, lab technician and AVR mentor at Aston University, designed the AVR Project PCB, provided components and guided the project from initial setup through to interrupt-driven state machine development.

**Andy Tanner**, lab technician at Aston University, shared knowledge about Nixie tube displays and microcontroller interfacing during a session.

**Petro**, lecturer at Aston University, provided advice during sessions and will teach Analogue and Power Electronics in Year 2.

---

## Contact and Support

Open an issue in this repository for questions or bugs.

You can also reach me directly at [contact@zacess.com](mailto:contact@zacess.com) or via [LinkedIn](https://www.linkedin.com/in/isaacadjei).

<p align="center">
  <b>Project Status:</b> Active — Year 1 EE1EPJ in progress<br>
  <b>Last Updated:</b> May 2026
</p>

<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=gradient&height=80&section=footer" />
</p>
