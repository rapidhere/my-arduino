#ifdef _BUILD_demo_blink

#include "rlib/framework.h"
#include "rlib/commondata.h"

void setup() {
    pinMode(LED_PIN, OUTPUT);     // set pin as output
}

void loop() {
    digitalWrite(LED_PIN, HIGH);  // set the LED on
    delay(1000);                  // wait for a second
    digitalWrite(LED_PIN, LOW);   // set the LED off
    delay(1000);                  // wait for a second
}

#endif
