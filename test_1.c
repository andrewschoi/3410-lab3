#include "3140_concur.h"
#include "utils.h"

//The red LED turns on and off three times with consistent timing
void p1(void) {
  int i;
  for (i=0; i < 6; i++) {
    delay();
    LEDRed_Toggle();
  }
}

int main(void) {
  LED_Initialize();

//Only one process is ran
  if(process_create(p1, 32) < 0) {
    return -1;
  }
//Expectation: P1 will run as normal with consistent timing
  process_start();

  while(1) {
    return 0;
  }
  
}