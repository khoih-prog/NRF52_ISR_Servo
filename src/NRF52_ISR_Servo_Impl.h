/****************************************************************************************************************************
  NRF52_ISR_Servo_Impl.h
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

#ifndef NRF52_ISR_SERVO_IMPL_H
#define NRF52_ISR_SERVO_IMPL_H

#include <string.h>

#ifndef ISR_SERVO_DEBUG
  #define ISR_SERVO_DEBUG               1
#endif

#define DEFAULT_NRF52_TIMER_NO          NRF_TIMER_1       // NRF_TIMER_1 for many boards

static NRF52_ISR_Servo NRF52_ISR_Servos;  // create servo object to control up to 16 servos

//////////////////////////////////////////////////

NRF52_ISR_Servo::NRF52_ISR_Servo() : numServos (-1)
{
}

//////////////////////////////////////////////////

// find the first available slot
// return -1 if none found
int8_t NRF52_ISR_Servo::findFirstFreeSlot()
{
  // all slots are used
  if (numServos >= MAX_SERVOS)
    return -1;

  // return the first slot with no count (i.e. free)
  for (int8_t servoIndex = 0; servoIndex < MAX_SERVOS; servoIndex++)
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

int8_t NRF52_ISR_Servo::setupServo(const uint8_t& pin, const uint16_t& minUs, const uint16_t& maxUs, int value)
{
  int8_t servoIndex;
    
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

void NRF52_ISR_Servo::write(const uint8_t& servoIndex, uint16_t value)
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

void NRF52_ISR_Servo::writeMicroseconds(const uint8_t& servoIndex, uint16_t value) 
{
  value = constrain(value, _minUs, _maxUs);
  servo[servoIndex].position = value;

  if ( (servo[servoIndex].enabled) && (servo[servoIndex].pwm) )
  {
  	servo[servoIndex].pwm->writePin(servo[servoIndex].pin, value/DUTY_CYCLE_RESOLUTION);
  }
}

/////////////////////////////////////////////////////

bool NRF52_ISR_Servo::setPosition(const uint8_t& servoIndex, uint16_t position)
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
int NRF52_ISR_Servo::getPosition(const uint8_t& servoIndex)
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
bool NRF52_ISR_Servo::setPulseWidth(const uint8_t& servoIndex, uint16_t& pulseWidth)
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
unsigned int NRF52_ISR_Servo::getPulseWidth(const uint8_t& servoIndex)
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

void NRF52_ISR_Servo::deleteServo(const uint8_t& servoIndex)
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

bool NRF52_ISR_Servo::isEnabled(const uint8_t& servoIndex)
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

bool NRF52_ISR_Servo::enable(const uint8_t& servoIndex)
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

bool NRF52_ISR_Servo::disable(const uint8_t& servoIndex)
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

  for (int8_t servoIndex = 0; servoIndex < MAX_SERVOS; servoIndex++)
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
  for (int8_t servoIndex = 0; servoIndex < MAX_SERVOS; servoIndex++)
  {
    servo[servoIndex].enabled = false;
  }
}

//////////////////////////////////////////////////

bool NRF52_ISR_Servo::toggle(const uint8_t& servoIndex)
{
  if (servoIndex >= MAX_SERVOS)
    return false;

  servo[servoIndex].enabled = !servo[servoIndex].enabled;

  return true;
}

//////////////////////////////////////////////////

int8_t NRF52_ISR_Servo::getNumServos()
{
  return numServos;
}

#endif    // NRF52_ISR_SERVO_IMPL_H
