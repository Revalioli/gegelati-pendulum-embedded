/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <cstdbool>
#include <vector>
#include <iostream>
#include <unistd.h>

#include "PendulumEnvironment.h"
extern "C" {
	#include "pendulum_program.h"
}

#include "TimingBench.h"
#include "INA219Bench.h"
#include "PendulumINA219Monitor.h"
#include "ina219.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// === Pendulum global access and parameters === */

PendulumEnvironment * pendulum_ptr;
uint16_t nbActions = 1000;	// Number of actions per inference
double initAngle = 0.0;
double initVelocity = 0.0;

// === Measurements and log ===

INA219_t ina219t;
const char logStart[] = "##### Log Start #####";
const char logEnd[] = "##### Log End #####";
int timingNbAttempts = 10;

// === TPG ===

uint32_t seed;  // Seed use to reset the PendulumEnvironment
extern "C" {
	extern double* in1;
}


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void energyBenchWrapper(void);
void timingExecutionBenchWrapper(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @return int
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
	MX_I2C1_Init();
	MX_TIM5_Init();
	MX_TIM7_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */

	/* == Pendulum environment === */
	std::vector<double> availableAction = {0.05, 0.1, 0.2, 0.4, 0.6, 0.8, 1.0};
	PendulumEnvironment pendulum(availableAction);
	in1 = pendulum.currentState;
	pendulum_ptr = &pendulum;

#ifdef TPG_SEED
  seed = TPG_SEED;
#else
  seed = HAL_GetTick();
 #endif

	/* === INA219 setup === */

	if(INA219_Init(&ina219t, &hi2c1, INA219_DEFAULT_ADDRESS, 3.2768) == 0){
		std::cout << "Can't access the INA219 sensor, end of program" << std::endl;
		while(1){}	// Waiting for reset
	}

	INA219_setConfig(&ina219t, INA219_CONFIG_BADCRES_12BIT | INA219_CONFIG_BVOLTAGERANGE_32V
								| INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS
								| INA219_CONFIG_SADCRES_12BIT);	// 0x219f

	/* === Benchmark === */

	// Timers are configure as follows :
	// - TIM5, used for timing benches, count each microseconds
	// - TIM7, used for energy monitoring count each 0.1 milliseconds, raised interrupt every 3 milliseconds

	PendulumINA219Monitor pendulumMonitor(&ina219t, pendulum, &htim7, TimeUnit::Milliseconds, 3.f);
	INA219Bench energybench(energyBenchWrapper, &pendulumMonitor);

	TimingBench executionTimingBench(timingExecutionBenchWrapper, &htim5, timingNbAttempts, TimeUnit::Microseconds);

  std::cout << "START" << std::endl;  // Synchronise with PC

  std::cout << "Seed STM32 : " << seed << std::endl;

  char buffStart;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

    read(STDIN_FILENO, &buffStart, sizeof(char));
		if(buffStart == '\n'){

				// Reset pendulum environment and store the initial conditions
			pendulum.reset(seed);
			initAngle = pendulum.getAngle();
			initVelocity = pendulum.getVelocity();

			std::cout << "Starting energy bench" << std::endl;

			std::cout << logStart << std::endl;
			energybench.startBench();
			std::cout << logEnd << std::endl;

			std::cout << "Exiting energy bench" << std::endl;

			std::cout << "Starting inference execution time bench" << std::endl;

			executionTimingBench.startBench();
			std::cout << logStart << std::endl;
			executionTimingBench.printResult();
			std::cout << logEnd << std::endl;


			std::cout << "Exiting inference execution time bench" << std::endl;


			std::cout << "END" << std::endl;
			while(1) {}		// Waiting for reset
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


void energyBenchWrapper(void){
	pendulum_ptr->startInference((int)nbActions);
}

void timingExecutionBenchWrapper(void){
	// Pendulum setup time is negligible over inference execution time
	pendulum_ptr->setAngle(initAngle);
	pendulum_ptr->setVelocity(initVelocity);

	pendulum_ptr->startInference((int)nbActions);
}


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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
