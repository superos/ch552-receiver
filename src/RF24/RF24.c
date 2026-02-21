#include "nRF24L01.h"
#include "RF24.h"

static uint8_t _pinCE, _pinCSN, _pinSCK, _pinMOSI, _pinMISO;

// =============================================================================
//   Internal SPI Helpers
// =============================================================================

static inline void csnSelect()   { digitalWrite(_pinCSN, LOW); }
static inline void csnDeselect() { digitalWrite(_pinCSN, HIGH); }

// =============================================================================
//   Initialization & Control
// =============================================================================

void RF24_Init(uint8_t ce, uint8_t csn, uint8_t sck, uint8_t mosi, uint8_t miso) {
  _pinCE = ce; _pinCSN = csn; _pinSCK = sck; _pinMOSI = mosi; _pinMISO = miso;
  pinMode(_pinCE, OUTPUT); pinMode(_pinCSN, OUTPUT);
  pinMode(_pinSCK, OUTPUT); pinMode(_pinMOSI, OUTPUT);
  pinMode(_pinMISO, INPUT);
  digitalWrite(_pinCE, LOW); csnDeselect(); digitalWrite(_pinSCK, LOW);
}

void RF24_CE(bool level) { digitalWrite(_pinCE, level); }

void RF24_StartListening() {
  RF24_WriteReg(CONFIG, RF24_ReadReg(CONFIG) | _BV(PWR_UP) | _BV(PRIM_RX));
  RF24_WriteReg(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
  csnSelect(); RF24_SpiTransfer(FLUSH_RX); csnDeselect();
  RF24_CE(HIGH);
}

void RF24_EnableAckPayload() {
  RF24_WriteReg(FEATURE, RF24_ReadReg(FEATURE) | _BV(EN_ACK_PAY) | _BV(EN_DPL));
  RF24_WriteReg(DYNPD, RF24_ReadReg(DYNPD) | _BV(DPL_P0) | _BV(DPL_P1));
}

void RF24_WriteAckPayload(uint8_t pipe, const uint8_t *buf, uint8_t len) {
  // ack payload used for led state only
  // if tx fifo is full, old state can be safely dropped
  // the latest state is what matters
  if (RF24_ReadReg(FIFO_STATUS) & _BV(TX_FULL)) {
    csnSelect(); RF24_SpiTransfer(FLUSH_TX); csnDeselect();
  }
  RF24_SpiWrite(W_ACK_PAYLOAD | (pipe & 0x7), buf, len);
}

// =============================================================================
//   Register Access
// =============================================================================

uint8_t RF24_ReadReg(uint8_t reg) {
  csnSelect();
  RF24_SpiTransfer(R_REGISTER | (reg & 0x1F));
  uint8_t result = RF24_SpiTransfer(NOP);
  csnDeselect();
  return result;
}

uint8_t RF24_WriteReg(uint8_t reg, uint8_t value) {
  csnSelect();
  RF24_SpiTransfer(W_REGISTER | (reg & 0x1F));
  uint8_t result = RF24_SpiTransfer(value);
  csnDeselect();
  return result;
}

void RF24_ReadBuf(uint8_t reg, uint8_t *buf, uint8_t len) {
  RF24_SpiRead(R_REGISTER | (reg & 0x1F), buf, len);
}

void RF24_WriteBuf(uint8_t reg, const uint8_t *buf, uint8_t len) {
  RF24_SpiWrite(W_REGISTER | (reg & 0x1F), buf, len);
}

bool RF24_IsDataReady() { return !(RF24_ReadReg(FIFO_STATUS) & _BV(RX_EMPTY)); }
void RF24_ClearRxFlag() { RF24_WriteReg(STATUS, _BV(RX_DR)); }

// =============================================================================
//   Low Level SPI
// =============================================================================

uint8_t RF24_SpiTransfer(uint8_t data) {
  uint8_t result = 0;
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(_pinMOSI, (data & 0x80));
    data <<= 1;
    digitalWrite(_pinSCK, HIGH);
    result = (result << 1) | digitalRead(_pinMISO);
    digitalWrite(_pinSCK, LOW);
  }
  return result;
}

void RF24_SpiRead(uint8_t cmd, uint8_t *buf, uint8_t len) {
  csnSelect();
  RF24_SpiTransfer(cmd);
  for(uint8_t i = 0; i < len; i++) buf[i] = RF24_SpiTransfer(NOP);
  csnDeselect();
}

void RF24_SpiWrite(uint8_t cmd, const uint8_t *buf, uint8_t len) {
  csnSelect();
  RF24_SpiTransfer(cmd);
  for(uint8_t i = 0; i < len; i++) RF24_SpiTransfer(buf[i]);
  csnDeselect();
}
