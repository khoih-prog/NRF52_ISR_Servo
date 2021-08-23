/****************************************************************************************************************************
  NRF52_ISR_Servo.cpp
  For :
  - nRF52832-based boards such as AdaFruit Feather nRF52832, 
  - nRF52840-based boards such as Adafruit nRF52840 Express, Itsy-Bitsy nRF52840 Express, NINA_B302_ublox, etc.
  
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/NRF52_ISR_Servo
  Licensed under MIT license

  Based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy

  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      22/08/2021 Initial coding for nRF52832/nRF52840 boards
 *****************************************************************************************************************************/

#include "NRF52_ISR_Servo.h"
#include <string.h>

#ifndef ISR_SERVO_DEBUG
  #define ISR_SERVO_DEBUG               1
#endif

#define DEFAULT_NRF52_TIMER_NO          NRF_TIMER_1       // NRF_TIMER_1 for many boards

NRF52_ISR_Servo NRF52_ISR_Servos;  // create servo object to control up to 16 servos

//////////////////////////////////////////////////

NRF52_ISR_Servo::NRF52_ISR_Servo() : numServos (-1)
{
}

//////////////////////////////////////////////////

// find the first available slot
// return -1 if none found
int NRF52_ISR_Servo::findFirstFreeSlot()
{
  // all slots are used
  if (numServos >= MAX_SERVOS)
    return -1;

  // return the first slot with no count (i.e. free)
  for (int servoIndex = 0; servoIndex < MAX_SERVOS; servoIndex++)
  {
    if (servo[servoIndex].enabled == false)
    {
      ISR_SERVO_LOGDEBUG1("Index =", servoIndex);

      return servoIndex;
    }
  }

  // no free slots found
  return -1;
}

//////////////////////////////////////////////////

int NRF52_ISR_Servo::setupServo(uint8_t pin, int minUs, int maxUs, int value)
{
  int servoIndex;
    
  if (pin > NRF52_MAX_PIN)
    return -1;

  if (numServos < 0)
    init();

  servoIndex = findFirstFreeSlot();
  
  if (servoIndex < 0)
    return -1;
    
  pinMode(pin, OUTPUT);
   
  _maxUs = maxUs;
  _minUs = minUs;
  
  servo[servoIndex].position = 0;

  numServos++;
  
  ///////////////////////////////////////////
  
  bool succeeded = false;
  
  // Adafruit, add pin to 1 of available Hw PWM
  // first, use existing HWPWM modules (already owned by Servo)
  for ( int i = 0; i < HWPWM_MODULE_NUM; i++ )
  {
    if ( HwPWMx[i]->isOwner(SERVO_TOKEN) && HwPWMx[i]->addPin(pin) )
    {
      servo[servoIndex].pwm = HwPWMx[i];
      succeeded = true;
      break;
    }
  }

  // if could not add to existing owned PWM modules, try to add to a new PWM module
  if ( !succeeded )
  {
    for ( int i = 0; i < HWPWM_MODULE_NUM; i++ )
    {
      if ( HwPWMx[i]->takeOwnership(SERVO_TOKEN) && HwPWMx[i]->addPin(pin) )
      {
        servo[servoIndex].pwm = HwPWMx[i];
        succeeded = true;
        break;
      }
    }
  }

  if ( succeeded )
  {
    pinMode(pin, OUTPUT);
    
    servo[servoIndex].pin     = pin;
    servo[servoIndex].enabled = true;
    
    servo[servoIndex].pwm->setMaxValue(MAXVALUE);
    servo[servoIndex].pwm->setClockDiv(CLOCKDIV);

    return servoIndex;
  }
  else
  {
    return -1;
  }
  
  ///////////////////////////////////////////
 
  ISR_SERVO_LOGDEBUG1("Index =", servoIndex);
  ISR_SERVO_LOGDEBUG3("min =", _minUs, ", max =", _maxUs);
  
  Serial.print("setupServo Index ="); Serial.println(servoIndex);
 
  return servoIndex;
}

/////////////////////////////////////////////////////

