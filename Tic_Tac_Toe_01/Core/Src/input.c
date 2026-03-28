#include "input.h"
#include "main.h"
#include "usart.h"
#include "keyboard.h"
#include "i2c_lcd.h"
#include <stdio.h>

void printHelp(void);
void clearTerminal(void);

static void input_draw_selection(uint8_t selected)
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

static input_key_t input_map_spi_key(uint8_t key)
{
    switch (key)
    {
        case 1:  return INPUT_KEY_1;
        case 2:  return INPUT_KEY_2;
        case 3:  return INPUT_KEY_3;
        case 4:  return INPUT_KEY_4;
        case 5:  return INPUT_KEY_5;
        case 6:  return INPUT_KEY_6;
        case 7:  return INPUT_KEY_7;
        case 8:  return INPUT_KEY_8;
        case 9:  return INPUT_KEY_9;
        case 13: return INPUT_KEY_BACKSPACE;
        case 14: return INPUT_KEY_HELP;
        case 16: return INPUT_KEY_ENTER;
        default: return INPUT_KEY_NONE;
    }
}

static input_key_t input_map_uart_char(char c)
{
    switch (c)
    {
        case '1': return INPUT_KEY_1;
        case '2': return INPUT_KEY_2;
        case '3': return INPUT_KEY_3;
        case '4': return INPUT_KEY_4;
        case '5': return INPUT_KEY_5;
        case '6': return INPUT_KEY_6;
        case '7': return INPUT_KEY_7;
        case '8': return INPUT_KEY_8;
        case '9': return INPUT_KEY_9;

        case '\b': return INPUT_KEY_BACKSPACE;
        case 127:  return INPUT_KEY_BACKSPACE;

        case '\r': return INPUT_KEY_ENTER;
        case '\n': return INPUT_KEY_ENTER;

        case 'h':
        case 'H': return INPUT_KEY_HELP;

        default:  return INPUT_KEY_NONE;
    }
}

static input_key_t input_get_spi_key_nonblocking(void)
{
    uint8_t key = keyboard_get_key_nonblocking();

    if (key == 0)
        return INPUT_KEY_NONE;

    return input_map_spi_key(key);
}

static input_key_t input_get_uart_key_nonblocking(void)
{
    char c;

    if (HAL_UART_Receive(&huart2, (uint8_t*)&c, 1, 0) == HAL_OK)
    {
        return input_map_uart_char(c);
    }

    return INPUT_KEY_NONE;
}

static input_key_t input_read_key_blocking(void)
{
    input_key_t key;

    while (1)
    {
        key = input_get_spi_key_nonblocking();
        if (key != INPUT_KEY_NONE)
            return key;

        key = input_get_uart_key_nonblocking();
        if (key != INPUT_KEY_NONE)
            return key;

        HAL_Delay(1);
    }
}

void input_init(void)
{
    /* aktuell nichts noetig */
}

uint8_t input_get_confirmed_move_1_to_9(void)
{
    uint8_t selected = 0;
    input_key_t key;

    input_draw_selection(selected);

    while (1)
    {
        key = input_read_key_blocking();

        if (key >= INPUT_KEY_1 && key <= INPUT_KEY_9)
        {
            uint8_t number = (uint8_t)key;

            if (selected == 0)
            {
                selected = number;
                input_draw_selection(selected);
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
            input_draw_selection(selected);
            printf("Korrektur: Auswahl geloescht\r\n");
        }
        else if (key == INPUT_KEY_HELP)
        {
            clearTerminal();
            printHelp();

            if (selected >= 1 && selected <= 9)
                printf("Aktuelle Auswahl: %d\r\n", selected);
            else
                printf("Aktuelle Auswahl: keine\r\n");

            input_draw_selection(selected);
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
                input_draw_selection(selected);
            }
        }
    }
}
