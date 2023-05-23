/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "usart.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* x - number of task (1,2,3)
 * datax[0] - mean
 * datax[1] - square mean
 * statusx  - flag =1 when means are up to date
 */
typedef struct data_t{
	float data1[2];
	float data2[2];
	float data3[2];
	uint8_t status1;
	uint8_t status2;
	uint8_t status3;
}data_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define SEMAPHORE TRUE
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

data_t data;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask01 */
osThreadId_t myTask01Handle;
const osThreadAttr_t myTask01_attributes = {
  .name = "myTask01",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
const osThreadAttr_t myTask03_attributes = {
  .name = "myTask03",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for myPrintTask */
osThreadId_t myPrintTaskHandle;
const osThreadAttr_t myPrintTask_attributes = {
  .name = "myPrintTask",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myMutex01 */
osMutexId_t myMutex01Handle;
const osMutexAttr_t myMutex01_attributes = {
  .name = "myMutex01"
};
/* Definitions for myBinarySem01 */
osSemaphoreId_t myBinarySem01Handle;
const osSemaphoreAttr_t myBinarySem01_attributes = {
  .name = "myBinarySem01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask01(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void StartPrintTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of myMutex01 */
  myMutex01Handle = osMutexNew(&myMutex01_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myBinarySem01 */
  myBinarySem01Handle = osSemaphoreNew(1, 1, &myBinarySem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask01 */
  myTask01Handle = osThreadNew(StartTask01, NULL, &myTask01_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* creation of myTask03 */
  myTask03Handle = osThreadNew(StartTask03, NULL, &myTask03_attributes);

  /* creation of myPrintTask */
  myPrintTaskHandle = osThreadNew(StartPrintTask, NULL, &myPrintTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask01 */
/**
* @brief Function implementing the myTask01 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask01 */
void StartTask01(void *argument)
{
  /* USER CODE BEGIN StartTask01 */
	float rnd, mean, square;
	srand((unsigned)osKernelSysTick());
	data.status1 = 0;
  /* Infinite loop */
  for(;;)
  {
	  if(!data.status1) {

		HAL_GPIO_TogglePin(Task1_GPIO_Port, Task1_Pin);

		#ifdef SEMAPHORE
		osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
		#else
		osMutexAcquire(myMutex01Handle, osWaitForever);
		#endif

		mean = 0;
		square = 0;

		for(uint16_t i = 0; i < 1000; i++) {
		 // cast random value to float, divide by RAND_MAX to get 0-1 range, *2 to get 0-2 range and -1 to get -1 to 1 range
		  rnd = (float)rand() / RAND_MAX * 2.0f - 1.0f;
		  mean += rnd;
		  square += rnd * rnd;
		}

		data.data1[0] = mean/1000;
		data.data1[1] = sqrt(square/1000);
		data.status1  =	1;

		#ifdef SEMAPHORE
		osSemaphoreRelease(myBinarySem01Handle);
		#else
		osMutexRelease(myMutex01Handle);
		#endif

		HAL_GPIO_TogglePin(Task1_GPIO_Port, Task1_Pin);

	}
    osDelay(1);
  }
  /* USER CODE END StartTask01 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
	float rnd, mean, square;
	srand((unsigned)osKernelSysTick());
	data.status2 = 0;

  /* Infinite loop */
  for(;;)
  {
	  if(!data.status2) {

		HAL_GPIO_TogglePin(Task2_GPIO_Port, Task2_Pin);

		#ifdef SEMAPHORE
		osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
		#else
		osMutexAcquire(myMutex01Handle, osWaitForever);
		#endif

		mean = 0;
		square = 0;

		for(uint16_t i = 0; i < 1000; i++) {
		 // cast random value to float, divide by RAND_MAX to get 0-1 range, *2 to get 0-2 range and -1 to get -1 to 1 range
		  rnd = (float)rand() / RAND_MAX * 2.0f - 1.0f;
		  mean += rnd;
		  square += rnd * rnd;
		}

		data.data2[0] = mean/1000;
		data.data2[1] = sqrt(square/1000);
		data.status2  =	1;

		#ifdef SEMAPHORE
		osSemaphoreRelease(myBinarySem01Handle);
		#else
		osMutexRelease(myMutex01Handle);
		#endif

		HAL_GPIO_TogglePin(Task2_GPIO_Port, Task2_Pin);

	}
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
	float rnd, mean, square;
	srand((unsigned)osKernelSysTick());
	data.status3 = 0;

  /* Infinite loop */
  for(;;)
  {
	  if(!data.status3) {

		HAL_GPIO_TogglePin(Task3_GPIO_Port, Task3_Pin);


		#ifdef SEMAPHORE
		osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
		#else
		osMutexAcquire(myMutex01Handle, osWaitForever);
		#endif

		mean = 0;
		square = 0;

		for(uint16_t i = 0; i < 1000; i++){

			// cast random value to float, divide by RAND_MAX to get 0-1 range, *2 to get 0-2 range and -1 to get -1 to 1 range
			rnd = (float)rand() / RAND_MAX * 2.0f - 1.0f;
			mean += rnd;
			square += rnd * rnd;
		}

		data.data3[0] = mean/1000;
		data.data3[1] = sqrt(square/1000);
		data.status3  = 1;

		#ifdef SEMAPHORE
		osSemaphoreRelease(myBinarySem01Handle);
		#else
		osMutexRelease(myMutex01Handle);
		#endif

		HAL_GPIO_TogglePin(Task3_GPIO_Port, Task3_Pin);

	  }
	  osDelay(1);
  }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartPrintTask */
/**
* @brief Function implementing the myPrintTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPrintTask */
void StartPrintTask(void *argument)
{
  /* USER CODE BEGIN StartPrintTask */
	float means[2];
	uint8_t buf[61];
	/* Infinite loop */
	for(;;)
	{
		if (data.status1 && data.status2 && data.status3){

			HAL_GPIO_TogglePin(PrintTask_GPIO_Port, PrintTask_Pin);

			#ifdef SEMAPHORE
			osSemaphoreAcquire(myBinarySem01Handle, osWaitForever);
			#else
			osMutexAcquire(myMutex01Handle, osWaitForever);
			#endif

			//means of means
			means[0] = (data.data1[0] + data.data2[0] + data.data3[0]) / 3;
			means[1] = (data.data1[1] + data.data2[1] + data.data3[1]) / 3;

			sprintf((char*)buf, "Mean of means = %f | Mean of square means = %f", means[0], means[1]);
			HAL_UART_Transmit(&huart2, buf, strlen((const char*)buf), HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2, (uint8_t*)"\n", sizeof("\n"), HAL_MAX_DELAY);

			// delay 10 * 60 chars
			osDelay(600);

			data.status1 = 0;
			data.status2 = 0;
			data.status3 = 0;

			#ifdef SEMAPHORE
			osSemaphoreRelease(myBinarySem01Handle);
			#else
			osMutexRelease(myMutex01Handle);
			#endif

			HAL_GPIO_TogglePin(PrintTask_GPIO_Port, PrintTask_Pin);
		}

		//1s - 600ms from previous delay
		osDelay(400);
	}
  /* USER CODE END StartPrintTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

