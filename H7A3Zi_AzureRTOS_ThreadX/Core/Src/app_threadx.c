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
#define __PATTERN_1_3_2_4__
#undef __PATTERN_1_3_2_4__
#define THREAD_STACK_SIZE		512
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
//UART_HandleTypeDef huart3;
//TX_THREAD thread_ptr1;
//TX_THREAD thread_ptr2;
//TX_THREAD thread_ptr3;
//TX_THREAD thread_ptr4;
//TX_SEMAPHORE semaphore;
//int status;
//
char message1[] = "This is thread-1\n";
char message2[] = "This is thread-2\n";
char message3[] = "This is thread-3\n";
char message4[] = "This is thread-4\n";

/* get the thread info */
//CHAR *name;
//UINT state;
//ULONG run_count;
//UINT priority[4];
//UINT preemption_threshold[4];
//UINT time_slice;
//TX_THREAD *next_thread;
//TX_THREAD *suspended_thread;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void thread1_entry_func(void);
void thread2_entry_func(void);
void thread3_entry_func(void);
void thread4_entry_func(void);
void priority_inverse(void);
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
  CHAR *pointer;

  /* <!-- create a semaphore --> */
    if(tx_semaphore_create(&semaphore_1, "semaphore-1", 1) != TX_SUCCESS){
  	  ret = TX_POOL_ERROR;
    }
    if(tx_semaphore_create(&semaphore_2, "semaphore-2", 0) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
	}
    if(tx_semaphore_create(&semaphore_3, "semaphore-3", 0) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
	}
    if(tx_semaphore_create(&semaphore_4, "semaphore-4", 0) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
	}

    /* <!-- allocate memory to thread-1 from memory pool -->
    NOTE: byte_pool is already created above. We just have to change the value of TX_APP_MEM_POOL_SIZE in the App/app_azure_rtos_config.h file */
    if(tx_byte_allocate(byte_pool, (void **)&pointer, THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS){
  	  ret = TX_POOL_ERROR;
    }
    if(tx_thread_create(&thread_ptr1, "thread-1", (void*)thread1_entry_func, 0x0000, pointer, THREAD_STACK_SIZE, 15, 15, 1, TX_AUTO_START) != TX_SUCCESS){
  	  ret = TX_POOL_ERROR;
    }

    /* <!-- allocate memory to thread-2 from memory pool --> */
    if(tx_byte_allocate(byte_pool, (void **)&pointer, THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS){
  	  ret = TX_POOL_ERROR;
    }
    if(tx_thread_create(&thread_ptr2, "thread-2", (void*)thread2_entry_func, 0x0000, pointer, THREAD_STACK_SIZE, 15, 15, 1, TX_AUTO_START) != TX_SUCCESS){
  	  ret = TX_POOL_ERROR;
    }

    /* <!-- allocate memory to thread-3 from memory pool --> */
	if(tx_byte_allocate(byte_pool, (void **)&pointer, THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
	}
	if(tx_thread_create(&thread_ptr3, "thread-3", (void*)thread3_entry_func, 0x0000, pointer, THREAD_STACK_SIZE, 15, 15, 1, TX_AUTO_START) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
	}

	/* <!-- allocate memory to thread-4 from memory pool --> */
	if(tx_byte_allocate(byte_pool, (void **)&pointer, THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
	}
	if(tx_thread_create(&thread_ptr4, "thread-4", (void*)thread4_entry_func, 0x0000, pointer, THREAD_STACK_SIZE, 15, 15, 1, TX_AUTO_START) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
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
		status = tx_semaphore_get(&semaphore_1, TX_WAIT_FOREVER);

#ifdef __PATTERN_1_3_2_4__
		status = tx_semaphore_put(&semaphore_3);
#else
		if(pattern_flag == 1 && support_flag == 1){
			HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
			tx_thread_sleep(50); //delay of 500 ms
			status = tx_semaphore_put(&semaphore_4);
		}
		else if(pattern_flag == 1 && support_flag == 0){
			support_flag = 1;
			status = tx_semaphore_put(&semaphore_4);
		}
		else if(pattern_flag == 0 && support_flag == 0){
			support_flag = 1;
			status = tx_semaphore_put(&semaphore_2);
		}
		else if(pattern_flag == 0 && support_flag == 1){
			HAL_UART_Transmit(&huart3, (uint8_t *)message1, strlen(message1), 100);
			tx_thread_sleep(50); //delay of 500 ms
			status = tx_semaphore_put(&semaphore_2);
		}
#endif

	}
}

void thread2_entry_func(void){
	while(1){
		status = tx_semaphore_get(&semaphore_2, TX_WAIT_FOREVER);

#ifdef __PATTERN_1_3_2_4__
		status = tx_semaphore_put(&semaphore_4);
#else
		if(pattern_flag == 1 && support_flag == 1){
			HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
			tx_thread_sleep(50); //delay of 500 ms
			status = tx_semaphore_put(&semaphore_1);
		}
		else if((pattern_flag == 1) && (support_flag == 0)){
			support_flag = 1;
			status = tx_semaphore_put(&semaphore_4);
		}
		else if(pattern_flag == 0 && support_flag == 0){
			support_flag = 1;
			status = tx_semaphore_put(&semaphore_1);
		}
		else if(pattern_flag == 0 && support_flag == 1){
			HAL_UART_Transmit(&huart3, (uint8_t *)message2, strlen(message2), 100);
			tx_thread_sleep(50); //delay of 500 ms
			status = tx_semaphore_put(&semaphore_3);
		}
#endif

	}
}

void thread3_entry_func(void){
	while(1){
		status = tx_semaphore_get(&semaphore_3, TX_WAIT_FOREVER);

#ifdef __PATTERN_1_3_2_4__
		status = tx_semaphore_put(&semaphore_2);
#else
		if(pattern_flag == 1 && support_flag == 1){
			HAL_UART_Transmit(&huart3, (uint8_t *)message3, strlen(message3), 100);
			tx_thread_sleep(50); //delay of 500 ms
			status = tx_semaphore_put(&semaphore_2);
		}
		else if((pattern_flag == 1) && (support_flag == 0)){
			support_flag = 1;
			status = tx_semaphore_put(&semaphore_4);
		}
		else if(pattern_flag == 0 && support_flag == 0){
			support_flag = 1;
			status = tx_semaphore_put(&semaphore_1);
		}
		else if(pattern_flag == 0 && support_flag == 1){
			HAL_UART_Transmit(&huart3, (uint8_t *)message3, strlen(message3), 100);
			tx_thread_sleep(50); //delay of 500 ms
			status = tx_semaphore_put(&semaphore_4);
		}
#endif

	}
}

void thread4_entry_func(void){
	while(1){
		/* get the suspended semaphore */
		status = tx_semaphore_get(&semaphore_4, TX_WAIT_FOREVER);

#ifdef __PATTERN_1_3_2_4__
		status = tx_semaphore_put(&semaphore_1);
#else
		if((pattern_flag == 1) && (support_flag == 1)){
			HAL_UART_Transmit(&huart3, (uint8_t *)message4, strlen(message4), 100);
			tx_thread_sleep(50); //delay of 500 ms
			status = tx_semaphore_put(&semaphore_3);
		}
		else if((pattern_flag == 1) && (support_flag == 0)){
			support_flag = 1;
			status = tx_semaphore_put(&semaphore_3);
		}
		else if(pattern_flag == 0 && support_flag == 0){
			support_flag = 1;
			status = tx_semaphore_put(&semaphore_1);
		}
		else if(pattern_flag == 0 && support_flag == 1){
			HAL_UART_Transmit(&huart3, (uint8_t *)message4, strlen(message4), 100);
			tx_thread_sleep(50); //delay of 500 ms
			status = tx_semaphore_put(&semaphore_1);
		}
#endif

	}
}

/* USER CODE END 1 */
