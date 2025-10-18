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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <inttypes.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

COM_InitTypeDef BspCOMInit;
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc4;

FDCAN_HandleTypeDef hfdcan1;

TIM_HandleTypeDef htim2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ReadAPPS */
osThreadId_t ReadAPPSHandle;
uint32_t ReadAPPSBuffer[ 128 ];
osStaticThreadDef_t ReadAPPSControlBlock;
const osThreadAttr_t ReadAPPS_attributes = {
  .name = "ReadAPPS",
  .stack_mem = &ReadAPPSBuffer[0],
  .stack_size = sizeof(ReadAPPSBuffer),
  .cb_mem = &ReadAPPSControlBlock,
  .cb_size = sizeof(ReadAPPSControlBlock),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ReadWSS */
osThreadId_t ReadWSSHandle;
uint32_t ReadWSSBuffer[ 128 ];
osStaticThreadDef_t ReadWSSControlBlock;
const osThreadAttr_t ReadWSS_attributes = {
  .name = "ReadWSS",
  .stack_mem = &ReadWSSBuffer[0],
  .stack_size = sizeof(ReadWSSBuffer),
  .cb_mem = &ReadWSSControlBlock,
  .cb_size = sizeof(ReadWSSControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ReadCAN */
osThreadId_t ReadCANHandle;
uint32_t ReadCANBuffer[ 128 ];
osStaticThreadDef_t ReadCANControlBlock;
const osThreadAttr_t ReadCAN_attributes = {
  .name = "ReadCAN",
  .stack_mem = &ReadCANBuffer[0],
  .stack_size = sizeof(ReadCANBuffer),
  .cb_mem = &ReadCANControlBlock,
  .cb_size = sizeof(ReadCANControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ReadCOM */
osThreadId_t ReadCOMHandle;
uint32_t ReadCOMBuffer[ 128 ];
osStaticThreadDef_t ReadCOMControlBlock;
const osThreadAttr_t ReadCOM_attributes = {
  .name = "ReadCOM",
  .stack_mem = &ReadCOMBuffer[0],
  .stack_size = sizeof(ReadCOMBuffer),
  .cb_mem = &ReadCOMControlBlock,
  .cb_size = sizeof(ReadCOMControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for WriteCOM */
osThreadId_t WriteCOMHandle;
uint32_t WriteCOMBuffer[ 128 ];
osStaticThreadDef_t WriteCOMControlBlock;
const osThreadAttr_t WriteCOM_attributes = {
  .name = "WriteCOM",
  .stack_mem = &WriteCOMBuffer[0],
  .stack_size = sizeof(WriteCOMBuffer),
  .cb_mem = &WriteCOMControlBlock,
  .cb_size = sizeof(WriteCOMControlBlock),
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */
__IO uint32_t BspButtonState = BUTTON_RELEASED;
uint32_t printCANData = 0;
uint32_t printSASAngle = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC4_Init(void);
static void MX_TIM2_Init(void);
static void MX_FDCAN1_Init(void);
void StartDefaultTask(void *argument);
void StartReadAPPS(void *argument);
void StartReadWSS(void *argument);
void StartReadCAN(void *argument);
void StartReadCOM(void *argument);
void StartWriteCOM(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

FDCAN_RxHeaderTypeDef   RxHeader;
uint8_t               	RxData[8];
int16_t					steeringAngle;

// FDCAN2 Callback
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
		/* Retreive Rx messages from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			Error_Handler();
		}
		if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
			Error_Handler();
		}
		if (printCANData) {
			printf("CAN ID: %" PRIu32 ", Data: ", RxHeader.Identifier);
			for (int i = 0; i < 12; i++) {
				printf("%02x ", RxData[i]);
			}
			printf("\n");
		}
		if (printSASAngle) {
			steeringAngle = ((uint16_t) RxData[1] << 8) | (uint16_t) RxData[0];
			printf("HI:8191,LO:-8191,SAS:%d\n", steeringAngle);
		}
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
  MX_ADC1_Init();
  MX_ADC4_Init();
  MX_TIM2_Init();
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */
	// Start FDCAN1
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
		Error_Handler();
	}
	// Activate the notification for new data in FIFO0 for FDCAN1
	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
		Error_Handler();
	}
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

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

  /* creation of ReadAPPS */
  ReadAPPSHandle = osThreadNew(StartReadAPPS, NULL, &ReadAPPS_attributes);

  /* creation of ReadWSS */
  ReadWSSHandle = osThreadNew(StartReadWSS, NULL, &ReadWSS_attributes);

  /* creation of ReadCAN */
  ReadCANHandle = osThreadNew(StartReadCAN, NULL, &ReadCAN_attributes);

  /* creation of ReadCOM */
  ReadCOMHandle = osThreadNew(StartReadCOM, NULL, &ReadCOM_attributes);

  /* creation of WriteCOM */
  WriteCOMHandle = osThreadNew(StartWriteCOM, NULL, &WriteCOM_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Initialize led */
  BSP_LED_Init(LED_GREEN);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.GainCompensation = 0;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC4_Init(void)
{

  /* USER CODE BEGIN ADC4_Init 0 */

  /* USER CODE END ADC4_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC4_Init 1 */

  /* USER CODE END ADC4_Init 1 */

  /** Common config
  */
  hadc4.Instance = ADC4;
  hadc4.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc4.Init.Resolution = ADC_RESOLUTION_12B;
  hadc4.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc4.Init.GainCompensation = 0;
  hadc4.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc4.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc4.Init.LowPowerAutoWait = DISABLE;
  hadc4.Init.ContinuousConvMode = DISABLE;
  hadc4.Init.NbrOfConversion = 1;
  hadc4.Init.DiscontinuousConvMode = DISABLE;
  hadc4.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc4.Init.DMAContinuousRequests = DISABLE;
  hadc4.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc4.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC4_Init 2 */

  /* USER CODE END ADC4_Init 2 */

}

/**
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = ENABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 2;
  hfdcan1.Init.NominalSyncJumpWidth = 8;
  hfdcan1.Init.NominalTimeSeg1 = 15;
  hfdcan1.Init.NominalTimeSeg2 = 8;
  hfdcan1.Init.DataPrescaler = 2;
  hfdcan1.Init.DataSyncJumpWidth = 8;
  hfdcan1.Init.DataTimeSeg1 = 15;
  hfdcan1.Init.DataTimeSeg2 = 8;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
FDCAN_TxHeaderTypeDef	TxHeader;
uint8_t 				TxData[2];

void ResetSASCalibration()
{
	TxHeader.Identifier = 0x7C0;		// LWS_Config
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_2;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;


	TxData[0] = 0x05;		// Command: Reset calibration
	TxData[1] = 0x00;
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData) != HAL_OK) {
		Error_Handler();
	}
}

void ResetSASAngle()
{
	TxHeader.Identifier = 0x7C0;		// LWS_Config
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_2;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	TxData[0] = 0x03;		// Command: Set LWS_Angle = 0
	TxData[1] = 0x00;
	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData) != HAL_OK) {
		Error_Handler();
	}
}

void BSP_PB_Callback(Button_TypeDef Button)
{
	if (Button == BUTTON_USER) {
		if (BspButtonState == BUTTON_RELEASED) {
			printf("Attempting to reset SAS...\n");
			BSP_LED_Toggle(LED_GREEN);
			// Configure TX Header for FDCAN1 BOSCH LWS_Config
			TxHeader.Identifier = 0x7C0;		// LWS_Config
			TxHeader.IdType = FDCAN_STANDARD_ID;
			TxHeader.TxFrameType = FDCAN_DATA_FRAME;
			TxHeader.DataLength = FDCAN_DLC_BYTES_2;
			TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
			TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
			TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
			TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
			TxHeader.MessageMarker = 0;


			TxData[0] = 0x05;		// Command: Reset calibration
			TxData[1] = 0x00;
			if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData) != HAL_OK) {
				Error_Handler();
			}

			TxData[0] = 0x03;		// Command: Set LWS_Angle = 0
			TxData[1] = 0x00;
			if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData) != HAL_OK) {
				Error_Handler();
			}

			BspButtonState = BUTTON_PRESSED;
		} else {
			BspButtonState = BUTTON_RELEASED;
		}
	}
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	printf("STM32 Started!\n");
	/* Infinite loop */
	for(;;) {
		BSP_LED_Toggle(LED_GREEN);
		osDelay(1000);
	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartReadAPPS */
/**
 * @brief Function implementing the ReadAPPS thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadAPPS */
void StartReadAPPS(void *argument)
{
  /* USER CODE BEGIN StartReadAPPS */
	uint32_t apps1Pos, apps2Pos, appsAvgPos;
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	/* Infinite loop */
	for(;;) {
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		apps1Pos = HAL_ADC_GetValue(&hadc1);
		//printf("HI:4095,LO:0,APPS1:%" PRIu32 "\n", apps1Pos);

		osDelay(1);
	}
  /* USER CODE END StartReadAPPS */
}

/* USER CODE BEGIN Header_StartReadWSS */
/**
 * @brief Function implementing the ReadWSS thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadWSS */
void StartReadWSS(void *argument)
{
  /* USER CODE BEGIN StartReadWSS */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
  /* USER CODE END StartReadWSS */
}

/* USER CODE BEGIN Header_StartReadCAN */
/**
 * @brief Function implementing the ReadCAN thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadCAN */
void StartReadCAN(void *argument)
{
  /* USER CODE BEGIN StartReadCAN */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
  /* USER CODE END StartReadCAN */
}

/* USER CODE BEGIN Header_StartReadCOM */
/**
 * @brief Function implementing the ReadCOM thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadCOM */
void StartReadCOM(void *argument)
{
  /* USER CODE BEGIN StartReadCOM */
	uint8_t rxByte;
	uint8_t buffer[64];
	uint8_t idx = 0;
	/* Infinite loop */
	for(;;) {
		if (printCANData) {
			printf("CAN ID: %" PRIu32 ", Data: ", RxHeader.Identifier);
			for (int i = 0; i < 12; i++) {
				printf("%02x ", RxData[i]);
			}
			printf("\n");
		}
		if (printSASAngle) {
			steeringAngle = ((uint16_t) RxData[1] << 8) | (uint16_t) RxData[0];
			printf("HI:8191,LO:-8191,SAS:%d\n", steeringAngle);
		}

		if (HAL_UART_Receive(&hcom_uart[COM1], &rxByte, 1, HAL_MAX_DELAY) == HAL_OK) {
			if (rxByte == '\n' || rxByte == '\r') {
				buffer[idx] = '\0';		// terminate string
				printf("$ %s\n", buffer);

				if (!strcmp(buffer, "sas calibrate")) {
					ResetSASCalibration();
					printf("SAS reset calibration command sent.\n");
				} else if (!strcmp(buffer, "sas zero")) {
					ResetSASAngle();
					printf("SAS reset angle command sent.\n");
				} else if (!strcmp(buffer, "sas angle")) {
					if (printSASAngle) {
						printSASAngle = 0;
						printf("Hiding SAS Angle.\n");
					} else {
						printSASAngle = 1;
						printf("Showing SAS Angle.\n");
					}
				} else if (!strcmp(buffer, "can data")) {
					if (printCANData) {
						printCANData = 0;
						printf("Hiding CAN Data.\n");
					} else {
						printCANData = 1;
						printf("Showing CAN Data.\n");
					}
				} else {
					printf("Unknown command: %s\r\n", buffer);
				}

				idx = 0;
			} else if (idx < sizeof(buffer) - 1) {
				buffer[idx++] = rxByte;
			}
		}
	}
  /* USER CODE END StartReadCOM */
}

/* USER CODE BEGIN Header_StartWriteCOM */
/**
 * @brief Function implementing the WriteCOM thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartWriteCOM */
void StartWriteCOM(void *argument)
{
  /* USER CODE BEGIN StartWriteCOM */
	uint8_t* buf;
	/* Infinite loop */
	for(;;)
	{
		if (osMessageQueueGet(COMQueue, &byte, NULL, osWaitForever) == osOK) {
			printf("Got byte: %c\r\n", byte);
		}
	}
  /* USER CODE END StartWriteCOM */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
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
