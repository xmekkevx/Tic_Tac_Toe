
#ifndef INC_RETARGET_STDIO_H_
#define INC_RETARGET_STDIO_H_

#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "usart.h"

void RetargetInit(UART_HandleTypeDef *UARTx);


#endif /* INC_RETARGET_STDIO_H_ */
