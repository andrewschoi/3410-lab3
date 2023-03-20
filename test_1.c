#include "3140_concur.h"
#include "utils.h"

void p1(void) {
  int i;
  for (i=0; i < 6; i++) {
    delay();
    LEDRed_Toggle();
  }
}

int main(void) {
  LED_Initialize();

  if(process_create(p1, 32) < 0) {
    return -1;
  }

  process_start();

  while(1) {
    return 0;
  }
  
}