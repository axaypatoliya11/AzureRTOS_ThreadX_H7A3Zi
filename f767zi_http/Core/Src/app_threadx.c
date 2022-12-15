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
#include "nx_api.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NULL_DATA_FOR_THREAD 0x0000

#define HTTP_SERVER_ADDRESS  IP_ADDRESS(1,2,3,4)

#define NOB_IN_EACH_PACKET   128

#define SERVER_PACKET_SIZE  (NX_HTTP_SERVER_MIN_PACKET_SIZE * 2)

#define THREAD_STACK_SIZE		512
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
UINT status;

NX_PACKET_POOL  server_pool;
NX_IP           server_ip;

NX_PACKET_POOL pool_0;

TX_THREAD server_thread;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void server_thread_function(void);
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

  /* <!-- Initialize the NetX system --> */
//  nx_system_initialize();

  /* <!-- Create a thread --> */
  if(tx_byte_allocate(byte_pool, (void **)&pointer, THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS){
	 ret = TX_POOL_ERROR;
  }
  if(tx_thread_create(&server_thread, "thread-2", (void*)server_thread_function, NULL_DATA_FOR_THREAD, pointer, THREAD_STACK_SIZE, 1, 0, 1, TX_AUTO_START) != TX_SUCCESS){
	 ret = TX_POOL_ERROR;
  }


  /* <!-- Allocate the memory for packet pool. --> */
  if(tx_byte_allocate(byte_pool, (VOID **) &pointer, SERVER_PACKET_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  /* <!-- Create a packet pool. --> */
//  if(nx_packet_pool_create(&pool_0, "NetX Main Packet Pool", NOB_IN_EACH_PACKET, (UCHAR *)pointer , SERVER_PACKET_SIZE) != NX_SUCCESS){
//	  ret = NX_POOL_ERROR;
//  }

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
void server_thread_function(void){
	while(1){
		printf("print from server thread\r\n");
	}
}
/* USER CODE END 1 */
