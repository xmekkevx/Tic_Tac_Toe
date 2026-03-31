#include <Benutzerschnittstelle/input.h>
#include <Benutzerschnittstelle/ring_game.h>
#include <Benutzerschnittstelle/ui.h>
#include <Hardwaretreiber/i2c_lcd.h>
#include <Spiellogik/ai.h>
#include <Spiellogik/game_runner.h>
#include "main.h"
#include "rng.h"

/* Interne Hilfsfunktionen */
static void game_prepare_round(Game *game);
static void game_select_start_player(Game *game);
static void game_show_round_start(const Game *game);
static uint8_t game_handle_human_turn(Game *game);
static void game_handle_ai_turn(Game *game);
static uint8_t game_handle_end_state(Game *game);

/*
 * Eingang: game
 * Verarbeitung: Initialisiert die Runde und steuert den kompletten Spielablauf bis zum Ende
 * Ausgang: Spiel wird vollständig durchgeführt
 */
void game_run(Game *game)
{
    game_prepare_round(game);
    game_show_round_start(game);

    while (1)
    {
        ui_show_game_screen(game);

        if (game->currentPlayer == 'X')
        {
            if (!game_handle_human_turn(game))
            {
                continue;
            }
        }
        else
        {
            game_handle_ai_turn(game);
        }

        if (game_handle_end_state(game))
        {
            break;
        }

        switchPlayer(game);
        ui_show_player_turn(game->currentPlayer);
        HAL_Delay(1000);
    }
}

/*
 * Eingang: game
 * Verarbeitung: Setzt das Spiel zurück, bestimmt den Startspieler und initialisiert die Anzeige
 * Ausgang: Spiel ist für eine neue Runde vorbereitet
 */
static void game_prepare_round(Game *game)
{

    initGame(game);
    game_select_start_player(game);

    ring_game_init();
    ring_game_show_start_player(game->currentPlayer);
}

/*
 * Eingang: game
 * Verarbeitung: Bestimmt zufällig den Startspieler mittels Zufallszahl
 * Ausgang: currentPlayer wird auf 'X' oder 'O' gesetzt
 */
static void game_select_start_player(Game *game)
{
    uint32_t start_random = 0;

    HAL_RNG_GenerateRandomNumber(&hrng, &start_random);

    if ((start_random & 0x01u) == 0u)
    {
        game->currentPlayer = 'X';
    }
    else
    {
        game->currentPlayer = 'O';
    }
}

/*
 * Eingang: game
 * Verarbeitung: Zeigt den Start der Runde und den aktuellen Spieler an
 * Ausgang: Anzeige des Spielbeginns
 */
static void game_show_round_start(const Game *game)
{
    ui_show_game_start(game->currentPlayer);
    ui_show_player_turn(game->currentPlayer);
    HAL_Delay(1500);
}

/*
 * Eingang: game
 * Verarbeitung: Führt den Zug des Spielers aus und prüft auf gültige Eingabe
 * Ausgang: 1 bei erfolgreichem Zug, sonst 0
 */
static uint8_t game_handle_human_turn(Game *game)
{
    int input;

    ui_show_player_prompt();
    input = ui_ask_player_move();

    if (!playerMove(game, input))
    {
        ui_show_field_taken(input);
        HAL_Delay(1200);
        return 0;
    }

    ui_show_player_chosen();
    HAL_Delay(800);
    ring_game_update_turns(game->turnCounter);

    return 1;
}

/*
 * Eingang: game
 * Verarbeitung: Berechnet den KI-Zug und führt ihn aus
 * Ausgang: Spielzustand wird aktualisiert
 */
static void game_handle_ai_turn(Game *game)
{
    int move;

    ui_show_ai_thinking();
    HAL_Delay(2000);

    move = aiMove(game);

    ui_show_ai_move(move);
    HAL_Delay(1800);

    playerMove(game, move);
    ring_game_update_turns(game->turnCounter);
}

/*
 * Eingang: game
 * Verarbeitung: Prüft auf Sieg oder Unentschieden und zeigt das Ergebnis an
 * Ausgang: 1 wenn Spiel beendet, sonst 0
 */
static uint8_t game_handle_end_state(Game *game)
{
    if (checkWin(game, game->currentPlayer))
    {
        printBoard(game);

        if (game->currentPlayer == 'X')
        {
            ui_show_player_win();
            ring_game_animate_x_win();
            HAL_Delay(1000);
        }
        else
        {
            ui_show_ai_win();
            ring_game_animate_o_win();
            HAL_Delay(1000);
        }

        ring_game_clear();
        return 1;
    }

    if (isDraw(game))
    {
        printBoard(game);

        ui_show_draw();
        ring_game_clear();
        HAL_Delay(2000);

        return 1;
    }

    return 0;
}
