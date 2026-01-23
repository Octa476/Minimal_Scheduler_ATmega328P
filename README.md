# Minimal_Scheduler_ATmega328P
I will try to build a minimal scheduler on the ATmega328P microcontroller using the arduino IDE and some AVR assembly.

# My idea
The idea is that I want to make multiprogramming in this environment possible by using timer interrupts and a simple Round Robin scheduling scheme.

## Microcontroller usefull specifications
Some of the specifications of the microcontroller used by de Arduino - ATmega328P are:
- 32K bytes of in-system self-programmable flash program memory
- 1Kbytes EEPROM
- 2Kbytes internal SRAM
- 131 powerful instructions – most single clock cycle execution
- 32 x 8 general purpose working registers
- Two 8-bit Timer/Counters with separate prescaler and compare mode
- One 16-bit Timer/Counter with separate prescaler, compare mode, and capture
mode
- Real time counter with separate oscillator
- External and internal interrupt sources

## The Scheduler
The scheduling is done by a C/C++ function that changes the current context of the execution to another thread by using some assembly procedures.
For the scheduling scheme I chose is Round Robin because is simple and easy to implement for this kind of demo project. Every thread has a slice of cpu time
and the scheduler chooses which thread will be run next, saves the current context and change everything in order to make the new thread able to resume its prior
interrupted execution.  

## The threads
The user of this scheduler is free to implement its own task, every classic Arduino function is working except for the ones that use Timer1, because this timer is 
used to wakeup the scheduler and has some strict time requirements and setting constrains.


## Conclusion
This is the first iteration of this project, I want to add so much more functionality, like tasks that can return, customizable scheduling settings, using a more clever
scheduling algorithm, detecting if a task has overwritten the stack of another task and so much more.


