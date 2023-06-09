#include "3140_concur.h"
#include <stdlib.h>
#include <MKL46Z4.h>

struct process_state {
  unsigned int * sp;
  unsigned int * original_sp;
  process_t * next_process_ptr;
  int size;
  int is_blocked;
  
};

struct process_state* current_process = NULL;
struct process_state* process_queue = NULL;

process_t* dequeue(void) {
  if (process_queue == NULL) return NULL;

  process_t* head_process = process_queue;
  process_queue = process_queue -> next_process_ptr;
  head_process->next_process_ptr = NULL;
  return head_process;
}

void enqueue(process_t* new_process) {
  if (new_process == NULL) return;

  if (process_queue == NULL) {
    process_queue = new_process;
    process_queue -> next_process_ptr = NULL;
    return;
  }

  process_t* tmp = process_queue;
  while (tmp -> next_process_ptr != NULL) {
    tmp = tmp -> next_process_ptr;
  }

  tmp -> next_process_ptr = new_process;
  new_process -> next_process_ptr = NULL;
}

int process_create (void (*f)(void), int n){
  NVIC_DisableIRQ(PIT_IRQn);
  unsigned int * stack_ptr = process_stack_init(f, n);
  NVIC_EnableIRQ(PIT_IRQn);

  if (stack_ptr == NULL) return -1;
  process_t * new_process = malloc(sizeof(process_t));
  if (new_process == NULL) return -1;

  new_process -> next_process_ptr = NULL;
  new_process -> sp = stack_ptr;
  new_process -> original_sp = stack_ptr;
  new_process -> size = n;
  new_process->is_blocked = 0;

  enqueue(new_process);
  return 0;
}

// set up PIT timer and loads a time
void timer_setup(void) {
  SIM -> SCGC6 = SIM_SCGC6_PIT_MASK;
  PIT -> MCR = 0x00;
  PIT -> CHANNEL[0].LDVAL = 0x9FE80;
  PIT -> CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK;
}

void process_start(void) {
  NVIC_EnableIRQ( PIT_IRQn);
  timer_setup();

  if (process_queue == NULL) return;
  process_begin();

}

unsigned int * process_select(unsigned int * cursp) {
  if (cursp  == NULL) {
    if (current_process != NULL) {
      NVIC_DisableIRQ(PIT_IRQn);
      process_stack_free(current_process -> original_sp, current_process -> size);
      NVIC_EnableIRQ(PIT_IRQn);
    }
  } else {
    current_process -> sp = cursp;
    if ( current_process->is_blocked == 0 ) {
    	    // process_queue not empty, place current_process at the end
    		  if ( process_queue != NULL ) enqueue( current_process );

    		  // process_queue is empty, put currently running process as head of queue
    		  else process_queue = current_process;
    		}
  }

  process_t* new_currentprocess = dequeue();
  
  if (new_currentprocess == NULL) return NULL;
  else {
    current_process = new_currentprocess;
    current_process -> next_process_ptr = NULL;
    return current_process -> sp;
  }


}
