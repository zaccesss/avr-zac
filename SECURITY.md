# Security Policy

## Scope

This repository holds bare metal AVR C firmware, hardware documentation and personal study notes. It carries no runtime, no network access and no dependencies executed against real data, so the attack surface is narrow. Security issues that fall within scope are:

- Accidental inclusion of secrets, tokens or credentials in tracked files
- A firmware bug that could cause genuine hardware damage if flashed as-is (for example a fuse setting that bricks the microcontroller)

## Reporting a vulnerability

If you find a security issue in this repository, please do not open a public issue. Instead, contact me via:

- Email: contact@isaacadjei.me
- Contact form: https://isaacadjei.me/contact

Please include:

- A description of the issue
- Steps to reproduce
- Potential impact

You can expect a response within 72 hours.

## The shared policy

> [!NOTE]
> The full policy, covering scope, the disclosure process and expected response times, is in [zaccesss/security-policy](https://github.com/zaccesss/security-policy) and on [my site](https://isaacadjei.me/security-policy). This file takes precedence where the two differ.
