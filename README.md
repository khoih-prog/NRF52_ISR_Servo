# NRF52_ISR_Servo Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/NRF52_ISR_Servo.svg?)](https://www.ardu-badge.com/NRF52_ISR_Servo)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/NRF52_ISR_Servo.svg)](https://github.com/khoih-prog/NRF52_ISR_Servo/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/NRF52_ISR_Servo/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/NRF52_ISR_Servo.svg)](http://github.com/khoih-prog/NRF52_ISR_Servo/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Important Change from v1.1.0](#Important-Change-from-v110)
* [Why do we need this NRF52_ISR_Servo library](#why-do-we-need-this-nrf52_isr_servo-library)
  * [Features](#features)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Packages' Patches](#packages-patches)
  * [1. For Adafruit nRF52840 and nRF52832 boards](#1-for-adafruit-nrf52840-and-nrf52832-boards)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error) 
* [What special in this NRF52_ISR_Servo library](#what-special-in-this-nrf52_isr_servo-library)
* [HOWTO Usage](#howto-usage)
* [Examples](#examples)
  * [ 1. nRF52_MultipleRandomServos](examples/nRF52_MultipleRandomServos)
  * [ 2. nRF52_MultipleServos](examples/nRF52_MultipleServos)
  * [ 3. **multiFileProject**](examples/multiFileProject) **New**
* [Example nRF52_MultipleServos](#example-nrf52_multipleservos)
  * [1. File nRF52_MultipleServos.ino](#1-file-nrf52_multipleservosino)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. nRF52_MultipleRandomServos on Adafruit NRF52840_FEATHER](#1-nrf52_multiplerandomservos-on-adafruit-nrf52840_feather)
  * [2. nRF52_MultipleRandomServos on Adafruit NRF52840_ITSYBITSY](#2-nRF52_multiplerandomservos-on-adafruit-nrf52840_itsybitsy)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Important Change from v1.1.0

Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)

---
---

### Why do we need this [NRF52_ISR_Servo library](https://github.com/khoih-prog/NRF52_ISR_Servo)

#### Features

Imagine you have a system with a **mission-critical function** controlling a **robot arm** or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is blocking the loop() or setup().

So your function might not be executed, and the result would be disastrous.

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a **Hardware PWM module** to generate the PWM pulses.

These hardware PWM modules, using interrupt, still work even if other functions are blocking. Moreover, they are **much more precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the **loop() or setup() is blocked by certain operation**. For example, certain function is blocking while it's connecting to WiFi or some services.

This library enables you to use `1 Hardware PWM module` on an nRF52-based board to control up to `16 independent servo motors`.


### Currently supported Boards

  - **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox etc.**
  - **Sparkfun Pro nRF52840 Mini**


---
---

## Prerequisites

1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
2. [`Adafruit nRF52 v1.3.0+`](https://github.com/adafruit/Adafruit_nRF52_Arduino) for nRF52 boards such as Adafruit NRF52840_FEATHER, NRF52832_FEATHER, NRF52840_FEATHER_SENSE, NRF52840_ITSYBITSY, NRF52840_CIRCUITPLAY, NRF52840_CLUE, NRF52840_METRO, NRF52840_PCA10056, PARTICLE_XENON, **NINA_B302_ublox**, etc. [![GitHub release](https://img.shields.io/github/release/adafruit/Adafruit_nRF52_Arduino.svg)](https://github.com/adafruit/Adafruit_nRF52_Arduino/releases/latest)

---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for `NRF52_ISR_Servo`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/NRF52_ISR_Servo.svg?)](https://www.ardu-badge.com/NRF52_ISR_Servo) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [NRF52_ISR_Servo](https://github.com/khoih-prog/NRF52_ISR_Servo) page.
2. Download the latest release `NRF52_ISR_Servo-main.zip`.
3. Extract the zip file to `NRF52_ISR_Servo-main` directory 
4. Copy whole `NRF52_ISR_Servo-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**NRF52_ISR_Servo** library](https://registry.platformio.org/libraries/khoih-prog/NRF52_ISR_Servo) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/NRF52_ISR_Servo/installation). Search for **NRF52_ISR_Servo** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### Packages' Patches

#### 1. For Adafruit nRF52840 and nRF52832 boards

**To be able to compile, run and automatically detect and display BOARD_NAME on nRF52840/nRF52832 boards**, you have to copy the whole [nRF52 1.3.0](Packages_Patches/adafruit/hardware/nrf52/1.3.0) directory into Adafruit nRF52 directory (~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0). 

Supposing the Adafruit nRF52 version is 1.3.0. These files must be copied into the directory:
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/1.3.0/cores/nRF5/Udp.h`**

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z
These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`**

---

To use `Sparkfun Pro nRF52840 Mini`, you must install `Packages_Patches` and use Adafruit nrf52 core v1.0.0+


---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include this `.hpp` file

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "NRF52_ISR_Servo.hpp"     //https://github.com/khoih-prog/NRF52_ISR_Servo
```

in many files. But be sure to use the following `.h` file **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "NRF52_ISR_Servo.h"           //https://github.com/khoih-prog/NRF52_ISR_Servo
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.

Have a look at the discussion in [Different behaviour using the src_cpp or src_h lib #80](https://github.com/khoih-prog/ESPAsync_WiFiManager/discussions/80)



---
---

### What special in this [NRF52_ISR_Servo library](https://github.com/khoih-prog/NRF52_ISR_Servo)

Now these new **16 ISR-based Servo controllers** just use one nRF52 Hardware PWM module. The number 16 is just arbitrarily chosen, and depending on application, you can increase that number to 32, 48, etc. without problem.

The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers

Therefore, their executions are not blocked by bad-behaving functions / tasks. This important feature is absolutely necessary for mission-critical tasks. 

The [**nRF52_MultipleServos**](examples/nRF52_MultipleServos) example, which controls 6 servos independently, will demonstrate the nearly perfect accuracy.

Being ISR-based servo controllers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

---
---

## HOWTO Usage

How to use:

https://github.com/khoih-prog/NRF52_ISR_Servo/blob/4260125278f38956f8fdeaca58249ecd80b5b437/examples/nRF52_MultipleRandomServos/nRF52_MultipleRandomServos.ino#L45-L175

---
---

### Examples: 

 1. [nRF52_MultipleRandomServos](examples/nRF52_MultipleRandomServos) 
 2. [nRF52_MultipleServos](examples/nRF52_MultipleServos)
 3. [**multiFileProject**](examples/multiFileProject) **New**
 
---

### Example [nRF52_MultipleServos](examples/nRF52_MultipleServos)


#### 1. File [nRF52_MultipleServos.ino](examples/nRF52_MultipleServos/nRF52_MultipleServos.ino)

https://github.com/khoih-prog/NRF52_ISR_Servo/blob/4260125278f38956f8fdeaca58249ecd80b5b437/examples/nRF52_MultipleServos/nRF52_MultipleServos.ino#L45-L126

---
---

### Debug Terminal Output Samples

### 1. nRF52_MultipleRandomServos on Adafruit NRF52840_FEATHER


```
Starting NRF52_MultipleRandomServos on NRF52840_FEATHER
NRF52_ISR_Servo v1.2.0
Setup OK Servo index = 0
Setup OK Servo index = 1
Setup OK Servo index = 2
Setup OK Servo index = 3
Setup OK Servo index = 4
Setup OK Servo index = 5
Servos @ 0 degree
Servos idx = 0, act. pos. (deg) = [ISR_SERVO] Idx = 0
800, pulseWidth (us) = [ISR_SERVO] Idx = 0
800
Servos idx = 1, act. pos. (deg) = [ISR_SERVO] Idx = 1
800, pulseWidth (us) = [ISR_SERVO] Idx = 1
800
Servos idx = 2, act. pos. (deg) = [ISR_SERVO] Idx = 2
800, pulseWidth (us) = [ISR_SERVO] Idx = 2
800
Servos idx = 3, act. pos. (deg) = [ISR_SERVO] Idx = 3
800, pulseWidth (us) = [ISR_SERVO] Idx = 3
800
Servos idx = 4, act. pos. (deg) = [ISR_SERVO] Idx = 4
800, pulseWidth (us) = [ISR_SERVO] Idx = 4
800
Servos idx = 5, act. pos. (deg) = [ISR_SERVO] Idx = 5
800, pulseWidth (us) = [ISR_SERVO] Idx = 5
800
Servos @ 90 degree
Servos idx = 0, act. pos. (deg) = [ISR_SERVO] Idx = 0
1625, pulseWidth (us) = [ISR_SERVO] Idx = 0
1625
Servos idx = 1, act. pos. (deg) = [ISR_SERVO] Idx = 1
1625, pulseWidth (us) = [ISR_SERVO] Idx = 1
1625
Servos idx = 2, act. pos. (deg) = [ISR_SERVO] Idx = 2
1625, pulseWidth (us) = [ISR_SERVO] Idx = 2
1625
Servos idx = 3, act. pos. (deg) = [ISR_SERVO] Idx = 3
1625, pulseWidth (us) = [ISR_SERVO] Idx = 3
1625
Servos idx = 4, act. pos. (deg) = [ISR_SERVO] Idx = 4
1625, pulseWidth (us) = [ISR_SERVO] Idx = 4
1625
Servos idx = 5, act. pos. (deg) = [ISR_SERVO] Idx = 5
1625, pulseWidth (us) = [ISR_SERVO] Idx = 5
1625
Servos @ 180 degree
Servos idx = 0, act. pos. (deg) = [ISR_SERVO] Idx = 0
2450, pulseWidth (us) = [ISR_SERVO] Idx = 0
2450
Servos idx = 1, act. pos. (deg) = [ISR_SERVO] Idx = 1
2450, pulseWidth (us) = [ISR_SERVO] Idx = 1
2450
Servos idx = 2, act. pos. (deg) = [ISR_SERVO] Idx = 2
2450, pulseWidth (us) = [ISR_SERVO] Idx = 2
2450
Servos idx = 3, act. pos. (deg) = [ISR_SERVO] Idx = 3
2450, pulseWidth (us) = [ISR_SERVO] Idx = 3
2450
Servos idx = 4, act. pos. (deg) = [ISR_SERVO] Idx = 4
2450, pulseWidth (us) = [ISR_SERVO] Idx = 4
2450
Servos idx = 5, act. pos. (deg) = [ISR_SERVO] Idx = 5
2450, pulseWidth (us) = [ISR_SERVO] Idx = 5
2450
Servos sweeps from 0-180 degree
```

---

### 2. nRF52_MultipleRandomServos on Adafruit NRF52840_ITSYBITSY

```
Starting NRF52_MultipleRandomServos on NRF52840_ITSYBITSY
NRF52_ISR_Servo v1.2.0
Setup OK Servo index = 0
Setup OK Servo index = 1
Setup OK Servo index = 2
Setup OK Servo index = 3
Setup OK Servo index = 4
Setup OK Servo index = 5
Servos @ 0 degree
Servos idx = 0, act. pos. (deg) = [ISR_SERVO] Idx = 0
800, pulseWidth (us) = [ISR_SERVO] Idx = 0
800
Servos idx = 1, act. pos. (deg) = [ISR_SERVO] Idx = 1
800, pulseWidth (us) = [ISR_SERVO] Idx = 1
800
Servos idx = 2, act. pos. (deg) = [ISR_SERVO] Idx = 2
800, pulseWidth (us) = [ISR_SERVO] Idx = 2
800
Servos idx = 3, act. pos. (deg) = [ISR_SERVO] Idx = 3
800, pulseWidth (us) = [ISR_SERVO] Idx = 3
800
Servos idx = 4, act. pos. (deg) = [ISR_SERVO] Idx = 4
800, pulseWidth (us) = [ISR_SERVO] Idx = 4
800
Servos idx = 5, act. pos. (deg) = [ISR_SERVO] Idx = 5
800, pulseWidth (us) = [ISR_SERVO] Idx = 5
800
Servos @ 90 degree
Servos idx = 0, act. pos. (deg) = [ISR_SERVO] Idx = 0
1625, pulseWidth (us) = [ISR_SERVO] Idx = 0
1625
Servos idx = 1, act. pos. (deg) = [ISR_SERVO] Idx = 1
1625, pulseWidth (us) = [ISR_SERVO] Idx = 1
1625
Servos idx = 2, act. pos. (deg) = [ISR_SERVO] Idx = 2
1625, pulseWidth (us) = [ISR_SERVO] Idx = 2
1625
Servos idx = 3, act. pos. (deg) = [ISR_SERVO] Idx = 3
1625, pulseWidth (us) = [ISR_SERVO] Idx = 3
1625
Servos idx = 4, act. pos. (deg) = [ISR_SERVO] Idx = 4
1625, pulseWidth (us) = [ISR_SERVO] Idx = 4
1625
Servos idx = 5, act. pos. (deg) = [ISR_SERVO] Idx = 5
1625, pulseWidth (us) = [ISR_SERVO] Idx = 5
1625
Servos @ 180 degree
Servos idx = 0, act. pos. (deg) = [ISR_SERVO] Idx = 0
2450, pulseWidth (us) = [ISR_SERVO] Idx = 0
2450
Servos idx = 1, act. pos. (deg) = [ISR_SERVO] Idx = 1
2450, pulseWidth (us) = [ISR_SERVO] Idx = 1
2450
Servos idx = 2, act. pos. (deg) = [ISR_SERVO] Idx = 2
2450, pulseWidth (us) = [ISR_SERVO] Idx = 2
2450
Servos idx = 3, act. pos. (deg) = [ISR_SERVO] Idx = 3
2450, pulseWidth (us) = [ISR_SERVO] Idx = 3
2450
Servos idx = 4, act. pos. (deg) = [ISR_SERVO] Idx = 4
2450, pulseWidth (us) = [ISR_SERVO] Idx = 4
2450
Servos idx = 5, act. pos. (deg) = [ISR_SERVO] Idx = 5
2450, pulseWidth (us) = [ISR_SERVO] Idx = 5
2450
Servos sweeps from 0-180 degree
```


---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level from 0 to 2. Be careful and using level 2 only for temporary debug purpose only.

```cpp
#define TIMER_INTERRUPT_DEBUG       1
#define ISR_SERVO_DEBUG             1
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [NRF52_ISR_Servo issues](https://github.com/khoih-prog/NRF52_ISR_Servo/issues)

---
---

## TO DO

1. Search for bug and improvement.

---

## DONE

1. Similar features for Arduino (UNO, Mega, etc...), ESP32, ESP8266, STM32, RP2040, SAMD21/SAMD51
2. Add functions `getPosition()` and `getPulseWidth()`
3. Optimize the code
4. Add complicated examples
5. Convert to `h-only` style.
6. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project
7. Optimize code by using passing by `reference` instead of by `value`
8. Add support to `Sparkfun Pro nRF52840 Mini`
9. Permit using servos with different pulse ranges simultaneously

---
---

### Contributions and thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library. Especially to these people who have directly or indirectly contributed to this [NRF52_ISR_Servo library](https://github.com/khoih-prog/NRF52_ISR_Servo)


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/NRF52_ISR_Servo/blob/main/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang
