/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
typedef enum {INIT,EXEC,IDLE}e_States;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCDRW_Pin GPIO_PIN_13
#define LCDRW_GPIO_Port GPIOC
#define LCDRS_Pin GPIO_PIN_14
#define LCDRS_GPIO_Port GPIOC
#define LCDBL_Pin GPIO_PIN_15
#define LCDBL_GPIO_Port GPIOC
#define OSCIN_Pin GPIO_PIN_0
#define OSCIN_GPIO_Port GPIOF
#define OSCOUT_Pin GPIO_PIN_1
#define OSCOUT_GPIO_Port GPIOF
#define BTN0_Pin GPIO_PIN_0
#define BTN0_GPIO_Port GPIOC
#define BTN1_Pin GPIO_PIN_1
#define BTN1_GPIO_Port GPIOC
#define BTN2_Pin GPIO_PIN_2
#define BTN2_GPIO_Port GPIOC
#define BTN3_Pin GPIO_PIN_3
#define BTN3_GPIO_Port GPIOC
#define AI0_Pin GPIO_PIN_0
#define AI0_GPIO_Port GPIOA
#define SPISCK_Pin GPIO_PIN_5
#define SPISCK_GPIO_Port GPIOA
#define SPIMISO_Pin GPIO_PIN_6
#define SPIMISO_GPIO_Port GPIOA
#define SPIMOSI_Pin GPIO_PIN_7
#define SPIMOSI_GPIO_Port GPIOA
#define LED0_Pin GPIO_PIN_4
#define LED0_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOC
#define CS_Pin GPIO_PIN_12
#define CS_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_6
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_7
#define LED3_GPIO_Port GPIOC
#define LCDDB4_Pin GPIO_PIN_8
#define LCDDB4_GPIO_Port GPIOC
#define LCDDB5_Pin GPIO_PIN_9
#define LCDDB5_GPIO_Port GPIOC
#define MFP_Pin GPIO_PIN_8
#define MFP_GPIO_Port GPIOA
#define USART1TX_Pin GPIO_PIN_9
#define USART1TX_GPIO_Port GPIOA
#define USART1RX_Pin GPIO_PIN_10
#define USART1RX_GPIO_Port GPIOA
#define USART1CTS_Pin GPIO_PIN_11
#define USART1CTS_GPIO_Port GPIOA
#define USART1RTS_Pin GPIO_PIN_12
#define USART1RTS_GPIO_Port GPIOA
#define nSPICSLM70_Pin GPIO_PIN_15
#define nSPICSLM70_GPIO_Port GPIOA
#define LCDDB6_Pin GPIO_PIN_10
#define LCDDB6_GPIO_Port GPIOC
#define LCDDB7_Pin GPIO_PIN_11
#define LCDDB7_GPIO_Port GPIOC
#define LCDE_Pin GPIO_PIN_12
#define LCDE_GPIO_Port GPIOC
#define RXDCLK_Pin GPIO_PIN_7
#define RXDCLK_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define XVALUE 0
#define YVALUE 1
#define ZVALUE 2
#define NBAXES 3
#define PI 3.141

#define AGLE3 0x3F
#define AGLE2 0x5F
#define AGLE1 0x6F
#define AGLE0 0x77
#define AGLE_1 0x7B
#define AGLE_2 0x7D
#define AGLE_3 0x7E


#define MAXNMAX 0x7FFF

#define MAXNEGVALUE 0xFF4C
#define MAXPOSITIFVALUE 0x00B4
//Defines pour timings 
#define _3SEC 300
#define _500MSEC 500
#define _250MSEC 25
#define _25MSEC 25


#define ALLS 0x0F
#define S2 0x01
#define S3 0x02
#define S4 0x04
#define S5 0x08

#define LEDS 0x00F0
#define LED0 0x0010
#define LED1 0x0020
#define LED2 0x0040
#define LED3 0x0080
#define SIZE 7
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
