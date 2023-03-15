#include "3140_concur.h"
#include "utils.h"

void p1 (void)
{
	int i;
	for (i=0; i < 6; i++) {
		delay ();
    	LEDRed_Toggle();
	}
}

void p2 (void)
{
	int i;
	for (i=0; i < 13; i++) {
		delay ();
    	LEDGreen_Toggle();
	}
}


int main (void){
  LED_Initialize();
 

 if (process_create (p1,32) < 0) {
 	return -1;
 }

 if (process_create (p2,32) < 0) {
 	return -1;
 }
 	
 process_start ();

 /*
After process_start p1 and p2 should be runing concurrently. 
Both LEDs should blink. The process witht the red LED should 
finish first, and after that the green LED should blink a few 
more times, but twice as fast. Why?
*/

 
 while (1) ;

 return 0;
}
