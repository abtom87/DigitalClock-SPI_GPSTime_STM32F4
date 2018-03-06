/*
 * usart1_dma_comm.h
 *
 *  Created on: Feb 2, 2018
 *      Author: abel
 */

#ifndef USART6_DMA_COMM_H_
#define USART6_DMA_COMM_H_

#define USART6_GPIO GPIOC


#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "stm32f4xx_gpio.h"

#include <string.h>
#define TRUE 1
#define FALSE 0

void init_usart6_comm_module(void);
void init_usart6_gpio(void);
void enable_usart6_rx_irq(void);
void USART6_TX_byte(uint8_t);
void USART6_TX_string(char *);








#endif /* USART6_DMA_COMM_H_ */
