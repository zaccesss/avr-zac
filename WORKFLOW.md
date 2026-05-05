# Build and Flash Workflow

This document covers everything needed to build, flash and debug projects in both VS Code (PlatformIO) and Microchip Studio 7. All projects target the ATmega644P via the Pololu USB AVR Programmer v2.1 on COM4.

---

## Prerequisites

### VS Code

| Requirement       | Where to get it                                      |
| ----------------- | ---------------------------------------------------- |
| VS Code           | https://code.visualstudio.com                        |
| PlatformIO IDE    | VS Code Extensions panel, search `platformio.platformio-ide` |
| avrdude           | Extract to `C:\avrdude\` so that `avrdude.exe` exists at that path |
| Pololu drivers    | Install via Pololu USB AVR Programmer v2.1 setup     |

Once PlatformIO is installed, open the `platformio/` folder in VS Code (not the repo root). PlatformIO reads `platformio.ini` from the folder you open.

### Microchip Studio 7

| Requirement       | Where to get it                                           |
| ----------------- | --------------------------------------------------------- |
| Microchip Studio 7 | https://www.microchip.com/en-us/tools-resources/develop/microchip-studio |
| Pololu drivers    | Install via Pololu USB AVR Programmer v2.1 setup          |

---

## VS Code - PlatformIO

### Opening the Project

1. Open VS Code.
2. **File → Open Folder** and select the `platformio/` folder inside this repo (not the repo root).
3. PlatformIO will detect `platformio.ini` and initialise automatically. This may take a moment on first open while it downloads the AVR toolchain.

### How Environments Work

`platformio.ini` defines one environment per project using `build_src_filter`. Each environment compiles only its own source file and ignores all others in `platformio/src/`. The `[common]` section holds shared settings (board, upload protocol, build flags) so they only need to be written once.

```ini
[env:06_state_machine]
extends = common
build_src_filter = -<*> +<06_state_machine.c>
```

`-<*>` excludes everything. `+<filename.c>` then adds back only the one file for that environment.

### Switching Between Projects

1. Look at the **status bar at the bottom left** of VS Code. It shows the currently active environment, for example `env:06_state_machine`.
2. Click it to open the environment picker.
3. Select the environment you want to build.

You can also switch from the **PlatformIO sidebar** (ant icon on the left): expand the environment name and click **Build** or **Upload** directly.

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

Tasks are defined in `platformio/.vscode/tasks.json`. The Build task calls `pio run` which respects the active environment selected in the status bar.

### Adding a New Project

1. Add the source `.c` file to `platformio/src/`.
2. Also add a copy to `projects/<name>/` to keep the projects folder consistent.
3. Add a new environment block to `platformio.ini`:

```ini
[env:my_new_project]
extends = common
build_src_filter = -<*> +<my_new_project.c>
```

4. Switch to the new environment from the status bar and run Build and Upload.

### Manual avrdude Command

```
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -B 10 -V -U flash:w:.pio\build\ATmega644P\firmware.hex:i
```

Flag reference:

| Flag          | Meaning                                        |
| ------------- | ---------------------------------------------- |
| `-c stk500v2` | Pololu programmer protocol                     |
| `-p m644p`    | Target device: ATmega644P                      |
| `-P COM4`     | COM port for the Pololu programmer             |
| `-B 10`       | Slow ISP clock to ~50 kHz, required for Pololu |
| `-V`          | Skip verify after flash                        |

---

## Microchip Studio 7

### First-Time Programmer Setup

This only needs to be done once per installation.

1. Connect the Pololu programmer to USB and to the ISP header on the PCB.
2. Open Microchip Studio 7.
3. Click the **hammer icon** (Select debugger/programmer) in the toolbar.
4. Set **Selected debugger/programmer** to **STK500**.
5. Set **Interface** to **ISP**.
6. Click **Apply**.

See [docs/atmel_studio_workflow.md](docs/atmel_studio_workflow.md) for the full first-time setup walkthrough including screenshots.

### Opening an Existing Project

1. **File → Open → Project/Solution** and select the `.atsln` file.
2. If the solution contains multiple projects, right-click the one you want in Solution Explorer and select **Set as StartUp Project**.

### Creating a New Project

1. **File → New → Project**.
2. Select **GCC C Executable Project** under the C/C++ category.
3. Name the project, choose a location and click **OK**.
4. In the device selection window, search for `ATmega644P` and select it. Click **OK**.
5. Microchip Studio creates `main.c` with an empty skeleton. Rename or replace as needed.
6. Right-click the project in Solution Explorer and go to **Properties** to verify:
   - **Tool** tab: STK500, ISP
   - **Toolchain → AVR/GNU C Compiler → Optimization**: set to `-O0` to allow `_delay_ms` to work correctly

### Building and Flashing

Click the **green play button** (or **Debug → Start Without Debugging**, `Ctrl+Alt+F5`). This compiles the code and flashes it to the chip in a single step. There is no need to run a separate build beforehand.

Check the **Output** panel at the bottom for errors (red circle) and warnings (yellow triangle). Do not ignore warnings.

### Checking Build Output Size

After a successful build, the Output panel shows the flash usage in bytes and as a percentage of the ATmega644P's 64KB flash. If flash usage exceeds roughly 90%, consider reviewing large arrays or lookup tables.

### Simulation (No Hardware Required)

Simulation runs the code on a virtual ATmega644P inside Microchip Studio without needing physical hardware. It is useful for checking register state and logic flow but does not simulate real timing, external inputs or hardware peripherals accurately.

1. Click the **hammer icon** and set **Selected debugger/programmer** to **Simulator**.
2. **Debug → Start Debugging and Break** (`Alt+F5`). Execution stops at the first line of `main`.
3. Set breakpoints by clicking in the left margin or pressing **F9** (red dot appears on that line).
4. **F5** runs to the next breakpoint. **F11** steps one line at a time. **F10** steps over function calls.
5. View port and peripheral registers via **Debug → Windows → I/O**. An empty box is logic 0 and a filled box is logic 1. Anything that changed on the last step is highlighted in red.

To return to hardware flashing after simulation, go back to the hammer icon and switch back to **STK500** / **ISP**.

---

## Fuse Restoration

If fuse bytes are corrupted or accidentally changed, restore them with:

```
C:\avrdude\avrdude.exe -c stk500v2 -p m644p -P COM4 -F -U lfuse:w:0xFF:m -U hfuse:w:0xD1:m -U efuse:w:0xFF:m
```

The `-F` flag overrides the signature check, which is needed when fuses have been set incorrectly and the device no longer responds normally.

| Fuse    | Value  | Effect                                  |
| ------- | ------ | --------------------------------------- |
| `lfuse` | `0xFF` | External crystal oscillator, full swing |
| `hfuse` | `0xD1` | JTAG disabled, SPI enabled, 2KB boot    |
| `efuse` | `0xFF` | Brown-out detection off                 |

See [docs/hardware_notes.md](docs/hardware_notes.md) for the full fuse bit breakdown.

---

## Build Settings Reference

| Setting          | Value        | Reason                                       |
| ---------------- | ------------ | -------------------------------------------- |
| `F_CPU`          | `20000000UL` | 20 MHz external crystal on this PCB          |
| Optimisation     | `-O0`        | Disabled so that `_delay_ms` works correctly |
| Upload protocol  | `stk500v2`   | Pololu USB AVR Programmer v2.1               |
| ISP clock (`-B`) | `10`         | ~50 kHz, required to avoid Pololu timeouts   |

---

## Troubleshooting

### avrdude: stk500v2_ReceiveMessage(): timeout

The ISP clock is too fast for the Pololu programmer. Ensure `-B 10` is present in the upload flags in `platformio.ini` and in any manual avrdude commands.

### avrdude: can't open device

The COM port is wrong or the Pololu is not connected. Check **Device Manager → Ports** to find the correct port and update `upload_port` in `platformio.ini` and the Upload task in `tasks.json`.

### No device found / signature mismatch

The board may not be powered. Verify the power LED on the PCB is on before attempting to flash.

### _delay_ms produces wrong timing

Optimisation is set too high. Ensure `-O0` is in `build_flags` in `platformio.ini` and in the Microchip Studio project properties under **Toolchain → Compiler → Optimization**.

### PlatformIO does not find the environment

Make sure you opened the `platformio/` folder in VS Code, not the repo root. PlatformIO requires `platformio.ini` to be in the root of the opened folder.
