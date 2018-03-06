/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_it.h"
#include "gpio_led.h"
#include "spi_drv.h"
#include "usart2_comm.h"
#include "usart6_dma_comm.h"
#include "dma2_driver.h"
#include "spi_max7219.h"

#define BUF_SIZE       384
char gps_string[BUF_SIZE] = { 0 };
volatile unsigned long msTicks = 0;
volatile uint8_t DMA2_TxCplt_Occured = FALSE;

void Delay_ms(unsigned long del_count)
{
	del_count *= 1000;
	msTicks = del_count;
	while (msTicks != 0)
		;
}

void SysTick_Init(void)
{
	/****************************************
	 *SystemFrequency/1000      1ms         *
	 *SystemFrequency/100000    10us        *
	 *SystemFrequency/1000000   1us         *
	 *****************************************/

	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	while (SysTick_Config(SystemCoreClock / 1000000) != 0)
	{
	}

}

int main(void)
{

	SystemInit();
	SysTick_Init();
	RCC_HSICmd(ENABLE);

	init_led_gpios();

	/*Initialize SPI2 module*/
	init_spi2_gpio();
	init_spi2_module();

	/*Initialize USART2 module*/
	init_usart2_comm_module();
	init_usart2_gpio();

	/*Initialize DMA2 module*/
	init_dma2();
	enable_dma2_irq();

	/*Initialize USART6 (GPS) module*/
	init_usart6_gpio();
	init_usart6_comm_module();

	init_MAX7219_powerup();
	GPIO_SetBits(GPIOD, GPIO_Pin_12);

	USART_TX_string("Reset!\n\r");

	while (1)
	{

		if (DMA2_TxCplt_Occured == TRUE)
		{

			/* Copy the DMA Buffer into local buffer*/
			memcpy(gps_string, DMA_RX_Buffer, DMA_RX_BUFFER_SIZE);

			/*Initialize DMA again*/
			init_dma2();

			/*Display the parsed string over 7Segment LED*/
			Display_Time_MAX7219(gps_string, "$GPRMC", 7);
			DMA2_TxCplt_Occured = FALSE;
		}
	}

}

void DMA2_Stream2_IRQHandler(void)
{

	if (DMA_GetITStatus(DMA2_USART6_STREAM, DMA_IT_TCIF2) != RESET)
	{
		DMA2_TxCplt_Occured = TRUE;

		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);

		DMA_ClearITPendingBit(DMA2_USART6_STREAM, DMA_IT_TCIF2);
		DMA_ClearITPendingBit(DMA2_USART6_STREAM, DMA_IT_HTIF2);
		DMA_ClearITPendingBit(DMA2_USART6_STREAM, DMA_IT_FEIF2);
		DMA_ClearITPendingBit(DMA2_USART6_STREAM, DMA_IT_DMEIF2);
		DMA_ClearITPendingBit(DMA2_USART6_STREAM, DMA_IT_TEIF2);

	}
}

void SysTick_Handler(void)
{ /* SysTick interrupt Handler. */

	msTicks--;
}

