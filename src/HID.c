#include <Arduino.h>
#include <string.h>

#include "CdcHidCombo/USBCDC.h"
#include "NRF.h"
#include "LED.h"

#include "HID.h"

extern uint8_t HIDKey[8];
extern void USB_EP3_send();

#define SUSPENDED (USB_MIS_ST & bUMS_SUSPEND)
#define EP3_READY ((UEP3_CTRL & MASK_UEP_T_RES) == UEP_T_RES_NAK)
#define IDLE_MS   (5 * 60 * 1000UL)

static void wakeupComputer() {
  static uint8_t ignore[8];
  UDEV_CTRL |= bUD_LOW_SPEED, delay(20);
  UDEV_CTRL &= ~bUD_LOW_SPEED, delay(20);
  while (SUSPENDED) { nrfRead(ignore), delay(3); } // keep rf24 alive and ignore any data
}

void hidInit() { ledInit(), USBInit(); }

void hidInputPolling() {
  static uint32_t lastActive = 0;
  bool hasReport = nrfRead(HIDKey);
  bool keyStrike = hasReport && (memcmp(HIDKey, ZEROS, 8) != 0);

  if (SUSPENDED)
    if (keyStrike)
      wakeupComputer(), nrfInit();

  if (hasReport)
    if (EP3_READY)
      USB_EP3_send();

  if (keyStrike)
    return lastActive = millis(), ledOn(333);

  (millis() - lastActive) > IDLE_MS ? ledBlink() : ledOff();
}

void hidOutputCallback(uint8_t ledStatus) { ledOn(333), nrfSend(&ledStatus, 1); }
