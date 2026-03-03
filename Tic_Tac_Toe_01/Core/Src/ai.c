#include <stdlib.h>
#include "../Inc/ai.h"

int aiMove(Game *game) {
    int i, row, col;

    // 1. Gewinnchance prüfen
    for (i = 1; i <= 9; i++) {
        row = (i - 1) / 3;
        col = (i - 1) % 3;
        if (game->board[row][col] == ' ') {
            game->board[row][col] = game->currentPlayer;
            if (checkWin(game, game->currentPlayer)) {
                game->board[row][col] = ' ';
                return i; // sofort gewinnen
            }
            game->board[row][col] = ' ';
        }
    }

    // 2. Gegner blockieren
    char opponent = (game->currentPlayer == 'X') ? 'O' : 'X';
    for (i = 1; i <= 9; i++) {
        row = (i - 1) / 3;
        col = (i - 1) % 3;
        if (game->board[row][col] == ' ') {
            game->board[row][col] = opponent;
            if (checkWin(game, opponent)) {
                game->board[row][col] = ' ';
                return i; // blockieren
            }
            game->board[row][col] = ' ';
        }
    }

    // 3. Mitte nehmen
    if (game->board[1][1] == ' ')
        return 5;

    // 4. Ecken nehmen
    int corners[] = {1, 3, 7, 9};
    for (i = 0; i < 4; i++) {
        row = (corners[i] - 1) / 3;
        col = (corners[i] - 1) % 3;
        if (game->board[row][col] == ' ')
            return corners[i];
    }

    // 5. Seiten nehmen
    int sides[] = {2, 4, 6, 8};
    for (i = 0; i < 4; i++) {
        row = (sides[i] - 1) / 3;
        col = (sides[i] - 1) % 3;
        if (game->board[row][col] == ' ')
            return sides[i];
    }

    return -1; // kein Zug möglich
}
