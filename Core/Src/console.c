/*
 * console.c
 *
 *  Created on: Nov 3, 2021
 *      Author: carlk
 */

#include "main.h"
//
#include <string.h>
#include <stdbool.h>
//
#include "console.h"
//
#include "printf.h"
#include "stm32f7xx_ll_usart.h"


int __io_putchar(int ch)
{
    uint8_t c = (uint8_t)ch;

    if (c == '\n')
    {
        uint8_t cr = '\r';
        HAL_UART_Transmit(&huart3, &cr, 1, HAL_MAX_DELAY);
    }

    HAL_UART_Transmit(&huart3, &c, 1, HAL_MAX_DELAY);

    return ch;
}

//int __io_putchar(int ch) {
//	// Wait until LPUART Transmit Data Register Empty Flag is set
//	while (!__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE)){}
//	if ('\n' == ch){
//		huart3.Instance->TDR = ( '\r' & 0x00FF);
//	}
//
//	while (!__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE)){}
//	// Wait until TXE flag is set
//	//while (!__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE));
//
//	// Write directly to the register instance via handle
//	huart3.Instance->TDR = (ch & 0x00FF);
//
//	return ch;
//}

// For Marco Paland's printf
void _putchar(char character) {
	if ('\n' == character)
		__io_putchar('\r');
	__io_putchar(character);
}

volatile atomic_bool cnsl_buf_full;
volatile char cnsl_buf[16];
static size_t ix;

/**
 * @brief  Function called from LPUART IRQ Handler when RXNE flag is set
 *         Function is in charge of reading character received on USART RX line.
 * @param  None
 * @retval None
 */
//void LPUART_CharReception_Callback() {
//
//	while (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE)) {
//		uint8_t received_char = 0;
//
//		/* Read Received character. RXNE flag is cleared by reading of RDR register */
//		HAL_UART_Receive(&huart3, &received_char, 1, 10);
//
//		/* Echo received character on TX */
//		if (received_char) {
//			__io_putchar(received_char);
//			if (received_char == '\r') {
//				cnsl_buf[ix] = '\0';
//				cnsl_buf_full = true;
//			} else {
//				/* Queue the character */
//				if (ix < sizeof cnsl_buf - 1)
//					cnsl_buf[ix++] = received_char;
//			}
//		}
//	}
//}




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        if (rx_char)
        {
            __io_putchar(rx_char);

            if (rx_char == '\r')
            {
                cnsl_buf[ix] = '\0';
                cnsl_buf_full = true;
            }
            else if (ix < sizeof(cnsl_buf) - 1)
            {
                cnsl_buf[ix++] = rx_char;
            }
        }

        /* Restart reception */
        HAL_UART_Receive_IT(&huart3, &rx_char, 1);
    }
}

void cnsl_buf_clear() {
	ix = 0;
	cnsl_buf[ix] = '\0';
	cnsl_buf_full = false;
}