void NRF52_ISR_Servo::write(unsigned servoIndex, int value)
{
  // treat any value less than MIN_PULSE_WIDTH as angle in degrees (values equal or larger are handled as microseconds)
  if (value < MIN_PULSE_WIDTH)
  {
    // assumed to be 0-180 degrees servo
    value = constrain(value, 0, 180);
    value = map(value, 0, 180, _minUs, _maxUs);
  }
  
  writeMicroseconds(servoIndex, value);
}

/////////////////////////////////////////////////////

void NRF52_ISR_Servo::writeMicroseconds(unsigned servoIndex, int value) 
{
  value = constrain(value, _minUs, _maxUs);
  servo[servoIndex].position = value;

  if ( (servo[servoIndex].enabled) && (servo[servoIndex].pwm) )
  {
  	servo[servoIndex].pwm->writePin(servo[servoIndex].pin, value/DUTY_CYCLE_RESOLUTION);
  }
}

/////////////////////////////////////////////////////

bool NRF52_ISR_Servo::setPosition(unsigned servoIndex, int position)
{
  if (servoIndex >= MAX_SERVOS)
    return false;

  // Updates interval of existing specified servo
  if ( servo[servoIndex].enabled && (servo[servoIndex].pin <= NRF52_MAX_PIN) )
  {   
    // treat any value less than MIN_PULSE_WIDTH as angle in degrees (values equal or larger are handled as microseconds)
    if (position < MIN_PULSE_WIDTH) 
    {
      // assumed to be 0-180 degrees servo
      position = constrain(position, 0, 180);
      position = map(position, 0, 180, _minUs, _maxUs);
    }
    
    servo[servoIndex].position = position;
    
    writeMicroseconds(servoIndex, position);

    ISR_SERVO_LOGDEBUG3("Idx =", servoIndex, ", pos =",servo[servoIndex].position);

    return true;
  }

  // false return for non-used numServo or bad pin
  return false;
}

//////////////////////////////////////////////////

// returns last position in degrees if success, or -1 on wrong servoIndex
int NRF52_ISR_Servo::getPosition(unsigned servoIndex)
{
  if (servoIndex >= MAX_SERVOS)
    return -1;

  // Updates interval of existing specified servo
  if ( servo[servoIndex].enabled && (servo[servoIndex].pin <= NRF52_MAX_PIN) )
  {
    ISR_SERVO_LOGERROR3("Idx =", servoIndex, ", pos =",servo[servoIndex].position);

    return (servo[servoIndex].position);
  }

  // return 0 for non-used numServo or bad pin
  return -1;
}

//////////////////////////////////////////////////

// setPulseWidth will set servo PWM Pulse Width in microseconds, correcponding to certain position in degrees
// by using PWM, turn HIGH 'pulseWidth' microseconds within REFRESH_INTERVAL (20000us)
// min and max for each individual servo are enforced
// returns true on success or -1 on wrong servoIndex
bool NRF52_ISR_Servo::setPulseWidth(unsigned servoIndex, unsigned int pulseWidth)
{
  if (servoIndex >= MAX_SERVOS)
    return false;

  // Updates interval of existing specified servo
  if ( servo[servoIndex].enabled && (servo[servoIndex].pin <= NRF52_MAX_PIN) )
  {
    if (pulseWidth < _minUs)
      pulseWidth = _minUs;
    else if (pulseWidth > _maxUs)
      pulseWidth = _maxUs;

    servo[servoIndex].position  = map(pulseWidth, _minUs, _maxUs, 0, 180);

    ISR_SERVO_LOGERROR3("Idx =", servoIndex, ", pos =",servo[servoIndex].position);

    return true;
  }

  // false return for non-used numServo or bad pin
  return false;
}

//////////////////////////////////////////////////

