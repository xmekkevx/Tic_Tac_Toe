/*
 * keyboard.h
 *
 *  Created on: Mar 28, 2026
 *      Author: a911353
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

#include "stdint.h"
#include "spi.h"

void keyboard_init(SPI_HandleTypeDef *hspi, uint8_t ms_rate);
void get_key_1ms(void);
void keyboard_callback(uint8_t key_val);

/* neu */
uint8_t keyboard_get_last_key(void);
void keyboard_clear_last_key(void);
uint8_t keyboard_get_key_blocking(void);
uint8_t keyboard_get_move_1_to_9(void);

#endif /* INC_KEYBOARD_H_ */
