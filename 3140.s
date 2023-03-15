.section .data
//global variable in assembly
OrigStackPointer:
.word 0x00
.syntax unified


//variables for holding the low-registers while
//push-popping the high ones onto the stack
Reg4val:
.word 0x00
Reg5val:
.word 0x00
Reg6val:
.word 0x00
Reg7val:
.word 0x00

		
.section .text, "xa"
.syntax unified
//export assembly functions
.global process_terminated
.global process_begin
.global process_blocked
.global PIT_IRQHandler
.global SVC_Handler

//import C functions
.weak process_select
.type process_select, %function

//		PRESERVE8
				
.equ TFLG, 0x4003710C // TFLG address
.equ CTRL, 0x40037108 // Ctrl address
.equ SHCSR, 0xE000ED20

/*
General note: you need to declare actual function labels as functions.
Otherwise, the linker will not set the thumb bit correctly when branching,
and the processor will HardFault with INV_STATE.

Lab0 and Lab1 for ECE 3140 work fine because nothing is actually called
as a function. Main is called properly.
*/
.type  SVC_Handler, %function
SVC_Handler:
	LDR  R1, [SP,#24] 	    // Read PC from SVC exception
	MOVS R2,#2
	SUBS R1, R2
	LDRB R0, [R1] 		    // Get #N from SVC instruction (PC is SVC addr + 2)
	ADR  R1, SVC_Table
	LSLS R0, #2 		    //Multiply by 4 to (left shift by 2) to get offset from table
	LDR  R2, [R1,R0] 	    // Get Branch Address
	MOV  PC, R2				// Branch to Appropriate SVC Routine


SVC_Table:
.word SVC0_begin
.word SVC1_terminate
.word PIT_IRQHandler 		// Use system tick as SVC2 handler

.type  SVC0_begin, %function
SVC0_begin:
				PUSH {LR}    // First put return code on stack
				PUSH {R4-R7} // These are original values of R4-R7
				MOV R4, R8
				MOV R5, R9
				MOV R6, R10
				MOV R7, R11
				PUSH {R4-R7} // These now hold the values of R8-R11


				//******* Store Original Stack Pointer ********
				LDR R1, =OrigStackPointer
				MOV R2, SP
				STR R2, [R1]
				//********************************************

.type  SVC1_terminate, %function
SVC1_terminate:
				MOVS R0, #0
				B do_process_select


.type  process_terminated, %function
process_terminated:
				CPSIE i // Enable global interrupts, just in case
				SVC #1 // SVC1 = process terminated
				// This SVC shouldn't ever return, as it would mean the process was scheduled again

.type  process_begin, %function
process_begin:
				CPSIE i // Enable global interrupts (for SVC)
				SVC #0 // Syscall into scheduler
				BX LR

.type  process_blocked, %function
process_blocked:
				CPSIE i // Enable global interrupts, just in case
				SVC #2 // SVC2 = process blocked
				BX LR

.type  PIT_IRQHandler, %function
PIT_IRQHandler: // Timer Interrupt
				CPSID i 			// Disable all interrupts

				PUSH {LR}     // First put return code on stack
				PUSH {R4-R7}  // These are the original values of R4-R7
				MOV R4, R8
				MOV R5, R9
				MOV R6, R10
				MOV R7, R11
				PUSH {R4-R7} // These now hold the values of R8-R11

			  	//----store scheduling timer state----
			  	LDR R1, =CTRL
			  	LDR R0, [R1]
			  	PUSH {R0}

			  	//---clear the interrupt flag----
			  	LDR  R4, =TFLG
			  	MOVS R1, #1
			  	STR  R1, [R4]
			  	//-------------------------------
				
			  	//move sp to r0 to prepare for process_select
			   	MOV R0, SP
				
do_process_select:
				//******* Load Original Stack Pointer ********
				// We want the process select function to run on the "main" stack
				// This helps reduce funkiness when a process stack is small
				// and process_select overwrites other memory
				LDR R1, =OrigStackPointer
				LDR R2, [R1]
				MOV SP, R2
				//********************************************
				BL process_select	//Process_select returns 0 if there are no processes left
				CMP R0, #0
				BNE resume_process	//take branch if there are more processes
				
				// Disable scheduling timer before returning to initial caller
				LDR R1, =CTRL
				MOVS R0, #0
				STR R0, [R1]

				POP {R4-R7} // Grab the high registers and restore them

				MOV R8, R4
				MOV R9, R5
				MOV R10, R6
				MOV R11, R7

				POP {R4-R7,PC} // These are now the original values of R4-R7 and the return code

				
resume_process:
				MOV SP, R0    //switch stacks
				//---- restore scheduling timer state
				POP {R0}
			        LDR R1, =CTRL
			        STR R0, [R1]
				
				CPSIE I // Enable global interrupts before returning from handler

				POP {R4-R7} // Grab the high registers and restore them

				MOV R8, R4
				MOV R9, R5
				MOV R10, R6
				MOV R11, R7

				POP {R4-R7,PC} // These are now the original values of R4-R7 and the return code

