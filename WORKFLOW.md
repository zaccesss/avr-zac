# Build and Flash Workflow

This document covers how to build and flash projects in both VS Code (PlatformIO) and Microchip Studio 7. All projects target the ATmega644P via the Pololu USB AVR Programmer v2.1 on COM4.

---

## VS Code — PlatformIO

### Switching Between Projects

All project source files are in `platformio/src/`. The `platformio.ini` defines one environment per project. To switch the active project:

1. Open the `platformio/` folder in VS Code.
2. Click the **environment name in the status bar** (bottom left — shows the current env, e.g. `env:06_state_machine`).
3. Select the project you want from the list.

Available environments:

| Environment              | Source file                | Description                    |
| ------------------------ | -------------------------- | ------------------------------ |
| `01_blink`               | `01_blink.c`               | Double blink on PB0            |
| `02_led_cycle`           | `02_led_cycle.c`           | Five LEDs cycling              |
| `03_button_polling`      | `03_button_polling.c`      | Button polling                 |
| `04_interrupt_buzzer`    | `04_interrupt_buzzer.c`    | INT0 interrupt                 |
| `05_state_machine_basic` | `05_state_machine_basic.c` | Four-mode state machine        |
| `05_state_machine`       | `05_state_machine.c`       | Nine-mode state machine (v1)   |
| `06_state_machine`       | `06_state_machine.c`       | Nine-mode state machine (full) |
| `fuse_test`              | `fuse_test.c`              | Fuse configuration reference   |

### Running Tasks

Use **Terminal → Run Task** and choose from:

| Task             | Action                                  |
| ---------------- | --------------------------------------- |
| Build            | Compile the active environment only     |
| Upload           | Flash a previously compiled `.hex` file |
| Build and Upload | Compile then flash in a single step     |

The Build and Upload task runs:

```
pio run && C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -B 10 -V -U flash:w:.pio\build\ATmega644P\firmware.hex:i
```

### Manual avrdude Command

```
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -B 10 -V -U flash:w:.pio\build\ATmega644P\firmware.hex:i
```

Flag reference:

| Flag          | Meaning                                         |
| ------------- | ----------------------------------------------- |
| `-c stk500v2` | Pololu programmer protocol                      |
| `-p m644p`    | Target device: ATmega644P                       |
| `-P COM4`     | COM port for the Pololu programmer              |
| `-B 10`       | Slow ISP clock to ~50 kHz — required for Pololu |
| `-V`          | Skip verify after flash                         |

---

## Microchip Studio 7

### Opening a Project

1. Open the `.atsln` solution file for the project in Microchip Studio 7.
2. Right-click the project in Solution Explorer and set it as the StartUp Project if there are multiple projects in the solution.

### Building and Flashing

Click the **green play button** (or **Debug → Start Without Debugging**, `Ctrl+Alt+F5`). This builds and flashes in a single step — no separate build command is needed.

Check the Output panel at the bottom for errors (red circle) and warnings (yellow triangle). Do not ignore warnings.

### Programmer Setup

If the programmer is not yet configured:

1. Click the **hammer icon** in the toolbar to open tool selection.
2. Set **Selected debugger/programmer** to **STK500**.
3. Set **Interface** to **ISP**.

See [docs/atmel_studio_workflow.md](docs/atmel_studio_workflow.md) for the full first-time setup walkthrough.

### Simulation (No Hardware Required)

1. Set **Selected debugger/programmer** to **Simulator** in the hammer icon menu.
2. **Debug → Start Debugging and Break** (`Alt+F5`).
3. Set breakpoints with **F9**, step one line at a time with **F11**, run to next breakpoint with **F5**.
4. View port registers via **Debug → Windows → I/O**.

Simulation is useful for checking register state and logic flow. It does not simulate real timing or external hardware.

---

## Fuse Restoration

If fuse bytes are corrupted, restore them with:

```
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -F -U lfuse:w:0xFF:m -U hfuse:w:0xD1:m -U efuse:w:0xFF:m
```

| Fuse    | Value  | Effect                                  |
| ------- | ------ | --------------------------------------- |
| `lfuse` | `0xFF` | External crystal oscillator, full swing |
| `hfuse` | `0xD1` | JTAG disabled, SPI enabled, 2KB boot    |
| `efuse` | `0xFF` | Brown-out detection off                 |

See [docs/hardware_notes.md](docs/hardware_notes.md) for full fuse bit breakdown.

---

## Build Settings Reference

| Setting          | Value        | Reason                                          |
| ---------------- | ------------ | ----------------------------------------------- |
| `F_CPU`          | `20000000UL` | 20 MHz external crystal on this PCB             |
| Optimisation     | `-O0`        | Disabled — allows `_delay_ms` to work correctly |
| Upload protocol  | `stk500v2`   | Pololu USB AVR Programmer v2.1                  |
| ISP clock (`-B`) | `10`         | ~50 kHz — required to avoid Pololu timeouts     |
