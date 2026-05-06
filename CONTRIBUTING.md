# Contributing

Thanks for taking the time to contribute to this project. Whether you are correcting a mistake in the notes, improving a code comment, adding a lab task or fixing a wiring description, all contributions are welcome.

---

## Getting Started

1. Fork the repository and clone your fork.
2. Create a branch from `main` using the naming convention below.
3. Make your changes, following the style guide below.
4. Commit with a descriptive message using the format below.
5. Push and open a pull request against `main`.

---

## Branch Naming

Use lowercase with hyphens. Prefix with the type of change:

| Prefix | Use for |
| ------ | ------- |
| `feat/` | New project file, new lab task or new notes section |
| `fix/` | Correcting wrong information, broken links or code bugs |
| `docs/` | Rewrites, expansions or style improvements to existing notes |
| `refactor/` | Restructuring files or folders without changing content |
| `chore/` | Config files, build system, `.gitignore` or tooling |

Examples:

```
feat/session-09-spi-notes
fix/session-04-timer-calculation
docs/expand-session-06-uart-examples
chore/update-platformio-ini
```

---

## Commit Message Format

```
type: short description in present tense
```

Keep the subject line under 72 characters. Use the body for extra context if needed. Common types:

| Type | Use for |
| ---- | ------- |
| `feat` | New content or functionality |
| `fix` | Bug fix or factual correction |
| `docs` | Documentation only changes |
| `refactor` | Code or file restructure |
| `chore` | Build system, config or tooling |

Examples:

```
docs: add breadboard setup section to session 05 lab
fix: correct OCR0A calculation in session 05 notes
feat: add session 09 SPI notes and lab
chore: add ATmega644P datasheet to hardware/
```

---

## Style Guide

### Markdown files

- UK English throughout: initialise, behaviour, colour, programme.
- No em dashes (`—`) or en dashes (`–`). Use a colon for definitions, a semicolon or comma for connectors, or reword.
- No Oxford comma (no comma before the final "and" or "or" in a list of three or more items).
- Headings use sentence case, not title case.
- Tables should have aligned column separators.

### Code snippets in notes

- Every non-trivial line must have an inline comment explaining what it does and why, not just what the register is called.
- Use register names as defined in `<avr/io.h>` (e.g. `DDRB`, `TCCR0A`, `OCR0A`).
- Bit names must match the datasheet (e.g. `WGM01`, `COM0A1`).
- Optimisation flag must remain `-O0` for `_delay_ms` to work correctly.

### Source files

- One `.c` file lives in `platformio/src/` at a time. See [WORKFLOW.md](WORKFLOW.md) for how to switch.
- New learning projects go into `projects/learning_projects/<name>/`. Lab exercises go into `projects/lab_projects/`.
- File headers must include: filename, brief description, hardware target, build environment.

---

## Pull Request Checklist

Before opening a PR, confirm:

- [ ] Branch name follows the convention above
- [ ] Commit messages follow the format above
- [ ] No em or en dashes introduced in any Markdown file
- [ ] No UK English spelling errors (behaviour, initialise, colour, programme)
- [ ] All code snippets have thorough inline comments
- [ ] Links in Markdown files resolve to existing files
- [ ] If a new project file is added, the README projects table is updated
- [ ] PR title follows `type: short description`
- [ ] PR description explains what changed and why
- [ ] Related issue is linked with `closes #NUMBER` if applicable

---

## Reporting Issues

Use the issue templates in `.github/ISSUE_TEMPLATE/` for:

- **Bug reports**: wrong information, broken code, incorrect register names or calculations
- **Documentation requests**: missing content, unclear explanations or suggested additions

For questions about the hardware or build setup, check [WORKFLOW.md](WORKFLOW.md) and [docs/hardware_notes.md](docs/hardware_notes.md) first.
