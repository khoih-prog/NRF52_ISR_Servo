/****************************************************************************************************************************
  NRF52_ISR_Servo.hpp
  For :
  - nRF52832-based boards such as AdaFruit Feather nRF52832, 
  - nRF52840-based boards such as Adafruit nRF52840 Express, Itsy-Bitsy nRF52840 Express, NINA_B302_ublox, etc.
  
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/NRF52_ISR_Servo
  Licensed under MIT license

  Version: 1.1.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      22/08/2021 Initial coding for nRF52832/nRF52840 boards
  1.1.0   K Hoang      03/03/2022 Convert to `h-only` style. Optimize code. Add support to `Sparkfun Pro nRF52840 Mini`
 *****************************************************************************************************************************/

#pragma once

#ifndef NRF52_ISR_Servo_HPP
#define NRF52_ISR_Servo_HPP

#if !(defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
      defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || \
      defined(NRF52840_CLUE) || defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || \
      defined(MDBT50Q_RX) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
  #error This code is designed to run on nRF52 platform! Please check your Tools->Board setting.
#endif

#include "Arduino.h"
#include <Adafruit_TinyUSB.h>

#if !defined(NRF52_ISR_SERVO_VERSION)
  #define NRF52_ISR_SERVO_VERSION             "NRF52_ISR_Servo v1.1.0"
  
  #define NRF52_ISR_SERVO_VERSION_MAJOR       1
  #define NRF52_ISR_SERVO_VERSION_MINOR       1
  #define NRF52_ISR_SERVO_VERSION_PATCH       0

  #define NRF52_ISR_SERVO_VERSION_INT         1001000
  
#endif

//////////////////////////////////////////////////////////////////

#include <stddef.h>

#include <inttypes.h>

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

#include "NRF52_ISR_Servo_Debug.h"

//////////////////////////////////////////////////

#define NRF52_MAX_PIN           NUM_DIGITAL_PINS
#define NRF52_WRONG_PIN         255

// From Servo.h - Copyright (c) 2009 Michael Margolis.  All right reserved.

#define MIN_PULSE_WIDTH         800       // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH         2450      // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH     1500      // default pulse width when servo is attached
#define REFRESH_INTERVAL        20000     // minumim time to refresh servos in microseconds 

/**
 * NRF52 Only definitions
 * ---------------------
 */
//PWM_PRESCALER_PRESCALER_DIV_128 -> NRF_PWM_CLK_125kHz -> resolution 8µs
//MaxValue = 2450 -> PWM period = 20ms
//20ms - 50Hz
#define DUTY_CYCLE_RESOLUTION     8
#define MAXVALUE                  MAX_PULSE_WIDTH
#define CLOCKDIV                  PWM_PRESCALER_PRESCALER_DIV_128

enum
{
  SERVO_TOKEN = 0x76726553 // 'S' 'e' 'r' 'v'
};

class NRF52_ISR_Servo
{
  public:
    // maximum number of servos
    const static int MAX_SERVOS = 16;

    // constructor
    NRF52_ISR_Servo();

    // destructor
    ~NRF52_ISR_Servo()
    {
    }
    
    //////////////////////////////////////////////////

    // Bind servo to the timer and pin, return servoIndex
    int8_t setupServo(const uint8_t& pin, const uint16_t& minUs = MIN_PULSE_WIDTH, const uint16_t& maxUs = MAX_PULSE_WIDTH, int value = 0);
    
    // if value is < MIN_PULSE_WIDTH its treated as an angle, otherwise as pulse width in microseconds
    void write(const uint8_t& servoIndex, uint16_t value);
    
    // Write pulse width in microseconds
    void writeMicroseconds(const uint8_t& servoIndex, uint16_t value);

    // setPosition will set servo to position in degrees
    // by using PWM, turn HIGH 'duration' microseconds within REFRESH_INTERVAL (20000us)
    // returns true on success or -1 on wrong servoIndex
    bool setPosition(const uint8_t& servoIndex, uint16_t position);

    // returns last position in degrees if success, or -1 on wrong servoIndex
    int getPosition(const uint8_t& servoIndex);

    // setPulseWidth will set servo PWM Pulse Width in microseconds, correcponding to certain position in degrees
    // by using PWM, turn HIGH 'pulseWidth' microseconds within REFRESH_INTERVAL (20000us)
    // min and max for each individual servo are enforced
    // returns true on success or -1 on wrong servoIndex
    bool setPulseWidth(const uint8_t& servoIndex, uint16_t& pulseWidth);

    // returns pulseWidth in microsecs (within min/max range) if success, or 0 on wrong servoIndex
    unsigned int getPulseWidth(const uint8_t& servoIndex);

    // destroy the specified servo
    void deleteServo(const uint8_t& servoIndex);

    // returns true if the specified servo is enabled
    bool isEnabled(const uint8_t& servoIndex);

    // enables the specified servo
    bool enable(const uint8_t& servoIndex);

    // disables the specified servo
    bool disable(const uint8_t& servoIndex);

    // enables all servos
    void enableAll();

    // disables all servos
    void disableAll();

    // enables the specified servo if it's currently disabled,
    // and vice-versa
    bool toggle(const uint8_t& servoIndex);

    // returns the number of used servos
    int8_t getNumServos();

    // returns the number of available servos
    int8_t getNumAvailableServos() 
    {
      if (numServos <= 0)
        return MAX_SERVOS;
      else 
        return MAX_SERVOS - numServos;
    };

  private:

    void init()
    {
      for (int8_t servoIndex = 0; servoIndex < MAX_SERVOS; servoIndex++)
      {
        memset((void*) &servo[servoIndex], 0, sizeof (servo_t));
        //servo[servoIndex].count    = 0;
        servo[servoIndex].enabled  = false;
        // Intentional bad pin
        servo[servoIndex].pin      = NRF52_WRONG_PIN;
      }

      numServos   = 0;
    }
    
    //////////////////////////////////////////////////

    // find the first available slot
    int8_t findFirstFreeSlot();
    
    //////////////////////////////////////////////////
    
    uint16_t      _minUs;
    uint16_t      _maxUs;

    typedef struct
    {
      uint8_t       pin;                  // pin servo connected to
      int           position;             // In degrees
      bool          enabled;              // true if enabled
      
      HardwarePWM* pwm;
    } servo_t;
    
    //////////////////////////////////////////////////
    
    volatile servo_t servo[MAX_SERVOS];

    // actual number of servos in use (-1 means uninitialized)
    volatile int8_t numServos;
};


#endif    // NRF52_ISR_Servo_HPP
