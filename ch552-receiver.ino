#include <Arduino.h>
#include "src/HID.h"
#include "src/NRF.h"

void setup() {
  nrfInit();
  hidInit();
}

void loop() {
  hidInputPolling();
  nrfPrintInfo();
  delay(1);
}
