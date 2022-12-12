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
#define THREAD_STACK_SIZE		512
#define QUEUE_STACK_SIZE		128
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
char inp_msg[] = "enter the data:\n";
//CHAR *q_transmit_bfr = "this is the received data\r\n";
uint8_t q_receive_bfr;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void thread_to_rcv_q_data_func(void);
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

  /* <!-- create a queue --> */
	if(tx_byte_allocate(byte_pool, (void **)&pointer, QUEUE_STACK_SIZE*sizeof(ULONG), TX_NO_WAIT) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
	}
	if(tx_queue_create(&queue_ptr1, "queue-0", 1, pointer, QUEUE_STACK_SIZE*sizeof(ULONG)) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;;
	}

  /* <!-- create a thread --> */

  if(tx_byte_allocate(byte_pool, (void **)&pointer, THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
  }
  if(tx_thread_create(&thread_to_rcv_q_data, "thread-2", (void*)thread_to_rcv_q_data_func, 0x0000, pointer, THREAD_STACK_SIZE, 1, 0, 1, TX_AUTO_START) != TX_SUCCESS){
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
void thread_to_rcv_q_data_func(void){
	while(1){
		status = tx_queue_receive(&queue_ptr1, (VOID *)&q_receive_bfr, TX_WAIT_FOREVER);
#ifdef DEV_LOGS_ENABLED
		if(status==0x01){
			HAL_UART_Transmit(&huart3, (uint8_t*)"TX_DELETED", strlen("TX_DELETED"), 100);
		}
		if(status==0x0a){
			HAL_UART_Transmit(&huart3, (uint8_t*)"TX_QUEUE_EMPTY", strlen("TX_QUEUE_EMPTY"), 100);
		}
		if(status==0x1a){
			HAL_UART_Transmit(&huart3, (uint8_t*)"TX_WAIT_ABORTED", strlen("TX_WAIT_ABORTED"), 100);
		}
		if(status==0x09){
			HAL_UART_Transmit(&huart3, (uint8_t*)"TX_QUEUE_ERROR", strlen("TX_QUEUE_ERROR"), 100);
		}
		if(status==0x03){
			HAL_UART_Transmit(&huart3, (uint8_t*)"TX_PTR_ERROR", strlen("TX_PTR_ERROR"), 100);
		}
		if(status==0x04){
			HAL_UART_Transmit(&huart3, (uint8_t*)"TX_WAIT_ERROR", strlen("TX_WAIT_ERROR"), 100);
		}
#endif
		HAL_UART_Transmit(&huart3, &q_receive_bfr, 1, 10);
	}
}
/* USER CODE END 1 */
