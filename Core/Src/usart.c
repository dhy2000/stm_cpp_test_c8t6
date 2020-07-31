#include "usart.h"
#include <string.h>
#include <stdio.h>

UART_RxBuffer USART1_Buffer;

void init_USART_RxBuffer(UART_RxBuffer *rxbuff) {
	rxbuff->pRx_Data = rxbuff->uRx_Data;
}

void call_on_USART_IRQHandler(UART_HandleTypeDef *huart, UART_RxBuffer *rxbuff) {
	HAL_UART_Receive(huart, rxbuff->pRx_Data, 1, RECV_TIMEOUT);

	if (*(rxbuff->pRx_Data) == RECV_EOF) {
		*(++rxbuff->pRx_Data) = 0;
		USART_Rx_Callback(huart, rxbuff);
		rxbuff->pRx_Data = rxbuff->uRx_Data;
		rxbuff->uLength = 0;
	}
	else {
		rxbuff->pRx_Data++;
		rxbuff->uLength++;
	}
}

/*__weak void USART_Rx_Callback(UART_HandleTypeDef *huart, UART_RxBuffer *rxbuff) {
	if (huart == (&huart1)) {
		// Deal With Rx 1
		uart_fputs(huart, "Received : ");
		uart_fputs(huart, (char *)rxbuff->uRx_Data);
	}
	else {
		// Not Rx
	}
}*/
__weak void USART_Rx_Callback(UART_HandleTypeDef *huart, UART_RxBuffer *rxbuff) {
	if (huart == (&huart1)) {
		// Deal With Rx 1
	}
	else {
		// Not Rx
	}
}

// stdio in uart
unsigned int uart_fputs(UART_HandleTypeDef *huart, const char *msg) {
	return HAL_UART_Transmit(huart, (uint8_t *)msg, strlen(msg), 10);
}
