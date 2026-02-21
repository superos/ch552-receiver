#include <Arduino.h>

#include "pins.h"

#include "LED.h"

static uint32_t ledOffTime = 0;

void ledInit() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void ledOn(uint32_t ms) {
  digitalWrite(LED, HIGH);
  ledOffTime = millis() + ms;
}

void ledOff() {
  if (millis() < ledOffTime) return;
  digitalWrite(LED, LOW);
  ledOffTime = 0;
}

void ledBlink() {
  // Slow blink: ~2s ON, ~2s OFF (2^11 = 2048ms)
  digitalWrite(LED, (millis() >> 11) & 1);
}
