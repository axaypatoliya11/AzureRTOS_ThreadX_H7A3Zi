/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "stm32h7xx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define THREAD_STACK_SIZE	1024
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
UART_HandleTypeDef huart3;
uint8_t thread_stack1[THREAD_STACK_SIZE];
uint8_t thread_stack2[THREAD_STACK_SIZE];
TX_THREAD thread_ptr1;
TX_THREAD thread_ptr2;
char message1[] = "hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world from thread-1\n";
char message2[] = "hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world hello world from thread-2\n";
char notify[] = "semaphore puted!!!";
int status;
TX_SEMAPHORE semaphore;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void thread1_entry_func(void);
void thread2_entry_func(void);
void temp(void);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  status = tx_semaphore_create(&semaphore, "semaphore-1", 1);
  if(status == TX_SUCCESS){
	  tx_thread_create(&thread_ptr1, "thread-1", (void*)thread1_entry_func, 0x0000, thread_stack1, THREAD_STACK_SIZE, 15, 15, 1, TX_AUTO_START);
	  tx_thread_create(&thread_ptr2, "thread-2", (void*)thread2_entry_func, 0x0000, thread_stack2, THREAD_STACK_SIZE, 15, 15, 1, TX_AUTO_START);
  }
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
void thread1_entry_func(void){
	while(1){
		status = tx_semaphore_get(&semaphore, TX_WAIT_FOREVER);
		HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
		status = tx_semaphore_put(&semaphore);
	}
}

void thread2_entry_func(void){
	while(1){
		status = tx_semaphore_get(&semaphore, TX_WAIT_FOREVER);
		HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
		HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
		status = tx_semaphore_put(&semaphore);
	}
}

void temp(void){
	HAL_UART_Transmit(&huart3, (uint8_t *)notify, strlen(notify), 100);
}
/* USER CODE END 1 */
