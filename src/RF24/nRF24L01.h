#ifndef __NRF24L01_H__
#define __NRF24L01_H__

/* nRF24L01+ Register Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define RPD         0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD       0x1C
#define FEATURE     0x1D

/* SPI Commands */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define R_RX_PL_WID   0x60
#define W_ACK_PAYLOAD 0xA8
#define W_TX_PAYLOAD_NOACK 0xB0
#define NOP           0xFF

/* Bit Mnemonics */
// CONFIG
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0

// RF_SETUP
#define CONT_WAVE   7
#define RF_DR_LOW   5
#define PLL_LOCK    4
#define RF_DR_HIGH  3
#define RF_PWR_LOW  1
#define RX_EMPTY    0
#define RF_PWR_HIGH 2

// DYNPD (Dynamic Payload Length)
#define DPL_P5      5
#define DPL_P4      4
#define DPL_P3      3
#define DPL_P2      2
#define DPL_P1      1
#define DPL_P0      0

// FEATURE
#define EN_DPL      2  /* Enable Dynamic Payload Length */
#define EN_ACK_PAY  1  /* Enable Payload with ACK */
#define EN_DYN_ACK  0  /* Enable W_TX_PAYLOAD_NOACK Command */

// STATUS
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1 /* 3 bits */
#define TX_FULL     0

/* RF24 Library Compatibility Constants */
#define RF24_PA_MIN       0
#define RF24_PA_LOW       2
#define RF24_PA_HIGH      4
#define RF24_PA_MAX       6
#define RF24_LNA_ON       1

#define RF24_1MBPS        0
#define RF24_2MBPS        8
#define RF24_250KBPS      32

#define RF24_CRC_DISABLED 0
#define RF24_CRC_8        8
#define RF24_CRC_16       12

// Helper macro for bit value if not already defined
#ifndef _BV
#define _BV(x) (1<<(x))
#endif

#endif
