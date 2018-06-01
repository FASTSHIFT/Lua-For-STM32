#ifndef __FILEGROUP_H
#define	__FILEGROUP_H

//*************C/C++ Library**************//

#include "stm32f10x.h"
#include "Arduino.h"
#include "delay.h"
#include "exti.h"
#include "gpio.h"
#include "adc.h"
#include "pwm.h"
#include "usart.h"
#include "time_exti.h"
#include "math.h"

#include "HardwareSerial.h"
#include "SPI.h"
#include "Wire.h"
#include "WString.h"
#include "Stream.h"

#include "C_call_CPP.h"
#include "lua.hpp"

int Lua_millis(lua_State *L);
int Lua_delay(lua_State *L);
int Lua_pinMode(lua_State *L);
int Lua_digitalWrite(lua_State *L);
int Lua_digitalRead(lua_State *L);
int Lua_analogWrite(lua_State *L);
int Lua_analogRead(lua_State *L);

#endif
