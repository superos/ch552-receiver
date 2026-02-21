/**
 *  Onboard LED
 *  HIGH = ON, LOW = OFF
 */
#define LED 32

/**
 * NRF24L01+ SPI
 * - CSN, CE, MOSI, SCK need level shifter.
 * - MISO is direct.
 * - Use level shifter module or (5v*2/3) resistor divider.
 * - Soldering 10uF-47uF capacitor on Si24R1 VCC/GND for stability.
 */
#define CE     33
#define SCK    34
#define MISO   11
#define CSN    30
#define MOSI   31
