#include "ring_game.h"
#include "neopixel.h"

#define COLOR_X_R 255
#define COLOR_X_G 0
#define COLOR_X_B 0

#define COLOR_O_R 0
#define COLOR_O_G 255
#define COLOR_O_B 0

static char start_player = 'X';

void ring_game_init(void)
{
    start_player = 'X';
    neopixel_clear();
    neopixel_show();
}

void ring_game_show_start_player(char player)
{
    start_player = player;

    neopixel_clear();

    if (player == 'X')
    {
        neopixel_set_led(0, COLOR_X_R, COLOR_X_G, COLOR_X_B);
    }
    else
    {
        neopixel_set_led(0, COLOR_O_R, COLOR_O_G, COLOR_O_B);
    }

    neopixel_show();
}

void ring_game_update_turns(uint8_t turn_count)
{
    neopixel_clear();

    for (uint8_t i = 0; i < turn_count && i < NEOPIXEL_LED_COUNT; i++)
    {
        if ((i % 2) == 0)
        {
            if (start_player == 'X')
            {
                neopixel_set_led(i, COLOR_X_R, COLOR_X_G, COLOR_X_B);
            }
            else
            {
                neopixel_set_led(i, COLOR_O_R, COLOR_O_G, COLOR_O_B);
            }
        }
        else
        {
            if (start_player == 'X')
            {
                neopixel_set_led(i, COLOR_O_R, COLOR_O_G, COLOR_O_B);
            }
            else
            {
                neopixel_set_led(i, COLOR_X_R, COLOR_X_G, COLOR_X_B);
            }
        }
    }

    neopixel_show();
}

void ring_game_show_draw(void)
{
    neopixel_clear();
    neopixel_show();
}
