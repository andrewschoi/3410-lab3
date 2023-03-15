#include <MKL46Z4.h>
#include "utils.h"

/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void LED_Initialize(void) {

  SIM->SCGC5    |= (1 <<  13) | (1 <<  12);  /* Enable Clock to Port D & E */
  PORTE->PCR[29] = (1 <<  8) ;               /* Pin PTE29 is GPIO */
  PORTD->PCR[5] =  (1 <<  8);                /* Pin PTD5  is GPIO */
  
  PTE->PDOR = (1 << 29 );          /* switch Red LED off    */
  PTE->PDDR = (1 << 29 );          /* enable PTE29 as Output */

  PTD->PDOR = 1 << 5;            /* switch Greed LED off  */
  PTD->PDDR = 1 << 5;            /* enable PTD5 as Output */
}

/*----------------------------------------------------------------------------
  Function that toggles the red LED
 *----------------------------------------------------------------------------*/

void LEDRed_Toggle (void) {
	PTE->PTOR = 1 << 29; 	   /* Red LED Toggle */
}


/*----------------------------------------------------------------------------
  Function that toggles the green LED
 *----------------------------------------------------------------------------*/
void LEDGreen_Toggle (void) {
	PTD->PTOR = 1 << 5; 	   /* Green LED Toggle */
}

/*----------------------------------------------------------------------------
  Function that turns on Red LED 
 *----------------------------------------------------------------------------*/
void LEDRed_On (void) {
        PTE->PCOR   = 1 << 29;   /* Red LED On*/
 }

/*----------------------------------------------------------------------------
  Function that turns on Green LED 
 *----------------------------------------------------------------------------*/
void LEDGreen_On (void) {
        PTD->PCOR   = 1 << 5;   /* Green LED On*/
}

/*----------------------------------------------------------------------------
  Function that turns all LEDs off
 *----------------------------------------------------------------------------*/
void LED_Off (void) {	

	// Save and disable interrupts (for atomic LED change)
	uint32_t m;
	m = __get_PRIMASK();
	__disable_irq();
	
        PTD->PSOR   = 1 << 5;   /* Green LED Off*/
        PTE->PSOR   = 1 << 29;  /* Red LED Off*/
 	
	// Restore interrupts
	__set_PRIMASK(m);
}

void delay(void){
	int j;
	for(j=0; j<1000000; j++);
}
