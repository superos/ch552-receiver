#include <Arduino.h>
#include <string.h>

#include "RF24/RF24.h"
#include "RF24/nRF24L01.h"
#include "pins.h"
#include "printf.h"

#include "NRF.h"

static uint8_t ping = 0;
static uint32_t logTime = 0;

void nrfInit() {
  RF24_Init(CE, CSN, SCK, MOSI, MISO);
  delay(100);

  RF24_WriteReg(CONFIG, _BV(PWR_UP) | RF24_CRC_16);
  delay(5);

  RF24_WriteReg(RF_CH, 101);
  RF24_WriteReg(RF_SETUP, RF24_2MBPS | RF24_PA_MAX);
  RF24_WriteReg(SETUP_AW, 0x03);
  RF24_WriteReg(SETUP_RETR, 0x0F);
  RF24_WriteReg(EN_AA, 0x3F);
  RF24_EnableAckPayload();
  RF24_WriteReg(EN_RXADDR, 0x01);
  RF24_WriteReg(RX_PW_P0, 8);
  RF24_WriteBuf(RX_ADDR_P0, RX_ADDRESS, 5);

  RF24_StartListening();
}

bool nrfRead(uint8_t *buf) {
  if (!RF24_IsDataReady()) return false;

  uint8_t tmp[8];
  RF24_SpiRead(R_RX_PAYLOAD, tmp, 8);
  RF24_ClearRxFlag();

  return tmp[0] == '#' ? (ping++, false) : (memcpy(buf, tmp, 8), true);
}

void nrfSend(const uint8_t *buf, uint8_t len) {
  RF24_WriteAckPayload(0, buf, len);
}

void nrfPrintInfo() {
  if (millis() - logTime < 5000) return;

  logTime = millis();
  static char rx[6] = {0};
  RF24_ReadBuf(RX_ADDR_P0, (uint8_t*)rx, 5);

  Printf("\n\n%s: 0x%02x, 0x%02x, %u", rx, RF24_ReadReg(STATUS), RF24_ReadReg(FIFO_STATUS), ping);
}
