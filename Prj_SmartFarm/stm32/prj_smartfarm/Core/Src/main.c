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
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define printf_en 1
#define USART_Ch_1_EN	3
#define Console_Ch2   2
#define dht11_en 3
#define Ch1	1
#define cds_en	2
#define co2_en	3
#define w_height_en	4
#define servo_en 5
#define water_pump_en 	6
#define fan_en 6
#define light_en 6
#define pelt_en 6
#include "my_library.h"

#define Out_Bit(port, pin, out_data_bits) HAL_GPIO_WritePin(port, pin, out_data_bits)

#include "MCP2515.h"
#include "CANSPI.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
char Test_Msg[16];
char pass_sig[] = "Pass\r\n";
char msg_to_Rpi[64];
char Cmd_Msg[16];
extern int Cmd_values[5];
uint8_t Sensor_flag = 0;
uint8_t act_flag =0;

//struct Flags{
//	uint8_t start_flag;
//	uint8_t sensor_flag;
//}flags = {0,0};
//struct Flags *flag = &flags;

struct Sensor_Value{
	  float TEMP;		// 온도
	  float RH;			// 습도
	  float lux ;				// 밝기
	  float co2 ;				// c02 ppm
	  float w_1;		// 둘 중 하나
}sen_val = {0.0, 0.0, 0.0, 0.0, 0.0};
struct Sensor_Value *sen_v = &sen_val;

#include "main.h"
#include "stdio.h"

extern ADC_HandleTypeDef hadc1;

void set_motor_speed(uint8_t duty_percent)
{
  if(duty_percent > 100) duty_percent = 100;

  uint32_t pwm_period = __HAL_TIM_GET_AUTORELOAD(&htim1); // Period 값 읽기
  uint32_t compare_value = (pwm_period + 1) * duty_percent / 100;

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, compare_value);
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim6);
  HAL_TIM_Base_Start(&htim1);
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);


  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  HAL_TIM_Base_Start_IT(&htim2);

//  printf("Warming up RX-9 Sensor    \r\n");
//  HAL_Delay(180000);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  Peltier_Cooling();
	  HAL_Delay(60000);
	  Peltier_Heating();
	  HAL_Delay(60000);

//
//	  //receive start data	= STM32 Test Mode\n
//
//	  if( Sensor_flag == 0)
//	  {
//		  printf("Waiting Rpi Test Mode Msg\r\n");
//		  if(HAL_UART_Receive(&huart1, (uint8_t *)Test_Msg, 16, 1000)== HAL_OK)
//		  {
//			  Test_Msg[15] = '\0'; // 강제로 문자열 종료
//			  if (strcmp(Test_Msg, "STM32 Test Mode") == 0)
//			  {
//				  if(DHT11_Sensor_Test()&&check_light())
//				  {
//					  Sensor_flag = 1;
//					  printf("Sensor Check Complete\r\n");
//					  HAL_UART_Transmit(&huart1, (uint8_t*)pass_sig, strlen(pass_sig), 10);
//
//				  }
//			  }
//		  }
//	  }
//	  else
//	  {
//		  //물 순환 시작
//		  //인터럽트로 1분마다 읽게 만들거
//		  float TEMP = DHT11_Run_TEMP();		// 온도
//		  float RH = DHT11_Run_RH();			// 습도
//		  float lux = read_light();				// 밝기
//		  float co2 = read_co2();				// c02 ppm
//	  	  float w_1 = isWaterDetected1();		// 둘 중 하나 -> 타이머 인터럽트
//
//	  	  act_flag = 0;
//
//		  // 보내야할 값 4개
//		  // 둘 중 하나 사용
//		  printf("TEMP : %.1f RH : %.1f co2 : %.1f lux : %.1f\r\n", TEMP, RH, co2, lux);
//
//		  //tim 인터럽트로 인해 flag ==1로 만들어서 값 보내기
//		  if(TEMP){
//			  // co2,w_height level 반환 대신 그냥 adc값 반환하도록 변경해야됨
//			  // 둘 중 하나 사용
//			  sprintf(msg_to_Rpi, "%.1f,%.1f,%.1f,%.1f\r\n", TEMP, RH,co2,lux);
//			  HAL_UART_Transmit(&huart1, (char *)msg_to_Rpi, strlen(msg_to_Rpi), 10);
//		  }
//		  	  // 제어 신호 받으면?????
//		  if(HAL_UART_Receive(&huart1, (uint8_t *)Cmd_Msg, 8, 1000)== HAL_OK)
//		  {
//			  //제어 명령어 -  팬+팰티어, led. 팬, 모터 센서(급수)
//			  printf("Received data\r\n");
//			  printf("Cmd_Msg = %s",Cmd_Msg);
//			  Cmd_Msg[9] = '\0'; // 강제로 문자열 종료
//			  parse_fixed_csv(Cmd_Msg);
//			  for (int i = 0; i < 4; i++)
//			  {
//				  printf("Cmd_values[%d] = %d\r\n", i, Cmd_values[i]);  // 문제 잡기 물 센서 위치에 따라
//			  }
//	  	  	  process_commands(Cmd_values);
//
//		  }
//	  }
//	  HAL_Delay(5000);

  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
