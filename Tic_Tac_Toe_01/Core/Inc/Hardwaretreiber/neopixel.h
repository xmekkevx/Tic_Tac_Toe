#ifndef INC_HARDWARETREIBER_NEOPIXEL_H_
#define INC_HARDWARETREIBER_NEOPIXEL_H_

#include <stdint.h>

#define NEOPIXEL_LED_COUNT 12
#define NEOPIXEL_RESET_SLOTS 50
#define NEOPIXEL_BITS_PER_LED 24
#define NEOPIXEL_BUFFER_SIZE ((NEOPIXEL_LED_COUNT * NEOPIXEL_BITS_PER_LED) + NEOPIXEL_RESET_SLOTS)

void neopixel_init(void);
void neopixel_clear(void);
void neopixel_set_led(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
void neopixel_fill(uint8_t red, uint8_t green, uint8_t blue);
void neopixel_show(void);

#endif /* INC_HARDWARETREIBER_NEOPIXEL_H_ */
