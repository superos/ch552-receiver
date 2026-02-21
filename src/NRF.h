#ifndef __NRF_H__
#define __NRF_H__

#include <stdint.h>
#include <stdbool.h>

#define RX_ADDRESS  (uint8_t*)"G80S3"

void nrfInit();
bool nrfRead(uint8_t *buf);
void nrfSend(const uint8_t *buf, uint8_t len);
void nrfPrintInfo();

#endif
