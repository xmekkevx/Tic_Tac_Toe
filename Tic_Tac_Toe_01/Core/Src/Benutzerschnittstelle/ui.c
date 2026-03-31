#include <Benutzerschnittstelle/input.h>
#include <Benutzerschnittstelle/ui.h>
#include <Hardwaretreiber/i2c_lcd.h>
#include "main.h"
#include <stdio.h>
#include <Spiellogik/ai.h>

/*
 * Eingang: selected
 * Verarbeitung: Erstellt abhängig von der Auswahl die Anzeige für das LCD
 * Ausgang: aktuelle Auswahl wird auf dem LCD dargestellt
 */
static void ui_draw_move_selection(uint8_t selected)
{
    char line2[9];

    if (selected == 0)
    {
        lcd_show_input("Ur turn:", "        ", 0, 1);
    }
    else
    {
        snprintf(line2, sizeof(line2), "%d OK? ", selected);
        lcd_show_input("Ur turn:", line2, 0, 1);
    }
}

/*
 * Eingang: -
 * Verarbeitung: Löscht den aktuellen Terminalinhalt
 * Ausgang: leeres Terminal
 */
void ui_clear_terminal(void)
{
    printf("\033[2J\033[H");
}

/*
 * Eingang: -
 * Verarbeitung: Gibt  Hilfe mit Spielfeld und Tastenbelegung aus
 * Ausgang: Hilfetext im Terminal
 */
void ui_print_help(void)
{
    printf("\n");
    printf("+=================================+\r\n");
    printf("|             HILFE               |\r\n");
    printf("+=================================+\r\n");

    printf("| Positionen:                     |\r\n");
    printf("|  1 | 2 | 3                      |\r\n");
    printf("| ---+---+---                     |\r\n");
    printf("|  4 | 5 | 6                      |\r\n");
    printf("| ---+---+---                     |\r\n");
    printf("|  7 | 8 | 9                      |\r\n");

    printf("+---------------------------------+\r\n");

    printf("| SPI-Tastatur:                   |\r\n");
    printf("| 1-9       = Feld waehlen        |\r\n");
    printf("| 11        = Ja                  |\r\n");
    printf("| 12        = Nein                |\r\n");
    printf("| 13        = Korrektur           |\r\n");
    printf("| 14        = Hilfe               |\r\n");
    printf("| 16        = Enter               |\r\n");

    printf("+---------------------------------+\r\n");

    printf("| Normale Tastatur:               |\r\n");
    printf("| 1-9       = Feld waehlen        |\r\n");
    printf("| y / n     = Ja / Nein           |\r\n");
    printf("| Backspace = Korrektur           |\r\n");
    printf("| h         = Hilfe               |\r\n");
    printf("| Enter     = Bestaetigen         |\r\n");

    printf("+=================================+\r\n\n");
}

/*
 * Eingang: -
 * Verarbeitung: Zeigt die Begrüßung auf LCD und Terminal an
 * Ausgang: Begrüßungsanzeige, danach leeres LCD
 */
void ui_greet(void)
{
    ui_clear_terminal();
    lcd_show_2lines("Welcome ", "to...  ");
    printf("Herzlichen Willkommen zu Tic Tac Toe!\r\n");
    HAL_Delay(2000);

    lcd_show_2lines(" TicTac ", "  Toe! ");
    HAL_Delay(2000);

    lcd_clr();
}

/*
 * Eingang: terminal_text, lcd_line1, lcd_line2, cursor_x, cursor_y
 * Verarbeitung: Zeigt eine Ja-/Nein-Frage an und wertet die Eingabe aus
 * Ausgang: 1 bei Ja, 0 bei Nein
 */
int ui_ask_yes_no_question(const char *terminal_text,
                           const char *lcd_line1,
                           const char *lcd_line2,
                           uint8_t cursor_x,
                           uint8_t cursor_y)
{
    input_key_t key;

    printf("%s\r\n", terminal_text);
    printf("Ja: y oder Taste 11 | Nein: n oder Taste 12\r\n");

    lcd_show_input(lcd_line1, lcd_line2, cursor_x, cursor_y);

    while (1)
    {
        key = input_read_key_blocking();

        if (key == INPUT_KEY_YES)
        {
            lcd_putc('Y');
            HAL_Delay(500);
            lcd_set_mode(LCD_MODE_DISPLAY);
            return 1;
        }
        else if (key == INPUT_KEY_NO)
        {
            lcd_putc('N');
            HAL_Delay(500);
            lcd_set_mode(LCD_MODE_DISPLAY);
            return 0;
        }
        else if (key == INPUT_KEY_HELP)
        {
            ui_clear_terminal();
            ui_print_help();
            printf("%s\r\n", terminal_text);
            printf("Ja: y oder Taste 11 | Nein: n oder Taste 12\r\n");
            lcd_show_input(lcd_line1, lcd_line2, cursor_x, cursor_y);
        }
    }
}

