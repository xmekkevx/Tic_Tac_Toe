#ifndef GAME_H
#define GAME_H

#define SIZE 3

typedef struct {
    char board[SIZE][SIZE];
    int turnCounter;
    char currentPlayer;
} Game;

/* Initialisiert das Spielfeld */
void initGame(Game *game);

/* Gibt das Spielfeld aus */
void printBoard(Game *game);

/* Prüft ob jemand gewonnen hat */
int checkWin(Game *game, char player);

/* Prüft ob das Feld voll ist */
int isDraw(Game *game);

/* Führt Spieler-Zug aus */
int playerMove(Game *game, int pos);

/* Wechselt Spieler */
void switchPlayer(Game *game);

#endif
