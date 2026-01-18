#include "task2.h"

void task2() {
    while (1) {
        // Serial.println("Hello from task1!");
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
}