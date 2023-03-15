#include "3140_concur.h"
#include <stdlib.h>
#include <MKL46Z4.h>

struct process_state {
  unsigned int * sp;
  unsigned int * original_sp;
  process_t * next_process_ptr;
  int size;
  int is_blocked;
}

int process_create(void *f(void), n) {
}

void process_start(void) {

}

unsigned int * process_select(unsigned int * cursp) {

}