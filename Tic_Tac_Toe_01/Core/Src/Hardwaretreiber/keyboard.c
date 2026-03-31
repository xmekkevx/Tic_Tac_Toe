#include <Hardwaretreiber/keyboard.h>
#include "main.h"
#include <stdio.h>

/*
 * Hier werden globale Variablen genutzt, da mehrere Funktionen auf die gleichen
 * Daten zugreifen müssen (z.B. SPI-Handle und letzter Tastendruck).
 *
 * Besonders last_key_pressed wird zwischen der zyklischen Abfrage und den
 * Lesefunktionen geteilt. Eine Übergabe über Parameter wäre hier umständlich,
 * da die Abfrage unabhängig und zeitgesteuert läuft.
 */
static SPI_HandleTypeDef *keyboard_hspi = NULL;
static uint8_t keyrate = 0xff;
static volatile uint8_t last_key_pressed = 0;

/*
 * Eingang: -
 * Verarbeitung: Gibt die zuletzt erkannte Taste zurück
 * Ausgang: letzter Tastencode oder 0
 */
static uint8_t keyboard_get_last_key(void)
{
    return last_key_pressed;
}

/*
 * Eingang: -
 * Verarbeitung: Setzt den zuletzt gespeicherten Tastencode zurück
 * Ausgang: kein gespeicherter Tastendruck mehr vorhanden
 */
static void keyboard_clear_last_key(void)
{
    last_key_pressed = 0;
}

/*
 * Eingang: hspi, ms_rate
 * Verarbeitung: Speichert SPI-Schnittstelle und Abtastrate für die Tastatur
 * Ausgang: Tastatur ist initialisiert
 */
void keyboard_init(SPI_HandleTypeDef *hspi, uint8_t ms_rate)
{
    keyboard_hspi = hspi;
    keyrate = ms_rate;
}

/*
 * Eingang: -
 * Verarbeitung: Liest zyklisch über SPI den Tastenzustand und erkennt neue Tastendrücke
 * Ausgang: neuer Tastendruck wird intern gespeichert
 */
void get_key_1ms(void)
{
    static uint8_t ms_counter = 0;
    static uint8_t key = 0;
    static uint8_t lastkey = 0;
    uint8_t i;
    uint16_t spi_val = 0;

    ms_counter++;
    if (ms_counter > keyrate)
    {
        ms_counter = 0;

        if (keyboard_hspi != NULL)
        {
            HAL_SPI_Receive(keyboard_hspi, (uint8_t*)&spi_val, 1, 50);

            key = 0;
            for (i = 0; i < 16; i++)
            {
                if ((~spi_val) & (1 << i))
                {
                    key = i + 1;
                    break;
                }
            }
        }

        if ((key != 0) && (lastkey == 0))
        {
            last_key_pressed = key;
        }

        lastkey = key;
    }
}

/*
 * Eingang: -
 * Verarbeitung: Ruft die Abfragefunktion auf und gibt eine erkannte Taste zurück
 * Ausgang: Tastencode oder 0 wenn keine Eingabe vorhanden
 */
uint8_t keyboard_get_key_nonblocking(void)
{
    get_key_1ms();

    if (keyboard_get_last_key() != 0)
    {
        uint8_t key = keyboard_get_last_key();
        keyboard_clear_last_key();
        return key;
    }

    return 0;
}

/*
 * Eingang: -
 * Verarbeitung: Wartet in Schleife bis eine Taste erkannt wird
 * Ausgang: erkannter Tastencode
 */
uint8_t keyboard_get_key_blocking(void)
{
    uint8_t key = 0;

    while (key == 0)
    {
        key = keyboard_get_key_nonblocking();
        HAL_Delay(1);
    }

    return key;
}
