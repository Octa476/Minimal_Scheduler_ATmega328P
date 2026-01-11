# Minimal_Scheduler_ATmega328P
I will try to build a minimal scheduler on the ATmega328P microcontroller using the arduino IDE and some AVR assembly.

# My idea
Maybe my idea won't work, but I will try to spell it in this file for better organization.
I already created a structure for the whole project with all the folders and files that are necessary.

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
In this project I will try to build a preemptive minimal scheduler that will enable multiprogramming in an Arduino Uno environment. When the "minimal os" will start a single "thread" will be launched and that thread will take the role of an interactive terminal from which other threads can be launched or terminated. These threads can represent different applications like ono that measures temperatures and displays it to an LCD, other that control some LEDs and so on. Some of the tasks will be created by myself, but a user of this project can create its own tasks(by programming one) and launch them into execution or terminate them.

With the use of a timer interrupt the scheduler will be woken at a regular interval of time that can be set by the user. The features of the timers will be used to set this interval to a desired value and the whole schelduler awakening should be handled as a service interrupt routine.

When the interrupt is given, the routine will start and the context of the current thread will be saved in a special structure.
The "context" is a set that contains:
- the value of all the general purpose registers
- the value of the instruction pointer(instruction register)
- the value of the current stack pointer and base stack pointer

This "context switch" will be handled by an inline assembly routine, and after all the necessary information of the current thread is saved, the resources will be given to a new thread. All the information of the new context will be uploaded to the registers and the new thread will start running from where it was suspended in a past context switch.

The schelduling algorithm I will use is Round Robin, because is easy to implement and it prevents starvation. Every thread will receive a slice of CPU time in a cyclic order, perfectly for my minimal schelduler.

A thread will have the ability to return(to exit/to finish its execution), so when this happens the sheduler will free the resources that were allocated to that thread and update the necessary structures.

## The threads
A main thread will be created at the start of the minimal os and this thread will act as the link between the user and the scheduler. From this terminal thread, the user will be able to launch into execution other threads or kill the running ones.

Every thread will receive a slice of the SRAM and that slice will represent its whole stack, when a thread will try to go past its allocated stack I will try to build a mechanism that will be capable of detecting this overflow and kill the thread that violated the constrain.

I still need to think how all of these things will work and especially how the global variables should be handled with no virtual memory, I need to study more these aspects.

## Conclusion
I will try my best to build this whole system as a personal project and I hope I will learn a lot while doing it. 


