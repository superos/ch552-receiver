#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

void ledInit();
void ledOn(uint32_t ms);
void ledOff();
void ledBlink();

#endif 