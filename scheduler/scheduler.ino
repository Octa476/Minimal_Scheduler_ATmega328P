#include <Arduino.h>
#include "scheduler.h"

// For the start of this project I will build four different task and I will specify using some const array which tasks should be run by the system.
// The tasks will all be infinite loops and will do a basic thing like making a led go on off, dim it, measure some temperature and so on.
// This version of the scheduler will make every task run for a slice of cpu time and will switch between them in a round-robin fashion.
// This will be the first demo on this so called project.

// 4 available tasks with four different disjoint memory zones(sram sections).
#include "task1.h"
#include "task2.h"

#define STACK_UP_LIMIT 0x08FF
#define STACK_BUFF 0x100

// Specify which tasks you want to be run on the microcontroller.
const void (*tasks_function[]) () = {task1, task2};
static char curr_thread_indx = 0;
static Thread_Control_Block threads[4];

extern "C" {
  void resume_task(short old_stack_pointer_addr, short new_stack_pointer);
  void start_task(short stack_pointer, short address, short old_stack_pointer_addr);
  void start_first_task(short stack_pointer, short address);
}
// __attribute__((naked, noreturn))
// void start_first_task(short stack_pointer, short address)
// {
//     asm volatile (
//         "out 0x3D, r24\n\t"    // SPL
//         "out 0x3E, r25\n\t"    // SPH
//         "clr r0\n\t"            // r1 = 0
//         // "ldi r16, 0x80\n\t"     // I = 1
//         // "out 0x3F, r16\n\t"     // SREG
//         "sei \n\t"
//         "mov r30, r22\n\t"      // Z = task address
//         "mov r31, r23\n\t"
//         "ijmp\n\t"
//     );
// }

// __attribute__((naked, noreturn))
// void start_task(short stack_pointer, short address, short old_stack_pointer_addr)
// {
//     asm volatile (
//         "in r0, 0x3F\n\t"
//         "push r0\n\t" // SREG

//         "push r0\n\t"
//         "push r1\n\t"
//         "push r2\n\t"
//         "push r3\n\t"
//         "push r4\n\t"
//         "push r5\n\t"
//         "push r6\n\t"
//         "push r7\n\t"
//         "push r8\n\t"
//         "push r9\n\t"
//         "push r10\n\t"
//         "push r11\n\t"
//         "push r12\n\t"
//         "push r13\n\t"
//         "push r14\n\t"
//         "push r15\n\t"
//         "push r16\n\t"
//         "push r17\n\t"
//         "push r18\n\t"
//         "push r19\n\t"
//         "push r20\n\t"
//         "push r21\n\t"
//         "push r22\n\t"
//         "push r23\n\t"
//         "push r24\n\t"
//         "push r25\n\t"
//         "push r26\n\t"
//         "push r27\n\t"
//         "push r28\n\t"
//         "push r29\n\t"
//         "push r30\n\t"
//         "push r31\n\t"

//         // Save the old stack pointer into the old_stack_pointer_addr.
//         // Prepare the Z register.
//         "mov r30, r20\n\t"
//         "mov r31, r21\n\t"
//         // Store the low byte of the old stack in the address pointed by Z, then increment Z.
//         "in r16, 0x3D\n\t"
//         "ST Z+, r16\n\t"

//         // Store the high byte of the old stack in the address pointed by the Z.
//         "in r16, 0x3E\n\t"
//         "ST Z, r16\n\t"

//         // Change stack.
//         "out 0x3D, r24\n\t"    // SPL
//         "out 0x3E, r25\n\t"    // SPH
//         "clr r0\n\t"            // r1 = 0
//         // "ldi r16, 0x80\n\t"     // I = 1
//         // "out 0x3F, r16\n\t"     // SREG
//         "sei \n\t"
//         "mov r30, r22\n\t"      // Z = task address
//         "mov r31, r23\n\t"
//         "ijmp\n\t"
//     );
// }

// __attribute__((naked))
// void resume_task(short old_stack_pointer_addr, short new_stack_pointer)
// {
//     asm volatile (
//         // Push everything on the stack.
//         "in r0, 0x3F\n\t"
//         "push r0\n\t" // SREG

//         "push r0\n\t"
//         "push r1\n\t"
//         "push r2\n\t"
//         "push r3\n\t"
//         "push r4\n\t"
//         "push r5\n\t"
//         "push r6\n\t"
//         "push r7\n\t"
//         "push r8\n\t"
//         "push r9\n\t"
//         "push r10\n\t"
//         "push r11\n\t"
//         "push r12\n\t"
//         "push r13\n\t"
//         "push r14\n\t"
//         "push r15\n\t"
//         "push r16\n\t"
//         "push r17\n\t"
//         "push r18\n\t"
//         "push r19\n\t"
//         "push r20\n\t"
//         "push r21\n\t"
//         "push r22\n\t"
//         "push r23\n\t"
//         "push r24\n\t"
//         "push r25\n\t"
//         "push r26\n\t"
//         "push r27\n\t"
//         "push r28\n\t"
//         "push r29\n\t"
//         "push r30\n\t"
//         "push r31\n\t"
//         // Save the old stack pointer into the old_stack_pointer_addr.
//         // Prepare the Z register.
//         "mov r30, r24\n\t"
//         "mov r31, r25\n\t"
//         // Store the low byte of the old stack in the address pointed by Z, then increment Z.
//         "in r16, 0x3D\n\t"
//         "ST Z+, r16\n\t"

