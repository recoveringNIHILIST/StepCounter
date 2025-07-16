/*
 * uart.h
 *
 *  Created on: Mar 14, 2025
 *      Author: NIHILIST
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "usart.h"
extern uint8_t transmitting_uart;

void uart_tx (uint8_t* buffer);
void uart_stop_tx (void);

#endif /* INC_UART_H_ */
