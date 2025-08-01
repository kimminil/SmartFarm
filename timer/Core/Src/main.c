/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// My Lib
#define F446RE 1
#include <My_ARM_GPIO_Lib_V3_8.h>

volatile uint8_t rxd_flag1 = 0;
volatile uint8_t rxd_flag2 = 0;
volatile uint8_t rx_buf1[20];
volatile uint8_t rx_buf2[20];

#define My_Uart_LIB_EN   1
#define NO_RTOS          2
//#define RTOS           2
#define Console_Ch2      3
#define USART_Ch_2_EN    4
//#define USART_Ch_1_EN    4
#define IRQ_EN_Uart_1    5
#include "My_ARM_UART_Lib_V4_8.h"

uint8_t ad_end_flag1 = 0;
uint16_t test_loop1 = 0;
uint16_t ad_buf1[20];
uint8_t ad_ch_nb1 = 6;

#define ARM_Mode  1
#define UART      2
extern int out_dev_set_mode;
#include "My_MCU_Printf_Lib_V2_6.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
struct _CNT_
{
  uint16_t cnt;
  uint16_t t_loop;
  uint16_t cnt_loop;
  uint8_t  cnt_flag;
  uint8_t  sec;
  uint8_t  min;
  uint16_t dn_cnt;
}cnt = {1234, 0, 0, 0, 15, 25, 1250};
struct _CNT_ *ct = &cnt;

struct _cnt_bit_
{
  uint8_t  time_flag: 1;
  uint8_t  run_flag : 3;
  uint16_t tx_flag  : 4;
  uint8_t  tg_flag  : 2;
  uint8_t  t2_flag  : 1;
  uint8_t  t3_flag  : 1;
  uint8_t  t11_flag : 1;
  uint8_t  t12_flag : 1;
}flag = {0, 2, 10, 2, 0,0,0,0};
struct _cnt_bit_ *fg = &flag;

char d_buf[50];
uint16_t ad_buf[20];

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 콜백함수
//timer CallBack Func
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim -> Instance == TIM7)
  {
	// 1sec
    fg -> time_flag = (fg -> time_flag == 0) ? 1 : 0;
  }
 //--------------------------------------------------
  if(htim -> Instance == TIM3)
     {
     	// 500ms = OC
     	fg -> t3_flag = (fg -> t3_flag == 0) ? 1 : 0;
     }
  //---------------------------------------

  if(htim -> Instance == TIM11)
  {
	// 700ms
	Out_Bit_Tg(led1_GPIO_Port, led1_Pin);
	fg -> t11_flag = (fg -> t11_flag == 0) ? 1 : 0;
  }
 //-----------------------------------------------------

  if(htim -> Instance == TIM12)
   {
  	// 1.5sec
  	Out_Bit_Tg(led2_GPIO_Port, led2_Pin);
   	//fg -> t12_flag = (fg -> t12_flag == 0) ? 1 : 0;
  	fg -> t12_flag = !fg -> t12_flag;
   }
   //-----------------------------------------------------

  if(htim -> Instance == TIM2)
   {
 	// 100ms = OC
 	fg -> t2_flag = (fg -> t2_flag == 0) ? 1 : 0;
   }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM7_Init();
  MX_TIM12_Init();
  MX_USART2_UART_Init();
  MX_TIM11_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim7); // 1sec
  // 폴링방식으로 해도 됨
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1); // 100ms
  HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_2); // 500ms
  HAL_TIM_Base_Start_IT(&htim11);  // 700ms
  HAL_TIM_Base_Start_IT(&htim12);  // 1.5sec
 // HAL_TIM_Base_Start_IT(&htim1);  // 1.5sec

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	if(fg -> t2_flag ==1)
//	{
//	  printf("time2 IRQ\r\n");
//	  //tx_str("time2 IRQ\r\n", 2);
//	  fg -> t2_flag =0;
//	}
//
//	if(fg -> t3_flag ==1)
//	{
//	  printf("time3 IRQ\r\n");
//	  //tx_str("time3 IRQ\r\n", 2);
//	  fg -> t3_flag = 0;
//	}

	if(fg -> t11_flag ==1)
	{
	  printf("time11 IRQ\r\n");
	  fg -> t11_flag =0;
	}

	if(fg -> t12_flag ==1)
	{
	  printf("time12 IRQ\r\n");
	  fg -> t12_flag  =0;
	}

	if(fg -> time_flag) // 1sec chk sum
	{
	  fg -> time_flag = 0;

	  ct -> sec++;
	  if(ct -> sec > 59) // 1min chk sum
	  {
		(*ct).sec = 0;
		cnt.min++;
		if(ct -> min > 59) // 1 hour chk sum
		{
		  (*ct).min = 0;
		  // hour++
		}
	  }

	 printf("Now Time = %d : %d\r\n", ct -> min, ct -> sec);
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* TIM8_BRK_TIM12_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
  /* TIM7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM7_IRQn);
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
