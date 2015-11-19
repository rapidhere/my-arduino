#ifdef _BUILD_demo_blink

#define LED_PIN 13

#include "framework.h"

void setup() {
  pinMode(LED_PIN, OUTPUT);     // set pin as output
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // set the LED on
  delay(3000);                  // wait for a second
  digitalWrite(LED_PIN, LOW);   // set the LED off
  delay(3000);                  // wait for a second
}

#endif
