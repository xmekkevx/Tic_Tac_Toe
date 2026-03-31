#include <Hardwaretreiber/neopixel.h>
#include "tim.h"

/*
 * Begründung für globale Variablen:
 *
 * Die Arrays werden global genutzt, da mehrere Funktionen auf denselben LED-Zustand
 * und denselben DMA-Puffer zugreifen müssen.
 *
 * led_colors speichert die aktuellen Farben aller LEDs und wird von mehreren
 * Funktionen gelesen und verändert.
 *
 * ccr_buffer wird für die PWM-/DMA-Ausgabe benötigt und muss während der
 * Übertragung zentral verfügbar bleiben.
 */
static uint8_t led_colors[NEOPIXEL_LED_COUNT][3];
static volatile uint32_t ccr_buffer[NEOPIXEL_BUFFER_SIZE];

/*
 * Eingang: -
 * Verarbeitung: Wandelt die gespeicherten LED-Farben in PWM-Werte für den DMA-Puffer um
 * Ausgang: gefüllter DMA-Puffer für die Neopixel-Ausgabe
 */
static void neopixel_build_buffer(void)
{
    uint32_t buffer_index = 0;

    for (uint8_t led = 0; led < NEOPIXEL_LED_COUNT; led++)
    {
        uint32_t led_data =
            ((uint32_t)led_colors[led][1] << 16) |   // G
            ((uint32_t)led_colors[led][0] << 8)  |   // R
            ((uint32_t)led_colors[led][2]);          // B

        for (int bit = 23; bit >= 0; bit--)
        {
            if (led_data & (1UL << bit))
            {
                ccr_buffer[buffer_index++] = 20;   // logisch 1
            }
            else
            {
                ccr_buffer[buffer_index++] = 10;   // logisch 0
            }
        }
    }

    for (uint32_t i = 0; i < NEOPIXEL_RESET_SLOTS; i++)
    {
        ccr_buffer[buffer_index++] = 0;
    }
}

/*
 * Eingang: -
 * Verarbeitung: Löscht alle LED-Farben und überträgt den Zustand an den LED-Streifen
 * Ausgang: alle LEDs sind aus
 */
void neopixel_init(void)
{
    neopixel_clear();
    neopixel_show();
}

/*
 * Eingang: -
 * Verarbeitung: Setzt alle gespeicherten LED-Farben auf 0
 * Ausgang: interner LED-Zustand ist gelöscht
 */
void neopixel_clear(void)
{
    for (uint8_t i = 0; i < NEOPIXEL_LED_COUNT; i++)
    {
        led_colors[i][0] = 0; // R
        led_colors[i][1] = 0; // G
        led_colors[i][2] = 0; // B
    }
}

/*
 * Eingang: index, red, green, blue
 * Verarbeitung: Speichert die Farbwerte für die angegebene LED, falls der Index gültig ist
 * Ausgang: Farbe der gewählten LED ist intern gesetzt
 */
void neopixel_set_led(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
{
    if (index >= NEOPIXEL_LED_COUNT)
    {
        return;
    }

    led_colors[index][0] = red;
    led_colors[index][1] = green;
    led_colors[index][2] = blue;
}

/*
 * Eingang: red, green, blue
 * Verarbeitung: Setzt für alle LEDs die gleichen Farbwerte
 * Ausgang: alle LEDs haben intern dieselbe Farbe
 */
void neopixel_fill(uint8_t red, uint8_t green, uint8_t blue)
{
    for (uint8_t i = 0; i < NEOPIXEL_LED_COUNT; i++)
    {
        neopixel_set_led(i, red, green, blue);
    }
}

/*
 * Eingang: -
 * Verarbeitung: Erstellt aus den LED-Daten den DMA-Puffer und startet die PWM-Ausgabe
 * Ausgang: aktueller LED-Zustand wird an den Neopixel-Streifen übertragen
 */
void neopixel_show(void)
{
    neopixel_build_buffer();
    HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)ccr_buffer, NEOPIXEL_BUFFER_SIZE);
}

/*
 * Eingang: htim
 * Verarbeitung: Prüft den Timer und stoppt nach abgeschlossener DMA-Ausgabe die PWM-Übertragung
 * Ausgang: DMA-Übertragung für TIM2 wird beendet
 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
    }
}
