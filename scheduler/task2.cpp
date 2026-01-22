#include "task2.h"

void task2() {
    pinMode(3, OUTPUT);
    while (1) {
        // digitalWrite(LED_BUILTIN, 0);
        // Serial.println("siua");
        // interrupts();
        delay(50);
        digitalWrite(3, !digitalRead(3));
    }
}