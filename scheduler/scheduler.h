#pragma once
// This file will contain the type and function declaration used by the scheduler.

// This struct manages the information needed by a thread.
typedef struct {
  // The other parts of the context(register values, instruction pointer, SREG register) will be stored on the stack of the interrupt handler service.

  // The stack pointer SP is implemented as two 8-bit registers in the I/O space, so a short is enough to store it.
  short stack_pointer;

  // A simple thread id to differentiate between threads.
  short thread_id;

  // 2 for not set yet, 1 for running, 0 for ready, when a thread waits for input for example, it won't be immediately suspended by the scheduler, it will block until the input is received or
  // its time slice has expired and another thread will aquire the resorces. 
  char thread_state;
} Thread_Control_Block;

