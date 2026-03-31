#include <stdlib.h>
#include <Spiellogik/ai.h>

/*
 * Eingang: game
 * Verarbeitung: Sucht alle freien Felder und wählt eines zufällig aus
 * Ausgang: Nummer eines freien Feldes oder -1
 */
static int aiGetRandomMove(Game *game)
{
    int freeFields[9];
    int count = 0;

    for (int i = 1; i <= 9; i++) {
        int row = (i - 1) / 3;
        int col = (i - 1) % 3;

        if (game->board[row][col] == ' ') {
            freeFields[count] = i;
            count++;
        }
    }

    if (count == 0)
        return -1;

    return freeFields[rand() % count];
}

/*
 * Eingang: game, player
 * Verarbeitung: Prüft für jedes freie Feld, ob damit direkt ein Gewinn möglich ist
 * Ausgang: Gewinnfeld des Spielers oder -1
 */
static int aiFindWinningMove(Game *game, char player)
{
    int i, row, col;

    for (i = 1; i <= 9; i++) {
        row = (i - 1) / 3;
        col = (i - 1) % 3;

        if (game->board[row][col] == ' ') {
            game->board[row][col] = player;

            if (checkWin(game, player)) {
                game->board[row][col] = ' ';
                return i;
            }

            game->board[row][col] = ' ';
        }
    }

    return -1;
}

/*
 * Eingang: game
 * Verarbeitung: Wählt einen zufälligen gültigen Zug
 * Ausgang: Feldnummer oder -1
 */
static int aiMoveEasy(Game *game)
{
    return aiGetRandomMove(game);
}

/*
 * Eingang: game
 * Verarbeitung: Prüft zuerst eigenen Gewinnzug, dann Blockierung des Gegners, sonst Zufallszug
 * Ausgang: gewähltes Feld oder -1
 */
static int aiMoveMedium(Game *game)
{
    int move;
    char opponent = (game->currentPlayer == 'X') ? 'O' : 'X';

    /* 1. Selbst gewinnen, wenn moeglich */
    move = aiFindWinningMove(game, game->currentPlayer);
    if (move != -1)
        return move;

    /* 2. Gegner blockieren */
    move = aiFindWinningMove(game, opponent);
    if (move != -1)
        return move;

    /* 3. Sonst zufaellig */
    return aiGetRandomMove(game);
}

/*
 * Eingang: game
 * Verarbeitung: Prüft Gewinnzug, Blockierung, Mitte, Ecken und Seiten in dieser Reihenfolge
 * Ausgang: bestmöglich gewähltes Feld oder -1
 */
static int aiMoveHard(Game *game)
{
    int i, row, col;
    int move;
    char opponent = (game->currentPlayer == 'X') ? 'O' : 'X';

    /* 1. Gewinnchance pruefen */
    move = aiFindWinningMove(game, game->currentPlayer);
    if (move != -1)
        return move;

    /* 2. Gegner blockieren */
    move = aiFindWinningMove(game, opponent);
    if (move != -1)
        return move;

    /* 3. Mitte nehmen */
    if (game->board[1][1] == ' ')
        return 5;

    /* 4. Ecken nehmen */
    int corners[] = {1, 3, 7, 9};
    for (i = 0; i < 4; i++) {
        row = (corners[i] - 1) / 3;
        col = (corners[i] - 1) % 3;

        if (game->board[row][col] == ' ')
            return corners[i];
    }

    /* 5. Seiten nehmen */
    int sides[] = {2, 4, 6, 8};
    for (i = 0; i < 4; i++) {
        row = (sides[i] - 1) / 3;
        col = (sides[i] - 1) % 3;

        if (game->board[row][col] == ' ')
            return sides[i];
    }

    return -1;
}

/*
 * Eingang: game
 * Verarbeitung: Wählt abhängig von der eingestellten Schwierigkeit die passende KI-Logik
 * Ausgang: berechnetes Feld für den KI-Zug
 */
int aiMove(Game *game)
{
    switch (game->aiDifficulty) {
        case AI_EASY:
            return aiMoveEasy(game);

        case AI_MEDIUM:
            return aiMoveMedium(game);

        case AI_HARD:
            return aiMoveHard(game);

        default:
            return aiMoveHard(game);
    }
}

/*
 * Eingang: difficulty
 * Verarbeitung: Ordnet der Schwierigkeitsstufe den passenden Text zu
 * Ausgang: Text zur Schwierigkeitsstufe
 */
const char* aiDifficultyToString(AiDifficulty difficulty)
{
    switch (difficulty) {
        case AI_EASY:
            return "Einfach";
        case AI_MEDIUM:
            return "Mittel";
        case AI_HARD:
            return "Schwer";
        default:
            return "Unbekannt";
    }
}
