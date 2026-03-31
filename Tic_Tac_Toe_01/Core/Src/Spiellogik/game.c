#include <stdio.h>
#include <Spiellogik/game.h>


/*
 * Eingang: game
 * Verarbeitung: Setzt Spielstand, Startspieler und alle Felder auf Anfangswerte zurück
 * Ausgang: neues leeres Spiel
 */
void initGame(Game *game) {
    game->turnCounter = 0;
    game->currentPlayer = 'X';

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            game->board[i][j] = ' ';
        }
    }
}

/*
 * Eingang: game
 * Verarbeitung: Gibt das aktuelle Spielfeld im Terminal aus
 * Ausgang: sichtbare Darstellung des Spielfelds
 */
void printBoard(Game *game) {
    printf("\r\n");

    printf("+---+---+---+\r\n");
    for (int i = 0; i < SIZE; i++) {
        printf("| %c | %c | %c |\r\n",
               game->board[i][0],
               game->board[i][1],
               game->board[i][2]);

        printf("+---+---+---+\r\n");
    }

    printf("\n");
}

/*
 * Eingang: game, player
 * Verarbeitung: Prüft Reihen, Spalten und Diagonalen auf einen Sieg des Spielers
 * Ausgang: 1 bei Sieg, sonst 0
 */
int checkWin(Game *game, char player) {

    /* Reihen */
    for (int i = 0; i < SIZE; i++) {
        if (game->board[i][0] == player &&
            game->board[i][1] == player &&
            game->board[i][2] == player)
            return 1;
    }

    /* Spalten */
    for (int i = 0; i < SIZE; i++) {
        if (game->board[0][i] == player &&
            game->board[1][i] == player &&
            game->board[2][i] == player)
            return 1;
    }

    /* Diagonalen */
    if (game->board[0][0] == player &&
        game->board[1][1] == player &&
        game->board[2][2] == player)
        return 1;

    if (game->board[0][2] == player &&
        game->board[1][1] == player &&
        game->board[2][0] == player)
        return 1;

    return 0;
}

/*
 * Eingang: game
 * Verarbeitung: Prüft, ob bereits alle Züge verbraucht sind
 * Ausgang: 1 bei Unentschieden, sonst 0
 */
int isDraw(Game *game) {
    return game->turnCounter >= 9;
}

/*
 * Eingang: game, pos
 * Verarbeitung: Prüft die Position und setzt den Zug des aktuellen Spielers, falls das Feld frei ist
 * Ausgang: 1 bei erfolgreichem Zug, sonst 0
 */
int playerMove(Game *game, int pos) {

    if (pos < 1 || pos > 9)
        return 0;

    int row = (pos - 1) / 3;
    int col = (pos - 1) % 3;

    if (game->board[row][col] != ' ')
        return 0;

    game->board[row][col] = game->currentPlayer;
    game->turnCounter++;

    return 1;
}

/*
 * Eingang: game
 * Verarbeitung: Wechselt den aktuellen Spieler von X zu O oder von O zu X
 * Ausgang: currentPlayer ist auf den anderen Spieler gesetzt
 */
void switchPlayer(Game *game) {

    if (game->currentPlayer == 'X')
        game->currentPlayer = 'O';
    else
        game->currentPlayer = 'X';
}