// returns pulseWidth in microsecs (within min/max range) if success, or 0 on wrong servoIndex
unsigned int NRF52_ISR_Servo::getPulseWidth(unsigned servoIndex)
{
  if (servoIndex >= MAX_SERVOS)
    return 0;

  // Updates interval of existing specified servo
  if ( servo[servoIndex].enabled && (servo[servoIndex].pin <= NRF52_MAX_PIN) )
  {
    ISR_SERVO_LOGERROR3("Idx =", servoIndex, ", pos =",servo[servoIndex].position);

    return (servo[servoIndex].position);
  }

  // return 0 for non-used numServo or bad pin
  return 0;
}

//////////////////////////////////////////////////

void NRF52_ISR_Servo::deleteServo(unsigned servoIndex)
{
  if ( (numServos == 0) || (servoIndex >= MAX_SERVOS) )
  {
    return;
  }

  // don't decrease the number of servos if the specified slot is already empty
  if (servo[servoIndex].enabled)
  {
    memset((void*) &servo[servoIndex], 0, sizeof (servo_t));

    servo[servoIndex].enabled   = false;
    servo[servoIndex].position  = 0;

    // Intentional bad pin, good only from 0-16 for Digital, A0=17
    servo[servoIndex].pin       = NRF52_WRONG_PIN;

    // update number of servos
    numServos--;
    
    // remove pin from HW PWM
    HardwarePWM * pwm = servo[servoIndex].pwm;
    servo[servoIndex].pwm = nullptr;
    pwm->removePin(servo[servoIndex].pin);
    
    if (pwm->usedChannelCount() == 0) 
    {
      pwm->stop(); // disables peripheral so can release ownership
      pwm->releaseOwnership(SERVO_TOKEN);
    }
  }
}

//////////////////////////////////////////////////

bool NRF52_ISR_Servo::isEnabled(unsigned servoIndex)
{
  if (servoIndex >= MAX_SERVOS)
    return false;

  if (servo[servoIndex].pin > NRF52_MAX_PIN)
  {
    // Disable if something wrong
    servo[servoIndex].pin     = NRF52_WRONG_PIN;
    servo[servoIndex].enabled = false;
    return false;
  }

  return servo[servoIndex].enabled;
}

//////////////////////////////////////////////////

bool NRF52_ISR_Servo::enable(unsigned servoIndex)
{
  if (servoIndex >= MAX_SERVOS)
    return false;

  if (servo[servoIndex].pin > NRF52_MAX_PIN)
  {
    // Disable if something wrong
    servo[servoIndex].pin     = NRF52_WRONG_PIN;
    servo[servoIndex].enabled = false;
    return false;
  }

  if ( servo[servoIndex].position >= _minUs )
    servo[servoIndex].enabled = true;

  return true;
}

//////////////////////////////////////////////////

bool NRF52_ISR_Servo::disable(unsigned servoIndex)
{
  if (servoIndex >= MAX_SERVOS)
    return false;

  if (servo[servoIndex].pin > NRF52_MAX_PIN)
    servo[servoIndex].pin     = NRF52_WRONG_PIN;

  servo[servoIndex].enabled = false;

  return true;
}

//////////////////////////////////////////////////

void NRF52_ISR_Servo::enableAll()
{
  // Enable all servos with a enabled and count != 0 (has PWM) and good pin

  for (int servoIndex = 0; servoIndex < MAX_SERVOS; servoIndex++)
  {
    if ( (servo[servoIndex].position >= _minUs) && !servo[servoIndex].enabled 
      && (servo[servoIndex].pin <= NRF52_MAX_PIN) )
    {
      servo[servoIndex].enabled = true;
    }
  }
}

//////////////////////////////////////////////////

void NRF52_ISR_Servo::disableAll()
{
  // Disable all servos
  for (int servoIndex = 0; servoIndex < MAX_SERVOS; servoIndex++)
  {
    servo[servoIndex].enabled = false;
  }
}

//////////////////////////////////////////////////

bool NRF52_ISR_Servo::toggle(unsigned servoIndex)
{
  if (servoIndex >= MAX_SERVOS)
    return false;

  servo[servoIndex].enabled = !servo[servoIndex].enabled;

  return true;
}

//////////////////////////////////////////////////

int NRF52_ISR_Servo::getNumServos()
{
  return numServos;
}
