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

__attribute__((naked, noreturn))
void start_task(short stack_pointer, short address)
{
    asm volatile (
        "out 0x3D, r24\n\t"    // SPL
        "out 0x3E, r25\n\t"    // SPH
        // "clr r1\n\t"            // r1 = 0
        // "ldi r16, 0x80\n\t"     // I = 1
        // "out 0x3F, r16\n\t"     // SREG
        "sei \n\t"
        "mov r30, r22\n\t"      // Z = task address
        "mov r31, r23\n\t"
        "ijmp\n\t"
    );
}


void schedule_threads() {
  int num_tasks = sizeof(tasks_function) / sizeof(void (*) ());
  

  // It is the first time a thread will run.
  if (threads[curr_thread_indx].thread_state == 2) {
    threads[curr_thread_indx].thread_state = 1;
    Serial.println("first");
    start_task((short)threads[curr_thread_indx].stack_pointer, (short)tasks_function[curr_thread_indx]);
  } else if (threads[curr_thread_indx].thread_state == 1) {
    Serial.println("third");
    // Serial.println((int)curr_thread_indx);
    // This thread was running.
    threads[curr_thread_indx].thread_state = 0; // ready, but not running
    threads[curr_thread_indx].stack_pointer = 0x8FF; // save the old stack pointer

    // Go to the next thread.
    curr_thread_indx = (curr_thread_indx + 1) % num_tasks;

    // The first time this thread will run.
    if (threads[curr_thread_indx].thread_state == 2) {
      threads[curr_thread_indx].thread_state = 1;
      Serial.println("second");
      // start_task((short)threads[curr_thread_indx].stack_pointer, (short)tasks_function[curr_thread_indx]);
    } else {
      threads[curr_thread_indx].thread_state = 1;
      
    }
  }
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = 40535; // Timer Preloading
  // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  schedule_threads();
  // task1();
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
  TCCR1B |= B00000011;  // Prescalar = 64
  TCNT1 = 40535;        // Timer Preloading
  TIMSK1 |= B00000001;  // Enable Timer Overflow Interrupt

  // Some way to debug the timer interrupt using the builtin led.
  pinMode(LED_BUILTIN, OUTPUT);
  DDRB |= (1 << 5);

  // Init the threads control blocks.
  Threads_init();

  // We need a loop for the program to not stop running.
  while(1) {}
}