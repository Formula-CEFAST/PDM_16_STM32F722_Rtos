/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f7xx_hal.h"

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
#define PWM_4_Pin GPIO_PIN_10
#define PWM_4_GPIO_Port GPIOB
#define PWM_3_Pin GPIO_PIN_11
#define PWM_3_GPIO_Port GPIOB
#define BUCK_2_Pin GPIO_PIN_12
#define BUCK_2_GPIO_Port GPIOB
#define AlwaysOn_Pin GPIO_PIN_13
#define AlwaysOn_GPIO_Port GPIOB
#define BUCK_1_Pin GPIO_PIN_14
#define BUCK_1_GPIO_Port GPIOB
#define MediumCurrent_1_Pin GPIO_PIN_15
#define MediumCurrent_1_GPIO_Port GPIOB
#define MediumCurrent_2_Pin GPIO_PIN_6
#define MediumCurrent_2_GPIO_Port GPIOC
#define HighCurrent_1_Pin GPIO_PIN_7
#define HighCurrent_1_GPIO_Port GPIOC
#define HighCurrent_2_Pin GPIO_PIN_8
#define HighCurrent_2_GPIO_Port GPIOC
#define AlwaysOnA8_Pin GPIO_PIN_8
#define AlwaysOnA8_GPIO_Port GPIOA
#define AlwaysOnA9_Pin GPIO_PIN_9
#define AlwaysOnA9_GPIO_Port GPIOA
#define AlwaysOnA10_Pin GPIO_PIN_10
#define AlwaysOnA10_GPIO_Port GPIOA
#define SEL1_Pin GPIO_PIN_15
#define SEL1_GPIO_Port GPIOA
#define SEL0_Pin GPIO_PIN_10
#define SEL0_GPIO_Port GPIOC
#define AlwaysOnC11_Pin GPIO_PIN_11
#define AlwaysOnC11_GPIO_Port GPIOC
#define AlwaysOnC12_Pin GPIO_PIN_12
#define AlwaysOnC12_GPIO_Port GPIOC
#define PWM_2_Pin GPIO_PIN_7
#define PWM_2_GPIO_Port GPIOB
#define PWM_5_Pin GPIO_PIN_8
#define PWM_5_GPIO_Port GPIOB
#define PWM_1_Pin GPIO_PIN_9
#define PWM_1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
