#ifndef __PRINTF_H__
#define __PRINTF_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

// USB Serial Function (defined in src/CdcHidCombo/USBCDC.c)
extern uint8_t USBSerial_write(char c);
extern void USBSerial_flush();
extern volatile __bit UpPoint2BusyFlag;

/**
 * Robust Simplified Printf for 8051/SDCC
 * 
 * DESIGN GOALS:
 * 1. Safety: No 32-bit integer support to prevent Stack Overflow on 8051.
 * 2. Simplicity: Minimal parsing logic to reduce Flash usage and bugs.
 * 3. Compatibility: Supports standard 16-bit format specifiers used in this project.
 * 
 * SUPPORTED FORMATS:
 *   %s       - Null-terminated String
 *   %c       - Character
 *   %d, %i   - Signed Integer (16-bit)
 *   %u       - Unsigned Integer (16-bit)
 *   %x, %X   - Hexadecimal (16-bit)
 *   %p       - Pointer (16-bit address)
 *   %%       - Literal '%'
 * 
 * MODIFIERS:
 *   %02x     - Zero padding (only checks for '0' and a single digit width)
 *   %l...    - 'l' modifier is IGNORED (treated as 16-bit) -> WARNING: Do not pass 32-bit args!
 */
static inline void Printf(const char *fmt, ...) {
  // avoid blocking arduino loop() when USB Serial broken abnormally: wire unplugged, program quited.
  // ch5xx_usb.h uses 64 bytes MAX_PACKET_SIZE, so this patch can't avoid blocking while printing long strings.
  // timeout = 250ms per extra character, e.g. (68 - 64) * 250ms = 1000ms
  // if you want to avoid blocking completely, patch `USBSerial_write` in `USBCDC.c`.
  if (UpPoint2BusyFlag) return;

  const char hex_chars[] = "0123456789abcdef";
  va_list args;
  va_start(args, fmt);
  
  while (*fmt) {
    if (*fmt != '%') {
      USBSerial_write(*fmt++);
      continue;
    }
    
    fmt++; // Skip '%'
    
    // Declare variables at the top of the block to avoid scope issues
    bool zero_pad = false;
    uint8_t width = 0;
    
    // Parse Zero Padding
    if (*fmt == '0') {
      zero_pad = true;
      fmt++;
    }
    
    // Parse Width
    if (*fmt >= '0' && *fmt <= '9') {
      width = *fmt - '0';
      fmt++;
    }
    
    // Ignore 'l' modifier
    if (*fmt == 'l') fmt++;
    
    switch (*fmt) {
      case 'c': {
        char c = (char)va_arg(args, int);
        USBSerial_write(c);
        break;
      }
      
      case 's': {
        const char *s = va_arg(args, const char *);
        if (!s) s = "(null)";
        while (*s) USBSerial_write(*s++);
        break;
      }
      
      case 'd':
      case 'i': {
        int16_t val = va_arg(args, int);
        if (val < 0) {
          USBSerial_write('-');
          val = -val;
          if (width > 0) width--;
        }
        
        char buf[6];
        uint8_t i = 0;
        uint16_t uval = (uint16_t)val;
        
        if (uval == 0) buf[i++] = '0';
        else while (uval > 0) {
          buf[i++] = '0' + (uval % 10);
          uval /= 10;
        }
        
        while (i < width) buf[i++] = (zero_pad ? '0' : ' ');
        while (i > 0) USBSerial_write(buf[--i]);
        break;
      }
      
      case 'u': {
        uint16_t val = va_arg(args, unsigned int);
        char buf[6];
        uint8_t i = 0;
        
        if (val == 0) buf[i++] = '0';
        else while (val > 0) {
          buf[i++] = '0' + (val % 10);
          val /= 10;
        }
        
        while (i < width) buf[i++] = (zero_pad ? '0' : ' ');
        while (i > 0) USBSerial_write(buf[--i]);
        break;
      }
      
      case 'x':
      case 'X':
      case 'p': { 
        uint16_t val = va_arg(args, unsigned int);
        char buf[5];
        uint8_t i = 0;
        
        if (*fmt == 'p') {
            USBSerial_write('0'); USBSerial_write('x');
            width = 0; 
        }

        if (val == 0) buf[i++] = '0';
        else while (val > 0 || (zero_pad && i < width)) {
            buf[i++] = hex_chars[val & 0x0F];
            val >>= 4;
            // Break if we finished value and padding isn't needed
            if (val == 0 && !zero_pad) break; 
            // Safety break to prevent infinite loop if logic fails
            if (i >= 5) break;
        }
        
        while (i < width && i < 5) buf[i++] = (zero_pad ? '0' : ' ');
        while (i > 0) USBSerial_write(buf[--i]);
        break;
      }
      
      default:
        USBSerial_write('%');
        USBSerial_write(*fmt);
        break;
    }
    fmt++;
  }
  
  va_end(args);
  // Flush immediately to prevent broken lines
  USBSerial_flush(); 
}

#endif
