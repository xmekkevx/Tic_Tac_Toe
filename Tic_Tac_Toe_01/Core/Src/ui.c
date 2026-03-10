#include <stdio.h>
#include "ui.h"

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
