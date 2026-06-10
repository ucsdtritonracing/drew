/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

#include "stm32g4xx_nucleo.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define RCC_OSC32_IN_Pin GPIO_PIN_14
#define RCC_OSC32_IN_GPIO_Port GPIOC
#define RCC_OSC32_OUT_Pin GPIO_PIN_15
#define RCC_OSC32_OUT_GPIO_Port GPIOC
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOF
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOF
#define APP1_IN_Pin GPIO_PIN_0
#define APP1_IN_GPIO_Port GPIOC
#define APP2_IN_Pin GPIO_PIN_1
#define APP2_IN_GPIO_Port GPIOC
#define BSER_IN_Pin GPIO_PIN_0
#define BSER_IN_GPIO_Port GPIOA
#define BSEF_IN_Pin GPIO_PIN_1
#define BSEF_IN_GPIO_Port GPIOA
#define RRWSS_Pin GPIO_PIN_5
#define RRWSS_GPIO_Port GPIOA
#define R2D_Button_Pin GPIO_PIN_6
#define R2D_Button_GPIO_Port GPIOA
#define R2D_Sound_Pin GPIO_PIN_7
#define R2D_Sound_GPIO_Port GPIOA
#define BRAKE_LIGHT_Pin GPIO_PIN_2
#define BRAKE_LIGHT_GPIO_Port GPIOB
#define FRWSS_Pin GPIO_PIN_9
#define FRWSS_GPIO_Port GPIOA
#define FLWSS_Pin GPIO_PIN_10
#define FLWSS_GPIO_Port GPIOA
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define SDC_IN_Pin GPIO_PIN_15
#define SDC_IN_GPIO_Port GPIOA
#define RLWSS_Pin GPIO_PIN_12
#define RLWSS_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
