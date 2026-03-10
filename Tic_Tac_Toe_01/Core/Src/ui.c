#include <stdio.h>
#include "stm32l4xx_hal.h"
#include "ui.h"
#include "i2c_lcd.h"

void ui_printBoard(Game *game) {
    printf("\r\n");

    for (int i = 0; i < SIZE; i++) {
        printf(" %c | %c | %c \r\n",
               game->board[i][0],
               game->board[i][1],
               game->board[i][2]);

        if (i < 2)
            printf("---+---+---\r\n");
    }

    printf("\n");
}

void lcd_show_1line(uint8_t line, char* text)
{
    if (line > 1) return;   // nur 0 oder 1 erlaubt

    lcd_cur_pos(0, line);
    lcd_put_str("        "); // 8 Leerzeichen = Zeile löschen

    lcd_cur_pos(0, line);
    lcd_put_str(text);
}

void lcd_show_2lines(char* line1, char* line2)
{
    lcd_clr();

    lcd_cur_pos(0,0);
    lcd_put_str(line1);

    lcd_cur_pos(0,1);
    lcd_put_str(line2);
}

void lcd_show_player(char player)
{
    char line1[9];
    char line2[9];

    snprintf(line1, 9, "Start:%c", player);
    snprintf(line2, 9, "Turn: %c", player);

    lcd_show_2lines(line1, line2);
}

void greet() {

	lcd_show_2lines("Welcome ", "to...  ");
	HAL_Delay(2000);

	lcd_show_2lines(" TicTac ", "  Toe! ");
	HAL_Delay(2000);


	lcd_clr();
}

void printHelp() {

    printf("\nPositionen:\r\n");

    printf(" 1 | 2 | 3 \r\n");

    printf("---+---+---\r\n");

    printf(" 4 | 5 | 6 \r\n");

    printf("---+---+---\r\n");

    printf(" 7 | 8 | 9 \r\n\r\n");
}
