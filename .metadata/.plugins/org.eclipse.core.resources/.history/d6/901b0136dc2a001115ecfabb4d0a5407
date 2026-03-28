/*
 * Funktionen zum Midas I2C-LCD 2x8 (MC20805A6W-BNMLWI-V2)
 *
 * Version 1.0
 *
 * Joenen / HS Osnabrueck 10.01.2024
 */
#ifndef INC_I2C_LCD_H_
#define INC_I2C_LCD_H_

#include "main.h"
#include "i2c.h"
typedef enum { off, on } cursor_mode_t;

void lcd_init(I2C_HandleTypeDef* hi2c);
void lcd_clr(void);
void lcd_putc(char c);
void lcd_put_str(char* str);
void lcd_cur_pos(uint8_t x, uint8_t y);
void lcd_set_cur_blink(cursor_mode_t cur, cursor_mode_t blink);
#endif /* INC_I2C_LCD_H_ */


