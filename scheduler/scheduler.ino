#include <Arduino.h>
#include "scheduler.h"

// For the start of this project I will build two different task and I will specify using some const array which tasks should be run by the system.
// The tasks will all be infinite loops and will do a basic thing like making a led go on off, dim it, measure some temperature and so on.
// This version of the scheduler will make every task run for a slice of cpu time and will switch between them in a round-robin fashion.
// This will be the first demo on this so called project.

// 4 available tasks with four different disjoint memory zones(sram sections).
#include "task1.h"
#include "task2.h"

#define STACK_UP_LIMIT 0x08FF // The bottom of the stack.
#define STACK_BUFF 0x100 // The memory allocated to every thread.

// Specify which tasks you want to be run on the microcontroller.
const void (*tasks_function[]) () = {task1, task2};
static char curr_thread_indx = 0;
static Thread_Control_Block threads[4];
int time_slice = 100; // 100ms for every time slice.

// Initialize the thread control block with the state and the bottom stack pointer.
void Threads_init() {
  int num_tasks = sizeof(tasks_function) / sizeof(void (*) ());
  for (int i = 0; i < num_tasks; i++) {
    threads[i].thread_id = i;
    threads[i].thread_state = 2;
    threads[i].stack_pointer = STACK_UP_LIMIT - (i + 2) * STACK_BUFF;
  }
}

void schedule_threads() {
  int num_tasks = sizeof(tasks_function) / sizeof(void (*) ());

  // It is the first time a thread will run on the system.
  if (threads[curr_thread_indx].thread_state == 2) {
    threads[curr_thread_indx].thread_state = 1;
    // There is some asymmetry with this approach, but it works.
    start_first_task((short)threads[curr_thread_indx].stack_pointer, (short)tasks_function[curr_thread_indx]);
  } else if (threads[curr_thread_indx].thread_state == 1) {
    // This thread was running.
    threads[curr_thread_indx].thread_state = 0; // ready, but not running

    int old_thread_indx = curr_thread_indx;

    // Go to the next thread.
    curr_thread_indx = (curr_thread_indx + 1) % num_tasks;

    // The first time this thread will run on the system.
    if (threads[curr_thread_indx].thread_state == 2) {
      threads[curr_thread_indx].thread_state = 1;
      // The stack context will be used when the old_thread will e put into exeution again.
      start_task((short)threads[curr_thread_indx].stack_pointer, (short)tasks_function[curr_thread_indx], &(threads[old_thread_indx].stack_pointer));
    } else {
      // This will be the fashion in which the threads will change their grip over the cpu after the first cycle of context switches.
      // When a thread is interrupted, the execution goes like old_task -> ISR -> scheduler -> resume_task (on its own stack), then resume_task saves the context and changes
      // the stack pointer to the stack pointer of the new thread and then the execution goes like resume_task -> scheduler -> ISR -> new_task, so everything is restored and the
      // execution continue.
      threads[curr_thread_indx].thread_state = 1;
      resume_task(&(threads[old_thread_indx].stack_pointer), threads[curr_thread_indx].stack_pointer);
    }
  }
}

// The interrupt service routine that wakeup the scheduler.
ISR(TIMER1_OVF_vect)
{
  // This preloading is computed based on the frequency of the external 16MHz crystal and the
  // preloading setting of the timer.
  TCNT1 = 0xFFFF - time_slice * 16 * 1000 / 64; // Timer Preloading
  schedule_threads();

  // For debug purposes.
  if (curr_thread_indx == 0)
    digitalWrite(LED_BUILTIN, LOW);
  else
    digitalWrite(LED_BUILTIN, HIGH);
}

// Set the scheduler to wakeup after time_slice ms.
void set_scheduler_wakeup() {
  // Set Timer1 to give an interupt every 100ms for the time being.
  TCCR1A = 0;  // Init Timer1
  TCCR1B = 0; // Init Timer1
  TCCR1B |= B00000101; // Prescalar = 64
  TCNT1 = 0xFFFF - time_slice * 16 * 1000 / 64; // Timer Preloading
  // Serial.println(TCNT1);
  TIMSK1 |= B00000001; // Enable Timer Overflow Interrupt
}

int main() {
  // Start the Serial.
  init();
  interrupts();
  Serial.begin(9600);

  int num_tasks = sizeof(tasks_function);
  // Serial.println((uint16_t)(&num_tasks), HEX);
  // Serial.println((uint16_t)(&curr_thread_indx), HEX);
  // Serial.println((uint16_t)(&threads), HEX);
  // Serial.println(sizeof(void (*) ()));

  // Init the threads control blocks.
  Threads_init();
  
  // Make the scheduler wakeup every 100ms.
  set_scheduler_wakeup();

  // Some way to debug the timer interrupt using the builtin led.
  pinMode(LED_BUILTIN, OUTPUT);

  // We need a loop for the program to not stop running.
  while(1) {}
}