// Runs on TM4C123
// Alberto Martinez

/* Option A1, connect LEDs to PB5-PB0, switches to PA5-3, walk LED PF321
   Option A2, connect LEDs to PB5-PB0, switches to PA4-2, walk LED PF321 // This is my test case // South, West, Walk...
   Option A6, connect LEDs to PB5-PB0, switches to PE3-1, walk LED PF321
   Option A5, connect LEDs to PB5-PB0, switches to PE2-0, walk LED PF321
   Option B4, connect LEDs to PE5-PE0, switches to PC7-5, walk LED PF321
   Option B3, connect LEDs to PE5-PE0, switches to PC6-4, walk LED PF321
   Option B1, connect LEDs to PE5-PE0, switches to PA5-3, walk LED PF321
   Option B2, connect LEDs to PE5-PE0, switches to PA4-2, walk LED PF321
  */
// east/west red light connected to bit 5
// east/west yellow light connected to bit 4
// east/west green light connected to bit 3
// north/south red light connected to bit 2
// north/south yellow light connected to bit 1
// north/south green light connected to bit 0
// pedestrian detector connected to most significant bit (1=pedestrian present)
// north/south car detector connected to middle bit (1=car present)
// east/west car detector connected to least significant bit (1=car present)
// "walk" light connected to PF3-1 (built-in white LED)
// "don't walk" light connected to PF3-1 (built-in red LED)

// Option A2, connect LEDs to PB5-PB0, switches to PA4-2, walk LED PF321
// South West Walk 
// Initial State is South green, West red, walk red
// Total # of different outcomes: 10

#include <stdint.h>
#include "SysTick.h"
#include "Lab5grader.h"
#include "../inc/tm4c123gh6pm.h"
char EID1[] = "aam6728"; //  ;replace abc123 with your EID

void DisableInterrupts(void);
void EnableInterrupts(void);

struct state{
	uint32_t outputB;
	uint32_t outputF;
	uint32_t wait;
	uint32_t next[8];
};
typedef const struct state state_t;
#define SG 0
#define SY 1
#define SR 2
#define WG 3
#define WY 4
#define WR 5
#define WalkW 6
#define Red1 7
#define Off1 8
#define Red2 9
#define Off2 10
#define Red3 11
#define Off3 12
#define WalkR 13

state_t FSM[14]={
// {PORTB, PORTF {ALL EIGHT COMBINATIONS}
// add a third row for delay
	
	{0x21, 0x02, 100, { SG, SY, SG, SY, SY, SY, SY, SY}}, // SG
	{0x22, 0x02, 50,  { SR, SR, SR, SR, SR, SR, SR, SR}}, // SY
	{0x24, 0x02, 100, { SR, WG, SG, WG, WalkW, WG, WalkW, WG}}, // SR 
	{0x0C, 0x02, 100, { WG, WG, WY, WY, WY, WY, WY, WY}}, // WG
	{0x14, 0x02, 50,  { WR, WR, WR, WR, WR, WR, WR, WR}}, // WY
	{0x24, 0x02, 100, { WR, WG, SG, SG, WalkW, WalkW, WalkW, WalkW}}, // WR
	{0x24, 0x0E, 100, { WalkW, Red1, Red1, Red1, Red1, Red1, Red1, Red1}}, // WalkW // left here
	{0x24, 0x02, 50,  { Off1, Off1, Off1, Off1, Off1, Off1, Off1, Off1}}, // RED1
	{0x24, 0x00, 50,  { Red2, Red2, Red2, Red2, Red2, Red2, Red2, Red2}}, //OFF1
	{0x24, 0x02, 50,  { Off2, Off2, Off2, Off2, Off2, Off2, Off2, Off2}}, //RED2
	{0x24, 0x00, 50,  { Red3, Red3, Red3, Red3, Red3, Red3, Red3, Red3}}, //OFF2
	{0x24, 0x02, 50,  { Off3, Off3, Off3, Off3, Off3, Off3, Off3, Off3}}, //RED3
	{0x24, 0x00, 50,  { WalkR, WalkR, WalkR, WalkR, WalkR, WalkR, WalkR, WalkR}}, //OFF3
	{0x24, 0x02, 100, { WalkR, WG, SG, SG, WalkW, WG, SG, SG}}}; // WalkR
	

int main(void){ 
  DisableInterrupts();
  TExaS_Init(GRADER);
  SysTick_Init();   // Initialize SysTick for software waits
  // initialize system
	
		volatile uint32_t delay=100;
	
		SYSCTL_RCGCGPIO_R |= 0x23; // initializing ports a b and f
	
		delay; // waiting for clock to turn on
	
	
	  // making my outputs 1s and inputs 0s
		GPIO_PORTA_DIR_R  &=0x1C ; // port a 4-2 mak them 0s because theyre inputs
		GPIO_PORTB_DIR_R  |=0x3F ; // port b 5-0 
		GPIO_PORTF_DIR_R  |=0x0E ; // port f 1-3
	
	  // enabling all my inputs and outputs
		GPIO_PORTA_DEN_R  |=0x1C ; // port a 4-2
		GPIO_PORTB_DEN_R  |=0x3F ; // port b 5-0
		GPIO_PORTF_DEN_R  |=0x0E ; // port f 1-3 
		
		uint32_t S ; //  S keeps track of current state
		int32_t Input ; // input switch 
		
		S = SG;  
		
	
  EnableInterrupts(); 
 
  while(1){
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R&0x3F); GPIO_PORTB_DATA_R = FSM[S].outputB;  // set lights for west and south
		GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R&0x0E); GPIO_PORTF_DATA_R = FSM[S].outputF; // set walk lights
    SysTick_Wait10ms(FSM[S].wait);
    Input = (GPIO_PORTA_DATA_R&0x1C)>>2;   // read sensors and shift to PA0
    S = FSM[S].next[Input];
	}

}
