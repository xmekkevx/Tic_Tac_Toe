#include <Benutzerschnittstelle/input.h>
#include <Hardwaretreiber/keyboard.h>
#include "main.h"
#include "usart.h"

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
        case 11: return INPUT_KEY_YES;
        case 12: return INPUT_KEY_NO;
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

        case 'y':
        case 'Y': return INPUT_KEY_YES;

        case 'n':
        case 'N': return INPUT_KEY_NO;

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
    {
        return INPUT_KEY_NONE;
    }

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

void input_init(void)
{
    /* aktuell nichts noetig */
}

input_key_t input_read_key_nonblocking(void)
{
    input_key_t key;

    key = input_get_spi_key_nonblocking();
    if (key != INPUT_KEY_NONE)
    {
        return key;
    }

    key = input_get_uart_key_nonblocking();
    if (key != INPUT_KEY_NONE)
    {
        return key;
    }

    return INPUT_KEY_NONE;
}

input_key_t input_read_key_blocking(void)
{
    input_key_t key;

    while (1)
    {
        key = input_read_key_nonblocking();
        if (key != INPUT_KEY_NONE)
        {
            return key;
        }

        HAL_Delay(1);
    }
}
