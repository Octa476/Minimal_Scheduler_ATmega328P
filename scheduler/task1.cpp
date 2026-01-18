#include "task1.h"

// __attribute__((naked, noreturn))
void task1() {
    while (1) {
        Serial.println("buna");
        delay(1000);
        // int i = 0;
        // while (i < 20000) {
        //     i++;
        //     int j = 0;
        //     while (j < 200) {
        //         j++;
        //     }
        // }
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
    // asm volatile (
    //     // LED_BUILTIN = PB5
    //     "ldi r16, (1<<5)     \n\t" // r16 = 0b00100000
    //     "loop:               \n\t"
    //         "ldi r20, (1<<7) \n\t"
    //         "loop1: \n\t"
    //             "dec r20 \n\t"
    //             "ldi r21, (1<<7) \n\t"
    //             "loop2: \n\t"
    //                 "dec r21 \n\t"
    //                 "ldi r22, (1<<7) \n\t"
    //                 "loop3: \n\t"
    //                     "dec r22 \n\t"
    //                 "cpi r22, 0 \n\t"
    //                 "brne loop3 \n\t"
    //             "cpi r21, 0 \n\t"
    //             "brne loop2 \n\t"
    //         "cpi r20, 0 \n\t"
    //         "brne loop1 \n\t"
    //         "in r17, 0x05        \n\t" // PORTB
    //         "eor r17, r16        \n\t" // toggle bit 5
    //         "out 0x05, r17      \n\t"
    //     "rjmp loop           \n\t"
    // );
}