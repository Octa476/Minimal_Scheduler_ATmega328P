#include "task2.h"

// The user can defines task2 here, functions that use timers are allowed, but not functions that use Timer1.
void task2() {
    pinMode(2, OUTPUT);
    while (1) {
        delay(50);
        digitalWrite(2, !digitalRead(2));
    }
}