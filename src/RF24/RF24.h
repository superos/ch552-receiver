#ifndef __RF24_H__
#define __RF24_H__

#include <Arduino.h>

// --- Initialization & Control ---
void RF24_Init(uint8_t ce, uint8_t csn, uint8_t sck, uint8_t mosi, uint8_t miso);
void RF24_CE(bool level);
void RF24_StartListening();
void RF24_EnableAckPayload();
void RF24_WriteAckPayload(uint8_t pipe, const uint8_t *buf, uint8_t len);

// --- Register Access ---
uint8_t RF24_ReadReg(uint8_t reg);
uint8_t RF24_WriteReg(uint8_t reg, uint8_t value);
void RF24_ReadBuf(uint8_t reg, uint8_t *buf, uint8_t len);
void RF24_WriteBuf(uint8_t reg, const uint8_t *buf, uint8_t len);

bool RF24_IsDataReady();
void RF24_ClearRxFlag();

// --- Low Level SPI ---
uint8_t RF24_SpiTransfer(uint8_t data);
void RF24_SpiRead(uint8_t cmd, uint8_t *buf, uint8_t len);
void RF24_SpiWrite(uint8_t cmd, const uint8_t *buf, uint8_t len);

#endif
