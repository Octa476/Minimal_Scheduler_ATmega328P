
#define _SPH 0x3E
#define _SPL 0x3D

; This is used when a task is started to move the execution from the scheduler instructions 
; to the instructions of the task and to move the stack pointer to the frame of the new task.
; void start_task(short stack_pointer, short address) - C function
; short stack_pointer -> low byte r24; high byte r25
; short address -> low byte r22; high byte r23
.global start_task

start_task:
    ; change the SP
    out _SPL, r24
	out _SPH, r25
    ; push the address on the new stack
    push r22
    push r23
    ; ret will now take the address on the new stack and move PC to the start of the new task
    ; the stack will be clean and the new execution will begin
    ret
