#include "3140_concur.h"
#include "utils.h"

// process 1 is a longer process
void p1(void) {
  for (int i = 0; i < 6; i ++) {
    delay();
    LEDRed_Toggle();
  }
}

// process 2 is a shorter process 
void p2(void) {
  for (int i = 0; i < 3; i ++) {
    delay();
    LEDGreen_Toggle();
    delay();
    LEDRed_Toggle();
  }
}

int main(void) {
  LED_Initialize();

  if (process_create(p1, 32) < 0) {
    return -1
  }

  if (process_create(p1, 32) < 0) {
    return -1
  }


  while(1) {
    return 0
  }
}