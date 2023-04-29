#include "Board_LED.h"
#include <stdint.h>
#include "Board_Buttons.h"
#include <stm32f429xx.h>
#include "core_cm4.h"

volatile uint32_t  state = 0;
volatile uint32_t  count = 0;      //debouncing
volatile uint32_t  pressed = 0;    //state variable
volatile uint32_t t = 0;           //control variable
volatile uint32_t delay = 0;       //keeping track of delay

void toggle(){
		if(state)
			LED_On(0U);
		else
			LED_Off(0U);
		state = ~state;
	}


void SysTick_Handler(void){
	t++; delay++;
	if(Buttons_GetState()){
		count++; 
		if(count > 30){
			pressed++;
			count = 0;
		}
	}
	else{
		count = 0;
	}
	if(t==200){
		t = 0; pressed = 0;
	}
		
	if(delay == 100){   //generate 0.5ms delay
		delay = 0;
	}
}

int main(){
	//Setup code 
	LED_Initialize();
	Buttons_Initialize();
	SysTick_Config(SystemCoreClock * 5/(1000));  //timer configured for 5ms delay
	uint32_t test = 0;                           //used to exit the blink loop upon another push event
	
	
	while(1){
		if(Buttons_GetState()){
			toggle();
			delay = 0;
			while(delay<50);
		}
			
		
		
		if(pressed >=2 ){
			while(Buttons_GetState() ==0){
				LED_On(0U);
				delay = 0;
				while(delay<90)    //delay 0.9 * 0.5s
				if(Buttons_GetState()){ test = 1; break;}
				
				LED_Off(0U);
				delay = 0;
				while(delay<90)
				if(Buttons_GetState()){ test = 1; break;}
				
				if(test){test = 0; break;}
				
			}
		}
		//LED_Off(0U);
	}
		return 0;
}