# Atmel Studio 7 (Microchip Studio) Workflow

Source: AVR microcontrollers guide, Richard Reeves, Aston University, January 2024.

---

## Overview

Atmel Studio 7 (referred to as AS7, now rebranded as Microchip Studio) is the IDE used for
AVR C development on this project. It manages source files, compiler output, linker output and
project configuration in a structured hierarchy.

---

## File Types

| Extension | Type          | Description                                                   |
| --------- | ------------- | ------------------------------------------------------------- |
| `.c`      | Source file   | Your C programme code; the file you edit                      |
| `.h`      | Header file   | Library and register definitions included by your source file |
| `.cproj`  | Project file  | AS7 project configuration; do not edit manually               |
| `.atsln`  | Solution file | Top-level solution containing multiple projects               |
| `.hex`    | Output file   | Compiled firmware ready for flashing                          |
| `.elf`    | Output file   | Linked executable with debug information                      |
| `.pdsprj` | Proteus file  | Circuit simulation file; save in the project Debug folder     |

---

## Folder Structure

AS7 organises files in a two-level hierarchy: a Solution contains one or more Projects.

```
Solution (avr-zac.atsln)
├── Project 1 (01_blink)
│   ├── 01_blink.c                source file
│   └── Debug/                    compiled outputs
├── Project 2 (02_led_cycle)
│   ├── 02_led_cycle.c
│   └── Debug/
└── Project N ...
```

The solution file lives in the root folder. Each project is a sub-folder within that root.
Proteus simulation files should be saved inside the project's `Debug` folder.

---

## Naming Convention

Projects are named using a number prefix followed by a short description matching the source file name.

```
Format: NN_description
Example: 01_blink
```

---

## Setting Up a New Solution

1. Create a folder for the solution on your local drive.
2. Open AS7 and select **File → Open → Project / Solution**.
3. Navigate to the folder and open the `.atsln` file.

---

## Creating a New Project

1. In Solution Explorer, right-click on the solution name.
2. Select **Add → New Project...**
3. In the Add New Project dialog:
   - Language: **C/C++** (leave as default)
   - Project type: **GCC C Executable Project**
   - Name: enter the project name using the naming convention above
   - Location: leave as the solution folder
4. Click **OK**.

---

## Selecting the Device

After naming the project, AS7 opens the Device Selection screen.

Check the white blob on the IC to identify which variant is fitted:

| Marking            | Device to Select |
| ------------------ | ---------------- |
| No white blob      | ATmega164P       |
| White blob present | ATmega164A       |

For the ATmega644P used in this project, search `644p` and select **ATmega644P**.

---

## Renaming the Source File

AS7 generates a `main.c` file automatically. Rename it to match the project name:

1. Right-click `main.c` in Solution Explorer.
2. Select **Rename** (or press F2).
3. Change the name to match the project name; for example `01_blink.c`.
4. Ensure the `.c` extension is kept. If the syntax highlighting disappears and text turns black,
   the extension has been accidentally removed.

---

## Building and Flashing

| Action | Steps                                                                    |
| ------ | ------------------------------------------------------------------------ |
| Build  | Press **F7** or select **Build > Build Solution**                        |
| Flash  | Use **Tools > Program with Pololu** (custom tool configured for avrdude) |

The compiled `.hex` file is placed in the `Debug` folder inside the project folder.

---

## Switching Between Projects

Right-click on the project you want to work on in Solution Explorer and select
**Set as StartUp Project**. The active project name is displayed in bold.

---

## Notes

- The solution file and project files are managed by AS7; do not edit `.atsln` or `.cproj` files manually.
- Build output files (`Debug/`) do not need to be committed to version control.
- Each project within the solution should contain one standalone `.c` file with its own `main()`.
