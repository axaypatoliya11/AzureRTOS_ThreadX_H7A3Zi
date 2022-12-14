/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NULL_DATA_FOR_THREAD	0x000
#define THREAD_STACK_SIZE		512
#define NETX_STACK_SIZE			512
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
UART_HandleTypeDef huart3;
TX_THREAD server_thread;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void server_thread_func(void);
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

  /* <!-- create a thread --> */
  if(tx_byte_allocate(byte_pool, (void **)&pointer, THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
  }
  if(tx_thread_create(&server_thread, "Server Thread", (void*)server_thread_func, NULL_DATA_FOR_THREAD, pointer, THREAD_STACK_SIZE, 15, 15, 1, TX_AUTO_START) != TX_SUCCESS){
	  ret = TX_POOL_ERROR;
  }

  /* Allocate the memory for packet_pool.  */
  if (tx_byte_allocate(byte_pool, (void **)&pointer, NETX_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS){
    return TX_POOL_ERROR;
  }
  /* Create a packet pool.  */
  if(nx_packet_pool_create(&nx_packet_pool, "NetX Main Packet Pool", 40, (void *)pointer , NETX_STACK_SIZE) != TX_SUCCESS){
	  printf("error in creating NX packet pool\r\n");
  }
  /* Create an IP Instance.  */
  if(nx_ip_create(&ip, "NetX IP Instance (ip)", IP_ADDRESS(192, 168, 1, 1), IP_ADDRESS(255, 255, 255, 0), &nx_packet_pool, &user_driver, pointer, NETX_STACK_SIZE, 1) != TX_SUCCESS){
	  printf("error in creating an IP instance\r\n");
  }
  /* create a HTTP server */
  if(nx_http_server_create(&http_server, "HTTP Server", &ip, &media_instance, pointer, NETX_STACK_SIZE, &nx_packet_pool, NULL, NULL) != TX_SUCCESS){
	  printf("error in creating HTTP server");
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
void server_thread_func(void){
	while(1){
		printf("sample data\r\n");
	}
}
/* USER CODE END 1 */
