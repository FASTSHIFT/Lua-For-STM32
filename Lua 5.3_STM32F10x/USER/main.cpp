#include "FileGroup.h"

uint8_t LuaRunning=0;

__asm void GenerateSystemReset(void) 
{ 
 MOV R0, #1
 MSR FAULTMASK, R0
 LDR R0, =0xE000ED0C
 LDR R1, =0x05FA0004  
 STR R1, [R0]        
  
deadloop 
    B deadloop        
}

void Lua_Stop()
{
	if(LuaRunning)
	{
		if(Serial.read()=='S')
		{
			GenerateSystemReset();
		}
		else Serial.flush();
	}
}

int main(void)
{
	Delay_Init();
	ADCx_Init(ADC1);
	
	Serial.begin(115200);
	Serial.println("\r\nInitializing...");
	Serial.setTimeout(10);
	Serial.attachInterrupt(Lua_Stop);

	int error=0;
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	
	lua_register(L, "millis", Lua_millis);
	lua_register(L, "delay", Lua_delay);
	lua_register(L, "pinMode", Lua_pinMode);
	lua_register(L, "digitalWrite", Lua_digitalWrite);
	lua_register(L, "digitalRead", Lua_digitalRead);
	lua_register(L, "analogWrite", Lua_analogWrite);
	lua_register(L, "analogRead", Lua_analogRead);
	
	luaL_dostring(L,"print(\"> Version:\",_VERSION)");
	while (1)
	{
		if(Serial.available())
		{
			String rx=Serial.readString();
			char *rx_buffer = new char[rx.length()+10];
			
			Serial.println("\r\nCode:\r\n");
			Serial.println(rx);
			Serial.printf("\r\n(Size: %d )\r\n",rx.length());		
			rx.toCharArray(rx_buffer,rx.length()+10);
			
			Serial.print("\r\n> ");
			LuaRunning = 1;
			error = luaL_loadbuffer(L, rx_buffer, strlen(rx_buffer), "line") || lua_pcall(L, 0, 0, 0);
			delete []rx_buffer;
			LuaRunning = 0;
		}	
		if (error)
		{
			Serial.println(lua_tostring(L, -1));
			error = 0;
			LuaRunning = 0;
			//lua_pop(L, 1);
		}
	}
	//lua_close(L); 
}
