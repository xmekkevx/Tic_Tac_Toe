#include "retarget_stdio.h"
#include <stdio.h>
#include <errno.h>

UART_HandleTypeDef * gHuart=NULL;

// RetargetInit nach der UART_Init aufrufen
void RetargetInit(UART_HandleTypeDef *UARTx)
{
	gHuart=UARTx;                      // Uart-Handler der umgeleitet werden nsoll
	setvbuf(stdout, NULL, _IONBF, 0);  // i/O-zwischenspeicher fuer STDOUT ausschalten
}

//Umleitung STDOUT-Stream auf UART
int _write(int fd, char* ptr, int len)
{
	HAL_StatusTypeDef hstatus;

	hstatus = HAL_UART_Transmit(gHuart, (uint8_t *) ptr, len, HAL_MAX_DELAY);
	if (hstatus == HAL_OK)
		return len;
	else
		return EIO;
}

//Umleitung STDIN-Stream auf UART
int _read(int fd, char* ptr, int len) {
	HAL_StatusTypeDef hstatus;

	__HAL_UART_CLEAR_OREFLAG(gHuart);                                      // Overflow Flag loeschen
	hstatus = HAL_UART_Receive(gHuart, (uint8_t *) ptr, 1, HAL_MAX_DELAY); // lese ein Zeichen
	if (hstatus == HAL_OK)
	{
		HAL_UART_Transmit(gHuart, (uint8_t *) ptr, 1, HAL_MAX_DELAY);      // Echo zum Terminal senden
		return 1;
	}
	else
		return EIO;
}

