#include <Benutzerschnittstelle/ring_game.h>
#include <Hardwaretreiber/neopixel.h>
#include "main.h"

#define COLOR_X_R 255
#define COLOR_X_G 0
#define COLOR_X_B 0

#define COLOR_O_R 0
#define COLOR_O_G 255
#define COLOR_O_B 0

static char start_player = 'X';
static uint8_t led_state[NEOPIXEL_LED_COUNT][3];

/*
 * Eingang: -
 * Verarbeitung: Setzt alle gespeicherten LED-Farbwerte auf 0
 * Ausgang: LED-Zustand im Array ist vollständig gelöscht
 */
static void ring_game_clear_state(void)
{
    for (uint8_t i = 0; i < NEOPIXEL_LED_COUNT; i++)
    {
        led_state[i][0] = 0;
        led_state[i][1] = 0;
        led_state[i][2] = 0;
    }
}

/*
 * Eingang: -
 * Verarbeitung: Überträgt die gespeicherten LED-Farbwerte auf den Neopixel-Ring
 * Ausgang: aktueller LED-Zustand wird am Ring angezeigt
 */
static void ring_game_push_state_to_ring(void)
{
    neopixel_clear();

    for (uint8_t i = 0; i < NEOPIXEL_LED_COUNT; i++)
    {
        neopixel_set_led(i, led_state[i][0], led_state[i][1], led_state[i][2]);
    }

    neopixel_show();
}

/*
 * Eingang: -
 * Verarbeitung: Verschiebt alle LED-Zustände im Array um eine Position nach links
 * Ausgang: LED-Zustand ist einmal nach links rotiert
 */
static void ring_game_rotate_left_once(void)
{
    uint8_t first_r = led_state[0][0];
    uint8_t first_g = led_state[0][1];
    uint8_t first_b = led_state[0][2];

    for (uint8_t i = 0; i < (NEOPIXEL_LED_COUNT - 1); i++)
    {
        led_state[i][0] = led_state[i + 1][0];
        led_state[i][1] = led_state[i + 1][1];
        led_state[i][2] = led_state[i + 1][2];
    }

    led_state[NEOPIXEL_LED_COUNT - 1][0] = first_r;
    led_state[NEOPIXEL_LED_COUNT - 1][1] = first_g;
    led_state[NEOPIXEL_LED_COUNT - 1][2] = first_b;
}

/*
 * Eingang: -
 * Verarbeitung: Verschiebt alle LED-Zustände im Array um eine Position nach rechts
 * Ausgang: LED-Zustand ist einmal nach rechts rotiert
 */
static void ring_game_rotate_right_once(void)
{
    uint8_t last_r = led_state[NEOPIXEL_LED_COUNT - 1][0];
    uint8_t last_g = led_state[NEOPIXEL_LED_COUNT - 1][1];
    uint8_t last_b = led_state[NEOPIXEL_LED_COUNT - 1][2];

    for (int i = NEOPIXEL_LED_COUNT - 1; i > 0; i--)
    {
        led_state[i][0] = led_state[i - 1][0];
        led_state[i][1] = led_state[i - 1][1];
        led_state[i][2] = led_state[i - 1][2];
    }

    led_state[0][0] = last_r;
    led_state[0][1] = last_g;
    led_state[0][2] = last_b;
}

/*
 * Eingang: -
 * Verarbeitung: Setzt Startspieler auf X, löscht den LED-Zustand und überträgt ihn auf den Ring
 * Ausgang: Ring ist zurückgesetzt und zeigt keinen aktiven Spielstand
 */
void ring_game_init(void)
{
    start_player = 'X';
    ring_game_clear_state();
    ring_game_push_state_to_ring();
}

/*
 * Eingang: player (Startspieler als 'X' oder 'O')
 * Verarbeitung: Speichert den Startspieler, löscht den Ringzustand und setzt die erste LED in passender Farbe
 * Ausgang: Startspieler wird durch die erste LED angezeigt
 */
void ring_game_show_start_player(char player)
{
    start_player = player;
    ring_game_clear_state();

    if (player == 'X')
    {
        led_state[0][0] = COLOR_X_R;
        led_state[0][1] = COLOR_X_G;
        led_state[0][2] = COLOR_X_B;
    }
    else
    {
        led_state[0][0] = COLOR_O_R;
        led_state[0][1] = COLOR_O_G;
        led_state[0][2] = COLOR_O_B;
    }

    ring_game_push_state_to_ring();
}

/*
 * Eingang: turn_count (Anzahl der gespielten Züge)
 * Verarbeitung: Setzt den LED-Zustand neu und färbt die LEDs abwechselnd passend zum Startspieler
 * Ausgang: Ring zeigt die bisherige Zugreihenfolge farblich an
 */
void ring_game_update_turns(uint8_t turn_count)
{
    ring_game_clear_state();

    for (uint8_t i = 0; i < turn_count && i < NEOPIXEL_LED_COUNT; i++)
    {
        if ((i % 2) == 0)
        {
            if (start_player == 'X')
            {
                led_state[i][0] = COLOR_X_R;
                led_state[i][1] = COLOR_X_G;
                led_state[i][2] = COLOR_X_B;
            }
            else
            {
                led_state[i][0] = COLOR_O_R;
                led_state[i][1] = COLOR_O_G;
                led_state[i][2] = COLOR_O_B;
            }
        }
        else
        {
            if (start_player == 'X')
            {
                led_state[i][0] = COLOR_O_R;
                led_state[i][1] = COLOR_O_G;
                led_state[i][2] = COLOR_O_B;
            }
            else
            {
                led_state[i][0] = COLOR_X_R;
                led_state[i][1] = COLOR_X_G;
                led_state[i][2] = COLOR_X_B;
            }
        }
    }

    ring_game_push_state_to_ring();
}

/*
 * Eingang: -
 * Verarbeitung: Löscht den gespeicherten LED-Zustand und überträgt ihn auf den Ring
 * Ausgang: Ring wird ausgeschaltet
 */
void ring_game_clear(void)
{
    ring_game_clear_state();
    ring_game_push_state_to_ring();
}

/*
 * Eingang: -
 * Verarbeitung: Rotiert den aktuellen LED-Zustand mehrfach nach links mit Verzögerung
 * Ausgang: Gewinnanimation für Spieler X wird angezeigt
 */
void ring_game_animate_x_win(void)
{
    for (uint8_t step = 0; step < 24; step++)
    {
        ring_game_rotate_left_once();
        ring_game_push_state_to_ring();
        HAL_Delay(120);
    }
}

/*
 * Eingang: -
 * Verarbeitung: Rotiert den aktuellen LED-Zustand mehrfach nach rechts mit Verzögerung
 * Ausgang: Gewinnanimation für Spieler O wird angezeigt
 */
void ring_game_animate_o_win(void)
{
    for (uint8_t step = 0; step < 24; step++)
    {
        ring_game_rotate_right_once();
        ring_game_push_state_to_ring();
        HAL_Delay(120);
    }
}
