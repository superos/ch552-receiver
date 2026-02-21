#ifndef __HID_H__
#define __HID_H__

#include <stdint.h>

#define ZEROS "\0\0\0\0\0\0\0\0"

void hidInit();
void hidInputPolling();
void hidOutputCallback(uint8_t ledStatus);

#endif
