#include "3140_concur.h"
#include "utils.h"

void p1(void) {
  for (int i = 0; i < 6; i ++) {
    delay();
    LEDRed_Toggle();
  }
}

void p2(void) {
  for (int i = 0; i < 3; i ++) {
    delay();
    LEDGreen_Toggle();
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