//         // Store the high byte of the old stack in the address pointed by the Z.
//         "in r16, 0x3E\n\t"
//         "ST Z, r16\n\t"

//         // Now change the stack pointer with the new one and start popping everything.
//         "out 0x3D, r22\n\t"
//         "out 0x3E, r23\n\t"
        
//         // Pop everything in reverse order.
//         "pop r31\n\t"
//         "pop r30\n\t"
//         "pop r29\n\t"
//         "pop r28\n\t"
//         "pop r27\n\t"
//         "pop r26\n\t"
//         "pop r25\n\t"
//         "pop r24\n\t"
//         "pop r23\n\t"
//         "pop r22\n\t"
//         "pop r21\n\t"
//         "pop r20\n\t"
//         "pop r19\n\t"
//         "pop r18\n\t"
//         "pop r17\n\t"
//         "pop r16\n\t"
//         "pop r15\n\t"
//         "pop r14\n\t"
//         "pop r13\n\t"
//         "pop r12\n\t"
//         "pop r11\n\t"
//         "pop r10\n\t"
//         "pop r9\n\t"
//         "pop r8\n\t"
//         "pop r7\n\t"
//         "pop r6\n\t"
//         "pop r5\n\t"
//         "pop r4\n\t"
//         "pop r3\n\t"
//         "pop r2\n\t"
//         "pop r1\n\t"
//         "pop r0\n\t"

//         "pop r0\n\t"
//         "out 0x3F, r0\n\t"

//         // The execution will go back to the scheduler, from the scheduler back to the ISR and then to the task.
//         "ret\n\t"
//     );
// }

void schedule_threads() {
  int num_tasks = sizeof(tasks_function) / sizeof(void (*) ());
  // digitalWrite(LED_BUILTIN, LOW);

  // It is the first time a thread will run.
  if (threads[curr_thread_indx].thread_state == 2) {
    threads[curr_thread_indx].thread_state = 1;
    // Serial.println("first");
    start_first_task((short)threads[curr_thread_indx].stack_pointer, (short)tasks_function[curr_thread_indx]);
  } else if (threads[curr_thread_indx].thread_state == 1) {
    // Serial.println((int)curr_thread_indx);
    // Serial.println((int)curr_thread_indx);
    // This thread was running.
    threads[curr_thread_indx].thread_state = 0; // ready, but not running

    int old_thread_indx = curr_thread_indx;
    // Go to the next thread.
    curr_thread_indx = (curr_thread_indx + 1) % num_tasks;
    // Serial.println((int)curr_thread_indx);

    // The first time this thread will run.
    if (threads[curr_thread_indx].thread_state == 2) {
      threads[curr_thread_indx].thread_state = 1;
      // digitalWrite(2, HIGH);
      // Serial.println("second");
      start_task((short)threads[curr_thread_indx].stack_pointer, (short)tasks_function[curr_thread_indx], &(threads[old_thread_indx].stack_pointer));
    } else {
      // digitalWrite(2, HIGH);
      // Serial.println("fourth");

        // interrupts();
      threads[curr_thread_indx].thread_state = 1;
      // PORTD |= (1 << 5);
      resume_task(&(threads[old_thread_indx].stack_pointer), threads[curr_thread_indx].stack_pointer);
    }

  }
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = 40535; // Timer Preloading
  schedule_threads();
  digitalWrite(4, HIGH);
  if (curr_thread_indx == 0)
    digitalWrite(LED_BUILTIN, LOW);
  else
    digitalWrite(LED_BUILTIN, HIGH);
}

void Threads_init() {
  int num_tasks = sizeof(tasks_function) / sizeof(void (*) ());
  for (int i = 0; i < num_tasks; i++) {
    threads[i].thread_id = i;
    threads[i].thread_state = 2;
    threads[i].stack_pointer = STACK_UP_LIMIT - (i + 2) * STACK_BUFF;
    Serial.println(threads[i].stack_pointer, HEX);
  }
}

int main() {
  // Start the Serial.
  init();
  interrupts();
  Serial.begin(9600);

  int num_tasks = sizeof(tasks_function);
  Serial.println((uint16_t)(&num_tasks), HEX);
  Serial.println((uint16_t)(&curr_thread_indx), HEX);
  Serial.println((uint16_t)(&threads), HEX);
  Serial.println(sizeof(void (*) ()));

  // Set Timer1 to give an interupt every 100ms for the time being.
  TCCR1A = 0;           // Init Timer1
  TCCR1B = 0;           // Init Timer1
  TCCR1B |= B00000101;  // Prescalar = 64
  TCNT1 = 40535;        // Timer Preloading
  TIMSK1 |= B00000001;  // Enable Timer Overflow Interrupt

  // Some way to debug the timer interrupt using the builtin led.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, OUTPUT);
  // DDRB |= (1 << 5);
  // digitalWrite(LED_BUILTIN, LOW);

  // Init the threads control blocks.
  Threads_init();

  // We need a loop for the program to not stop running.
  while(1) {}
}