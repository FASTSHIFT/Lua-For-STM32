#include "FileGroup.h"

LTDC_Graphic tft(320,240);

uint8_t LuaRunning=0;

void Lua_Stop()
{
	if(LuaRunning)
	{
		if(Serial.read()=='C')
		{
			NVIC_SystemReset();
		}
		else Serial.flush();
	}
}

int main(void)
{
	Delay_Init();
	ADCx_Init(ADC1);
	tft.init();
	tft.setBackColor(LCD_COLOR_BLACK);
	tft.setTextColor(LCD_COLOR_WHITE);
	tft.setFont(&Font8x12);
	tft.clear();
	
	Serial.begin(115200);
	Serial.println("\r\nInitializing...");
	tft.println("Initializing...");
	Serial.setTimeout(10);
	Serial.attachInterrupt(Lua_Stop);

	int error=0;
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	
	lua_register(L, "millis", Lua_millis);
	lua_register(L, "delay", Lua_delay);
	lua_register(L, "pinMode", Lua_pinMode);
	lua_register(L, "togglePin", Lua_togglePin);
	lua_register(L, "digitalWrite", Lua_digitalWrite);
	lua_register(L, "digitalRead", Lua_digitalRead);
	lua_register(L, "analogWrite", Lua_analogWrite);
	lua_register(L, "analogRead", Lua_analogRead);
	
	luaL_dostring(L,"print(\"> Version:\",_VERSION)");
	while (1)
	{
		if(Serial.available())
		{
			String LuaCode = Serial.readString();
			char *LuaCode_buffer = new char[LuaCode.length()+10];
			
//			tft.clear();
//			tft.println("\r\nCode:\r\n");
			tft.setTextColor(LCD_COLOR_MAGENTA);
			tft << "\r\n" << LuaCode;
			tft.setTextColor(LCD_COLOR_WHITE);
//			tft.printf("\r\n(Size: %d )\r\n",LuaCode.length());
			
			Serial.println("\r\nCode:\r\n");
			Serial.println(LuaCode);
			Serial.printf("\r\n(Size: %d )\r\n",LuaCode.length());		
			LuaCode.toCharArray(LuaCode_buffer,LuaCode.length()+10);
			
			Serial.print("\r\n> ");
			tft.print("\r\n> ");
			LuaRunning = 1;
			error = luaL_loadbuffer(L, LuaCode_buffer, strlen(LuaCode_buffer), "line") || lua_pcall(L, 0, 0, 0);
			delete []LuaCode_buffer;
			LuaRunning = 0;
		}	
		if (error)
		{
			Serial.println(lua_tostring(L, -1));
			tft.setTextColor(LCD_COLOR_RED);
			tft << lua_tostring(L, -1);
			tft.setTextColor(LCD_COLOR_WHITE);
			error = 0;
			LuaRunning = 0;
			//lua_pop(L, 1);
		}
	}
	//lua_close(L); 
}
