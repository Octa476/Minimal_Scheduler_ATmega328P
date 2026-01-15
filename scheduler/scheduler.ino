#include <Arduino.h>
#include "scheduler.h"

// For the start of this project I will build four different task and I will specify using some const array which tasks should be run by the system.
// The tasks will all be infinite loops and will do a basic thing like making a led go on off, dim it, measure some temperature and so on.
// This version of the scheduler will make every task run for a slice of cpu time and will switch between them in a round-robin fashion.
// This will be the first demo on this so called project.

// 4 available tasks with four different disjoint memory zones(sram sections).
#include "task1/task1.h"
#include "task2/task2.h"
#include "task3/task3.h"
#include "task4/task4.h"

#define STACK_UP_LIMIT 0x08FF
#define STACK_BUFF 0x100

// Specify which tasks you want to be run on the microcontroller.
const void (*tasks_function[]) () = {task1, task2};
static char curr_thread_indx = 0;
static Thread_Control_Block threads[4];


void schedule_threads(short old_stack) {
  int num_tasks = sizeof(tasks_function) / sizeof(void (*) ());
  // It is the first time this thread will run.
  if (threads[curr_thread_indx].thread_state == 2) {
    threads[curr_thread_indx].thread_state = 1;
  } else if (threads[curr_thread_indx].thread_state == 1) {
    // This thread was running.
    threads[curr_thread_indx].thread_state = 0; // ready, but not running
    threads[curr_thread_indx].stack_pointer = old_stack; // save the old stack pointer

    // Go to the next thread.
    curr_thread_indx = (curr_thread_indx + 1) % num_tasks;

    // The first time this thread will run.
    if (threads[curr_thread_indx].thread_state == 2) {
      threads[curr_thread_indx].thread_state = 1;
    }
  }
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = 40535; // Timer Preloading
  // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void Threads_init() {
  int num_tasks = sizeof(tasks_function);
  for (int i = 0; i < num_tasks; i++) {
    threads[i].thread_id = i;
    threads[i].thread_state = 2;
    threads[i].stack_pointer = STACK_UP_LIMIT - (i + 2) * STACK_BUFF;
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

  // Set Timer1 to give an interupt every 100ms for the time being.
  TCCR1A = 0;           // Init Timer1
  TCCR1B = 0;           // Init Timer1
  TCCR1B |= B00000011;  // Prescalar = 64
  TCNT1 = 40535;        // Timer Preloading
  TIMSK1 |= B00000001;  // Enable Timer Overflow Interrupt

  // Some way to debug the timer interrupt using the builtin led.
  pinMode(LED_BUILTIN, OUTPUT);

  // Init the threads control blocks.
  Threads_init();

  // We need a loop for the program to not stop running.
  while(1) {}
}