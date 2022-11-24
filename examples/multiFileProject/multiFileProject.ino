/****************************************************************************************************************************
  multiFileProject.ino
  For :
  - nRF52832-based boards such as AdaFruit Feather nRF52832,
  - nRF52840-based boards such as Adafruit nRF52840 Express, Itsy-Bitsy nRF52840 Express, NINA_B302_ublox, etc.

  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/NRF52_ISR_Servo
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#define NRF52_ISR_SERVO_VERSION_MIN_TARGET      "NRF52_ISR_Servo v1.2.1"
#define NRF52_ISR_SERVO_VERSION_MIN             1002001

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "NRF52_ISR_Servo.h"

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  Serial.println("\nStart multiFileProjects on ");
  Serial.println(BOARD_NAME);
  Serial.println(NRF52_ISR_SERVO_VERSION);

#if defined(NRF52_ISR_SERVO_VERSION_MIN)

  if (NRF52_ISR_SERVO_VERSION_INT < NRF52_ISR_SERVO_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(NRF52_ISR_SERVO_VERSION_MIN_TARGET);
  }

#endif
}

void loop()
{
  // put your main code here, to run repeatedly:
}
