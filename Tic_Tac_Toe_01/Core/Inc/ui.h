#ifndef UI_H
#define UI_H

#include <stdint.h>
#include "game.h"

/* Outputs the board in the terminal. */
void ui_printBoard(Game *game);
void lcd_show_1line(uint8_t line, char* text);
void lcd_show_2lines(char* line1, char* line2);
void lcd_show_player(char player);
void greet(void);
void printHelp(void);

#endif
