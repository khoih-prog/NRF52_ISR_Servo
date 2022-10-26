/****************************************************************************************************************************
  NRF52_ISR_Servo_Debug.h
  For :
  - nRF52832-based boards such as AdaFruit Feather nRF52832,
  - nRF52840-based boards such as Adafruit nRF52840 Express, Itsy-Bitsy nRF52840 Express, NINA_B302_ublox, etc.

  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/NRF52_ISR_Servo
  Licensed under MIT license

  Version: 1.2.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      22/08/2021 Initial coding for nRF52832/nRF52840 boards
  1.1.0   K Hoang      03/03/2022 Convert to `h-only` style. Optimize code. Add support to `Sparkfun Pro nRF52840 Mini`
  1.2.0   K Hoang      23/04/2022 Permit using servos with different pulse ranges simultaneously. Delete left-over `cpp`
  1.2.1   K Hoang      26/10/2022 Add support to Seeed_XIAO_NRF52840 and Seeed_XIAO_NRF52840_SENSE
 *****************************************************************************************************************************/

#pragma once

#ifndef NRF52_ISR_Servo_Debug_h
#define NRF52_ISR_Servo_Debug_h

#include <Adafruit_TinyUSB.h>

//////////////////////////////////////////

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG         1
#endif

#ifndef ISR_SERVO_DEBUG
  #define ISR_SERVO_DEBUG               1
#endif

//////////////////////////////////////////

#if !defined(ISR_SERVO_DEBUG_OUTPUT)
  #define ISR_SERVO_DEBUG_OUTPUT    Serial
#endif

//////////////////////////////////////////////////////

const char ISR_SERVO_MARK[]  = "[ISR_SERVO] ";
const char ISR_SERVO_SP[]    = " ";

#define ISR_SERVO_PRINT          ISR_SERVO_DEBUG_OUTPUT.print
#define ISR_SERVO_PRINTLN        ISR_SERVO_DEBUG_OUTPUT.println
#define ISR_SERVO_FLUSH          ISR_SERVO_DEBUG_OUTPUT.flush

#define ISR_SERVO_PRINT_MARK     ISR_SERVO_PRINT(ISR_SERVO_MARK)
#define ISR_SERVO_PRINT_SP       ISR_SERVO_PRINT(ISR_SERVO_SP)

//////////////////////////////////////////////////////

#define ISR_SERVO_LOGERROR(x)         if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINTLN(x); }
#define ISR_SERVO_HEXLOGERROR(x)      if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINTLN(x, HEX); }
#define ISR_SERVO_LOGERROR0(x)        if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT(x); }
#define ISR_SERVO_HEXLOGERROR0(x)     if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT(x, HEX); }
#define ISR_SERVO_LOGERROR1(x,y)      if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(y); }
#define ISR_SERVO_HEXLOGERROR1(x,y)   if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(y, HEX); }
#define ISR_SERVO_LOGERROR2(x,y,z)    if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(z); }
#define ISR_SERVO_LOGERROR3(x,y,z,w)  if(ISR_SERVO_DEBUG>0) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(z); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(w); }

//////////////////////////////////////////////////////

#define ISR_SERVO_LOGDEBUG(x)         if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINTLN(x); }
#define ISR_SERVO_HEXLOGDEBUG(x)      if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINTLN(x, HEX); }
#define ISR_SERVO_LOGDEBUG0(x)        if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT(x); }
#define ISR_SERVO_HEXLOGDEBUG0(x)     if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT(x, HEX); }
#define ISR_SERVO_LOGDEBUG1(x,y)      if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(y); }
#define ISR_SERVO_HEXLOGDEBUG1(x,y)   if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(y, HEX); }
#define ISR_SERVO_LOGDEBUG2(x,y,z)    if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(z); }
#define ISR_SERVO_LOGDEBUG3(x,y,z,w)  if(ISR_SERVO_DEBUG>1) { ISR_SERVO_PRINT_MARK; ISR_SERVO_PRINT(x); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(y); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINT(z); ISR_SERVO_PRINT_SP; ISR_SERVO_PRINTLN(w); }

//////////////////////////////////////////


#endif      // NRF52_ISR_Servo_Debug_h
