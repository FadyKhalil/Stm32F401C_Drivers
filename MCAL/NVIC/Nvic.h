/*
 * Nvic.h
 *
 *  Created on: Mar 28, 2022
 *      Author: fady
 */

#ifndef NVIC_H_
#define NVIC_H_

/*################################################################*/
/******************************Types******************************/

typedef struct
{



}Nvic_tstrIRQ;

/*###############################################################*/


#define NVIC_WINDOW_WATCHDOG_TIMER		0
#define	NIVC_EXTERNAL_LINE16			1
#define	NIVC_EXTERNAL_LINE21			2
#define	NIVC_EXTERNAL_LINE22			3
#define	NIVC_FLASH_INTERRUPT			4
#define	NVIC_RCC_INTERRUPT				5
#define NVIC_EXT0_INTERRRUPT			6
#define NVIC_EXT1_INTERRRUPT			7
#define NVIC_EXT2_INTERRRUPT			8
#define NVIC_EXT3_INTERRRUPT			9
#define NVIC_EXT4_INTERRRUPT			10
#define NVIC_DMA1_STREAM0				11
#define NVIC_DMA1_STREAM1				12
#define NVIC_DMA1_STREAM2				13
#define NVIC_DMA1_STREAM3				14
#define NVIC_DMA1_STREAM4				15
#define NVIC_DMA1_STREAM5				16
#define NVIC_DMA1_STREAM6               17
#define NVIC_ADC_INTERRUPT              18
#define NVIC_EXT9_INTERRUPT             23
#define NVIC_TIM1_BRK_INTERRUPT         24
#define NVIC_TIM1_UP_INTERRUPT          25
#define NVIC_TIMI1_TRG_INTERRUPT        26
#define NVIC_TIMI1_CC_INTERRUPT         27
#define NVIC_TIMI2_INTERRUPT            28
#define NVIC_TIMI3_INTERRUPT            29
#define NVIC_TIMI4_INTERRUPT            30
#define NVIC_I2C1_EV_INTERRUPT          31
#define NVIC_I2C1_ER_INTERRUPT          32
#define NVIC_I2C2_EV_INTERRUPT          33
#define NVIC_I2C2_ER_INTERRUPT          34
#define NVIC_SPI1_INTERRUPT             35
#define NVIC_SPI2_INTERRUPT             36
#define NVIC_USART1_INTERRUPT           37
#define NVIC_USART2_INTERRUPT           38
#define NVIC_EXTI15_INTERRUPT           40
#define NVIC_EXTI17_INTERRUPT           41
#define NVIC_EXTI18_INTERRUPT           42
#define NVIC_DMA1_INTERRUPT             47
#define NVIC_SDIO_INTERRUPT             49
#define NVIC_TIM5_INTERRUPT             50
#define NVIC_SPI3_INTERRUPT             51
#define NVIC_DMA2_STREAM0_INTERRUPT     56
#define NVIC_DMA2_STREAM1_INTERRUPT     57
#define NVIC_DMA2_STREAM2_INTERRUPT     58
#define NVIC_DMA2_STREAM3_INTERRUPT     59
#define NVIC_DMA2_STREAM4_INTERRUPT     60
#define NVIC_OTG_FS_INTERRUPT           67
#define NVIC_DMA2_STREAM5_INTERRUPT     68
#define NVIC_DMA2_STREAM6_INTERRUPT     69
#define NVIC_DMA2_STREAM7_INTERRUPT     70
#define NVIC_USART6_INTERRUPT           71
#define NVIC_I2C3_EV_INTERRUPT          72
#define NVIC_I2C3_ER_INTERRUPT          73
#define NVIC_FPU_INTERRUPT              81
#define NVIC_SPI4_INTERRUPT             84







void Nvic_vidEnableIrq(u8 Copy_strIrq);
void Nvic_vidDisableIrq(u8 Copy_strIrq);
void Nvic_vidSetPendingIrq(u8 Copy_strIrq);
void Nvic_vidClearPendingIrq(u8 Copy_strIrq);
void Nvic_vidGetPendingIrq(u8 Copy_strIrq, pu8 Add_pu8PendingStatus);
void Nvic_vidSetPriority(u8 Copy_strIrq, u8 Copy_u8Prioty);
void Nvic_vidGetPriority(u8 Copy_strIrq, pu8 Add_pu8Prioty);


#endif /* NVIC_H_ */
