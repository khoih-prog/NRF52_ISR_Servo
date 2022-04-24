# NRF52_ISR_Servo Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/NRF52_ISR_Servo.svg?)](https://www.ardu-badge.com/NRF52_ISR_Servo)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/NRF52_ISR_Servo.svg)](https://github.com/khoih-prog/NRF52_ISR_Servo/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/NRF52_ISR_Servo/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/NRF52_ISR_Servo.svg)](http://github.com/khoih-prog/NRF52_ISR_Servo/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.0](#releases-v110)
  * [Releases v1.0.0](#releases-v100)

 
---
---

## Changelog

### Releases v1.2.0

1. Permit using servos with different pulse ranges simultaneously. Check [Fix Bug #5 : Permit using servos with different pulse ranges simultaneously #4](https://github.com/khoih-prog/RP2040_ISR_Servo/pull/4)
2. Delete left-over `cpp`

### Releases v1.1.0

1. Convert to `h-only` style.
2. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project
3. Optimize code by using passing by `reference` instead of by `value`
4. Add support to `Sparkfun Pro nRF52840 Mini`
5. Update `Packages_Patches`


### Releases v1.0.0

1. Basic 16 ISR-based servo controllers using 1 hardware PWM module for nRF52-based board
2. Support to both nRF52832 and nRF52840


