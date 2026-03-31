#ifndef AI_H
#define AI_H

#include "game.h"

/* Berechnet nächsten KI-Zug babbbabbaababa */
int aiMove(Game *game);

/* Liefert lesbaren Namen fuer Schwierigkeitsgrad */
const char* aiDifficultyToString(AiDifficulty difficulty);

#endif