/*
 * Eingang: -
 * Verarbeitung: Fragt, ob das Spiel gestartet werden soll
 * Ausgang: 1 bei Ja, 0 bei Nein
 */
int ui_ask_start_game(void)
{
    return ui_ask_yes_no_question(
        "Moechtest du das Spiel starten?",
        "Start?",
        "Y/N ?",
        7,
        1
    );
}

/*
 * Eingang: -
 * Verarbeitung: Fragt, ob erneut gespielt werden soll
 * Ausgang: 1 bei Ja, 0 bei Nein
 */
int ui_ask_replay(void)
{
    return ui_ask_yes_no_question(
        "Moechtest du noch einmal spielen?",
        "Again?",
        "Y/N ?",
        7,
        1
    );
}

/*
 * Eingang: -
 * Verarbeitung: Fragt die gewünschte KI-Schwierigkeit ab
 * Ausgang: gewählte Schwierigkeitsstufe
 */
AiDifficulty ui_ask_ai_difficulty(void)
{
    input_key_t key;

    printf("Waehle die KI-Schwierigkeit:\r\n");
    printf("1 = Einfach\r\n");
    printf("2 = Mittel\r\n");
    printf("3 = Schwer\r\n");

    lcd_show_input("AI Level", "1/2/3?", 7, 1);

    while (1)
    {
        key = input_read_key_blocking();

        switch (key)
        {
            case INPUT_KEY_1:
                printf("Einfach gewaehlt.\r\n");
                lcd_show_2lines("AI:", "Easy");
                HAL_Delay(1000);
                return AI_EASY;

            case INPUT_KEY_2:
                printf("Mittel gewaehlt.\r\n");
                lcd_show_2lines("AI:", "Medium");
                HAL_Delay(1000);
                return AI_MEDIUM;

            case INPUT_KEY_3:
                printf("Schwer gewaehlt.\r\n");
                lcd_show_2lines("AI:", "Hard");
                HAL_Delay(1000);
                return AI_HARD;

            case INPUT_KEY_HELP:
                ui_clear_terminal();
                ui_print_help();
                printf("Menueauswahl: 1 = Einfach, 2 = Mittel, 3 = Schwer\r\n");
                lcd_show_input("AI Level", "1/2/3?", 7, 1);
                break;

            default:
                break;
        }
    }
}

/*
 * Eingang: -
 * Verarbeitung: Liest Feldwahl, Korrektur und Bestätigung des Spielers ein
 * Ausgang: gewähltes Feld von 1 bis 9
 */
uint8_t ui_ask_player_move(void)
{
    uint8_t selected = 0;
    input_key_t key;

    ui_draw_move_selection(selected);

    while (1)
    {
        key = input_read_key_blocking();

        if (key >= INPUT_KEY_1 && key <= INPUT_KEY_9)
        {
            uint8_t number = (uint8_t)key;

            if (selected == 0)
            {
                selected = number;
                ui_draw_move_selection(selected);
                printf("Aktuelle Auswahl: %d\r\n", selected);
            }
            else
            {
                printf("Auswahl %d bereits gesetzt. Erst korrigieren.\r\n", selected);
            }
        }
        else if (key == INPUT_KEY_BACKSPACE)
        {
            selected = 0;
            ui_draw_move_selection(selected);
            printf("Korrektur: Auswahl geloescht\r\n");
        }
        else if (key == INPUT_KEY_HELP)
        {
            ui_clear_terminal();
            ui_print_help();

            if (selected >= 1 && selected <= 9)
            {
                printf("Aktuelle Auswahl: %d\r\n", selected);
            }
            else
            {
                printf("Aktuelle Auswahl: keine\r\n");
            }

            ui_draw_move_selection(selected);
        }
        else if (key == INPUT_KEY_ENTER)
        {
            if (selected >= 1 && selected <= 9)
            {
                lcd_set_mode(LCD_MODE_DISPLAY);
                printf("Eingabe bestaetigt: %d\r\n", selected);
                return selected;
            }
            else
            {
                printf("Noch keine gueltige Auswahl vorhanden\r\n");
                lcd_show_input("Ur turn:", "No sel! ", 0, 1);
                HAL_Delay(700);
                ui_draw_move_selection(selected);
            }
        }
    }
}

/*
 * Eingang: -
 * Verarbeitung: Löscht das Terminal und zeigt die Hilfe an
 * Ausgang: vorbereitete Spielanzeige für neues Spiel
 */
void ui_prepare_new_game_screen(void)
{
    ui_clear_terminal();
    ui_print_help();
}

