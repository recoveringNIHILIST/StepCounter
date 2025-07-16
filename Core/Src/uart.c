/*
 * uartr.c
 *
 *  Created on: Mar 14, 2025
 *      Author: NIHILIST
 */


#include "uart.h"
#include <stdint.h>
#include <string.h>

static uint32_t tx_timeout = 100;
uint8_t transmitting_uart = 1;
// transmit buffer over uart without blocking
void uart_tx (uint8_t* buffer)
{
	HAL_UART_Transmit(&huart2, buffer, strlen((char*)buffer), tx_timeout);
	transmitting_uart = 0;
}
// abort ongoing uart transmission and reset
void uart_stop_tx (void)
{
	HAL_UART_AbortTransmit(&huart2);
	transmitting_uart = 1;
}
