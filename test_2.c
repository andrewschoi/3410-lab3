#include "3140_concur.h"
#include "utils.h"

// Process 1 blinks the red LED three times
void p1(void) {
  for (int i = 0; i < 6; i ++) {
    delay();
    LEDRed_Toggle();
  }
}

// Process 2 blinks the green LED forever
void p2(void) {
	delay();
	LEDGreen_Toggle();
	p2();
}

int main(void) {
  LED_Initialize();

  if (process_create(p1, 32) < 0) {
    return -1;
  }

  if (process_create(p2, 32) < 0) {
    return -1;
  }

  // Expectation: Process 1 and 2 will occurs at the same time. After process 1 finishes, process 2 will continue forever
  process_start ();


  while(1) {
  }
  return 0;
}
