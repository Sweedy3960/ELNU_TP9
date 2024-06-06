/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "17400.h"
#include "stm32driverlcd.h"
#include "stm32gestSpiLIS3DH.h"
#include "stm32delays.h"
#include "stm32driverlcd.h"
#include "time.h"
#include "stm32f0xx_it.h"
#include <stdlib.h>
#include "math.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum { MODE_STD, MODE_CAL} MODES;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
e_States state = INIT;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
bool flag10Ms;//=false;
static bool InitialisationHasOccured = false;
// ----------------------------------------------------------------
	// *** A COMPLETER ! ***
	// Mettez vos fonctions ici..

// A utility function to reverse a string	
void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
		//https://www.geeksforgeeks.org/implement-itoa/
}

char* citoa(int16_t num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
    //https://www.geeksforgeeks.org/implement-itoa/
    return str;
}	
bool LectureDuFlag10ms(void)
//cette fonction v�rifie si le flag de 10ms est actif pour calculer les timmings demander 
//de plus la lecture des entr� est faite lorsque que le flag est actif ce qui permet d'avoir 
//une base de temps pour les echantillons du buffer d'entr�e au d�triment du temps de r�action
{
	
	static uint16_t cntTime = 0;
	//test du flag 
	if(flag10Ms)
	{
	
		
		flag10Ms=false; 
		cntTime++;
		if(cntTime < _3SEC)
		{
		   GPIOC -> ODR &= ~LEDS;
		}
		else
		{
		   InitialisationHasOccured=true;
			 GPIOC -> ODR |= LEDS;
			 cntTime = _3SEC;
			 state = EXEC;
		}
		return 1;	
	}
	return 0;
}
void initialisation(void)
{
	if (!(InitialisationHasOccured))
	{
		lcd_gotoxy(1,1);
		printf_lcd("TP Niveau El. <2024> ");
		lcd_gotoxy(1,2);
		printf_lcd("VCO ACL");
		lcd_bl_on();
	}
}
int calculAngle(int16_t* pValAcc)
{
	float anle_degree =(atan2(pValAcc[0],pValAcc[2]))*180/PI;
	return anle_degree;

}
void affichageligne1(int16_t *angle)
{
	lcd_gotoxy(1,1);
	printf_lcd("       %3d%1c",*angle,0xDF);
	
}
void Registreadecalage()
{
	
	char tx_Buffer2[2]=  {255,255};

	GPIOB->ODR &= ~RXDCLK_Pin;
	HAL_SPI_Transmit(&hspi2,(uint8_t*)tx_Buffer2,1,100);
	GPIOB->ODR |= RXDCLK_Pin;
	GPIOB->ODR &= ~RXDCLK_Pin;

	tx_Buffer2[0] = 0;
	HAL_SPI_Transmit(&hspi2,(uint8_t*)tx_Buffer2,1,100);
	GPIOB->ODR |= RXDCLK_Pin;
}
void affichageLCD(int16_t *angle)
{
	lcd_clearScreen();
	affichageligne1(&(*angle));
	Registreadecalage();
}



void affichageBulle(int16_t *angle)
{
	static char ligne[19] = "----+----=----+----";
    int16_t index;
    if (*angle < 0)
    {
        index = 9 - abs(*angle); // Pour les angles négatifs, la bulle est plus à gauche
    }
    else
    {
        index = 9 + *angle; // Pour les angles positifs, la bulle est plus à droite
    }
 
    // Vérifier les limites
    if (index < 0)
    {
      index = 0;
    }
    if (index > 18)
    {
      index = 18;
    }
   
    // Réinitialiser la ligne
    strcpy(ligne, "----+----=----+----");
   
    // Placer la bulle
    ligne[index] = 'O';
 
    // Afficher la ligne
        lcd_gotoxy(1,2);
    printf_lcd("%s\n", ligne);
	
}
void transmitpc(int16_t *angle)
{
	char tb_affichage[SIZE] ={0};
	char tb_end[]={'d',0x0D,'\0'};
	char nbDigit;
	char retval=0;
	
	nbDigit=(abs(*angle) <10)? 1:(abs(*angle) <100)?2:3;
	citoa(abs(*angle), &tb_affichage[1], 10);
	strcat(&tb_affichage[nbDigit],tb_end);
	if((uint16_t)*angle < MAXNMAX)
	{
		tb_affichage[0]='+';
	}
	else
	{
		tb_affichage[0]='-';
	}
	
	for(int i=0; i <= (nbDigit+4);i++ )
	{
	  retval=HAL_UART_Transmit(&huart1,(uint8_t*) tb_affichage,(nbDigit+3),100);
	}
	
}
void execution (int16_t* pValAcc)
{
	
	int16_t angle=0;
	LIS3DH_ReadAcc(pValAcc);
	angle=calculAngle(pValAcc);
	affichageLCD(&angle);
	affichageBulle(&angle);
	transmitpc(&angle);
}
// ----------------------------------------------------------------

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	//variables
	// *** A COMPLETER ! ***
	
	
	//init	
	// *** A COMPLETER ! ***
	

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
	LIS3DH_Init();
	HAL_TIM_Base_Start_IT(&htim6);
	lcd_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	int16_t pValAcc[NBAXES];
	
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/* TEST 
		if(LectureDuFlag10ms())
		{
			
			//LIS3DH_Read(0x0F,&value);
			
		}
		*//*
		GPIOB->ODR ^= RXDCLK_Pin;
		
		GPIOB->ODR ^= RXDCLK_Pin;*/
		// *** A COMPLETER ! ***		
		switch(state)
		{
			case INIT:
				state = IDLE;
				initialisation();
				break;
			case EXEC:
				
				execution(pValAcc);
				state = IDLE;
				break;
			case IDLE:
				break;
			default: 
				break;
		}
		LectureDuFlag10ms();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
