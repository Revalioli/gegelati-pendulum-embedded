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
#include "PendulumEnvironment.h"
#include "INA219Monitor.h"
#include "INA219Bench.h"
#include "PendulumINA219Monitor.h"
#include "TimingBench.h"
#include "ina219.h"
extern "C" {
	#include "pendulum_program.h"
}

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

/* === Pendulum global access and parameters === */

PendulumEnvironment * pendulum_ptr;
uint16_t nbActions = 1000;	// Number of actions per inference
INA219Monitor * monitor_ptr;
uint64_t actions[NB_ACTIONS];

/* === Energy measurements === */

INA219_t ina219t;
const char logStart[] = "##### Log Start #####";
const char logEnd[] = "##### Log End #####";

/* === TPG === */

uint32_t seed = 0;
extern "C" {
	extern double* in1;
}


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* ===[ Bench functions ]=== */
void inferenceBenchWrapper(void);
void energyMeasurementTimingBenchWrapper(void);
void environmentEvolutionTimingBenchWrapper(void);

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
	MX_TIM7_Init();
	MX_USART2_UART_Init();
	MX_TIM5_Init();
	/* USER CODE BEGIN 2 */

	/* === Pendulum environment === */
	std::vector<double> availableAction = {0.05, 0.1, 0.2, 0.4, 0.6, 0.8, 1.0};
	PendulumEnvironment pendulum(availableAction);
	in1 = pendulum.currentState;
	pendulum_ptr = &pendulum;	// For benchmark


	/* === INA219 === */
	if(INA219_Init(&ina219t, &hi2c1, INA219_DEFAULT_ADDRESS, 3.2768) == 0){
		std::cout << "Can't initialise INA219_t, end program" << std::endl;
		while(1){}
	}

	INA219_setConfig(&ina219t, INA219_CONFIG_BADCRES_12BIT | INA219_CONFIG_BVOLTAGERANGE_32V
								| INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS
								| INA219_CONFIG_SADCRES_12BIT);	// 0x219f


	// TIM5, used for timing benches, raises interrupt every microseconds
	// TIM7, used for energy monitoring, raised interrupt every 3 milliseconds

	/* === Timing Benchmark === *///

	TimingBench benchInference(inferenceBenchWrapper, &htim5, 15, TimeUnit::Milliseconds, 0.001f);

	INA219Monitor timingRecordMonitor(&ina219t, &htim7);
	timingRecordMonitor.flushWhenFull = false;
	monitor_ptr = &timingRecordMonitor;
	TimingBench benchINA219Monitor(energyMeasurementTimingBenchWrapper, &htim5, 100, TimeUnit::Microseconds, 1.f);


	for(int i = 0; i < NB_ACTIONS; i++)	// Generate random actions for benchEvolution
		actions[i] = rand() % 15;
	TimingBench benchEvolution(environmentEvolutionTimingBenchWrapper, &htim5, 15, TimeUnit::Milliseconds, 0.001f);

	/* === Current Benchmark === */

	PendulumINA219Monitor pendulumMonitor(&ina219t, pendulum, &htim7, TimeUnit::Microseconds, 3.f);
	INA219Bench inferenceEnergyBench(inferenceBenchWrapper, &pendulumMonitor);

	std::cout << "Press user push button to start benchmarks" << std::endl << std::endl;

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		if(PC13Sig){

			// Inference timing bench
			std::cout << "===> Starting inference timing bench" << std::endl;

			benchInference.startBench();
			benchInference.printResult();

			std::cout << "===> Exiting inference timing bench" << std::endl << std::endl;


			// INA219Monitor record timing
			std::cout << "===> Starting INA219Monitor timing bench" << std::endl;

			benchINA219Monitor.startBench();
			benchINA219Monitor.printResult();

			std::cout << "===> Exiting INA219Monitor timing bench" << std::endl << std::endl;


			// Inference energy bench
			std::cout << "===> Starting inference energy bench" << std::endl;

			std::cout << logStart << std::endl;
			inferenceEnergyBench.startBench();
			std::cout << logEnd << std::endl;

			std::cout << "===> Exiting inference energy bench" << std::endl << std::endl;


			// PendulumEnvironment evolution timing bench
			std::cout << "===> Starting environment timing bench" << std::endl;

			benchEvolution.startBench();
			benchEvolution.printResult();

			std::cout << "===> Exiting environment timing bench" << std::endl << std::endl;

			PC13Sig = false;
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

/* ===[ Bench functions ]=== */

void inferenceBenchWrapper(void){
	// Setup
	seed = HAL_GetTick();
	pendulum_ptr->reset(seed);

	pendulum_ptr->startInference((int)nbActions);
}

void energyMeasurementTimingBenchWrapper(void){
	monitor_ptr->record();
}

void environmentEvolutionTimingBenchWrapper(void){

	seed = HAL_GetTick();
	pendulum_ptr->reset(seed);
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < NB_ACTIONS; j++){
			pendulum_ptr->doAction(actions[j]);
		}
	}

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
