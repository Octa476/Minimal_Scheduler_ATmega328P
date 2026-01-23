#include "task1.h"

// The user can defines task1 here, functions that use timers are allowed, but not functions that use Timer1.
void task1() {
    pinMode(3, OUTPUT);
    while (1) {
        for (int i = 0; i < 255; i++) {
            delay(10);
            // Serial.println(i);
            analogWrite(3, i);
        }
        for (int i = 255; i > 0; i--) {
            delay(10);
            // Serial.println(i);
            analogWrite(3, i);
        }
    }
}