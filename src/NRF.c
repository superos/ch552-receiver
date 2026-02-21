#include <Arduino.h>
#include <string.h>

#include "RF24/RF24.h"
#include "RF24/nRF24L01.h"
#include "pins.h"
#include "printf.h"

#include "NRF.h"

void nrfInit() {}

bool nrfRead(uint8_t *buf) { return false; }

void nrfSend(const uint8_t *buf, uint8_t len) {}

void nrfPrintInfo() {}
