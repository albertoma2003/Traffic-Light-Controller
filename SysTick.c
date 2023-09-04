// SysTick.c
// Runs on TM4C123
// Put your names here
// Alberto Martinez, Rocio Hernandez
// Last Modified: 8/21/2022
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

#define NVIC_ST_CTRL_R (*((volatile uint32_t *)0xE000E010)) 
#define NVIC_ST_RELOAD_R (*((volatile uint32_t *)0xE000E014)) 
#define NVIC_ST_CURRENT_R (*((volatile uint32_t *)0xE000E018))

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
	  
		NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup 
		NVIC_ST_RELOAD_R = 0x00FFFFFF; // 2) maximum reload value 
		NVIC_ST_CURRENT_R = 0; // 3) any write to CURRENT clears it 
		NVIC_ST_CTRL_R = 0x00000005; // 4) enable SysTick with core clock
	
}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(uint32_t delay){

		 NVIC_ST_RELOAD_R = delay-1;  // number of counts
		 NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
		 while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for COUNT flag
		 }
}
// 10000us equals 10ms
void SysTick_Wait10ms(uint32_t delay){
	
		for(uint32_t i=0; i<delay; i++){
  SysTick_Wait(800000); // wait 10ms
			
		}
}