/*
 * Eingang: player
 * Verarbeitung: Erstellt den Text für den aktuellen Spieler und zeigt ihn an
 * Ausgang: Spieleranzeige auf dem LCD
 */
void ui_show_player_turn(char player)
{
    char line1[9];

    snprintf(line1, sizeof(line1), "Turn: %c", player);
    lcd_show_2lines(line1, "");
}

/*
 * Eingang: start_player
 * Verarbeitung: Zeigt den Spielstart und den Startspieler an
 * Ausgang: Ausgabe zum Spielbeginn
 */
void ui_show_game_start(char start_player)
{
    lcd_show_2lines("Game", "starts!");
    printf("\n============================\r\n");
    printf("Das Spiel beginnt!\r\n");
    printf("Startspieler: %c\r\n", start_player);
    printf("============================\r\n");
    HAL_Delay(2000);
}

/*
 * Eingang: -
 * Verarbeitung: Gibt die Eingabehinweise für den Spieler aus
 * Ausgang: Hinweistext im Terminal
 */
void ui_show_player_prompt(void)
{
    printf("\r\nDu bist dran!\r\n");
    printf("Waehle Feld 1-9, Taste 13/Backspace = Korrektur, Taste 14/h = Hilfe, Taste 16/Enter = Eingabe\r\n");
}

/*
 * Eingang: -
 * Verarbeitung: Zeigt an, dass die KI gerade berechnet
 * Ausgang: Statusanzeige für die KI
 */
void ui_show_ai_thinking(void)
{
    printf("\r\nKI denkt...\r\n");
    lcd_show_2lines("AI", "thinks...");
}

/*
 * Eingang: move
 * Verarbeitung: Gibt den von der KI gewählten Zug aus
 * Ausgang: Anzeige des KI-Zugs
 */
void ui_show_ai_move(uint8_t move)
{
    char aiMsg[9];

    printf("KI waehlt Feld %d\r\n", move);
    snprintf(aiMsg, sizeof(aiMsg), "%d", move);
    lcd_show_2lines("AI move:", aiMsg);
}

/*
 * Eingang: field
 * Verarbeitung: Meldet, dass das Feld bereits belegt ist
 * Ausgang: Hinweis auf ungültige Feldwahl
 */
void ui_show_field_taken(uint8_t field)
{
    printf("Feld %d ist bereits belegt!\r\n", field);
    lcd_show_2lines("Field", "Taken! ");
}

/*
 * Eingang: -
 * Verarbeitung: Zeigt an, dass die Spielerauswahl übernommen wurde
 * Ausgang: Bestätigungsanzeige auf dem LCD
 */
void ui_show_player_chosen(void)
{
    lcd_show_2lines("Chosen!", "");
}

/*
 * Eingang: -
 * Verarbeitung: Wartet kurz und zeigt danach den Sieg des Spielers an
 * Ausgang: Gewinnanzeige für den Spieler
 */
void ui_show_player_win(void)
{
    HAL_Delay(3000);
    ui_clear_terminal();
    printf("\r\nDu hast gewonnen!\r\n");
    lcd_show_2lines("You", "win!");
}

/*
 * Eingang: -
 * Verarbeitung: Wartet kurz und zeigt danach den Sieg der KI an
 * Ausgang: Gewinnanzeige für die KI
 */
void ui_show_ai_win(void)
{
    HAL_Delay(3000);
    ui_clear_terminal();
    printf("\r\nDie KI gewinnt!\r\n");
    lcd_show_2lines("AI", "wins!");
}

/*
 * Eingang: -
 * Verarbeitung: Zeigt ein Unentschieden an
 * Ausgang: Anzeige für Unentschieden
 */
void ui_show_draw(void)
{
    printf("\r\nUnentschieden!\r\n");
    lcd_show_2lines("Draw!", "");
}

/*
 * Eingang: -
 * Verarbeitung: Zeigt die Beendigung und eine Abschiedsanzeige an
 * Ausgang: geleerte Anzeige nach Programmende
 */
void ui_show_goodbye(void)
{
	printf("Programm wird beendet.\r\n");
	lcd_show_2lines("Program", "ended.");
	HAL_Delay(2000);
	ui_clear_terminal();

    lcd_show_2lines("See you", "later!");
    printf("Bis bald!\r\n");

    HAL_Delay(2000);

    ui_clear_terminal();
    lcd_clr();
}

/*
 * Eingang: game
 * Verarbeitung: Löscht das Terminal, gibt das Spielfeld aus und zeigt den aktuellen Spieler
 * Ausgang: aktueller Spielstand im Terminal
 */
void ui_show_game_screen(Game *game)
{
    ui_clear_terminal();
    printBoard(game);
    printf("\r\nAktueller Spieler: %c\r\n", game->currentPlayer);
}
