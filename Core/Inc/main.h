/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Encoder1_A_Pin GPIO_PIN_0
#define Encoder1_A_GPIO_Port GPIOA
#define Encoder1_B_Pin GPIO_PIN_1
#define Encoder1_B_GPIO_Port GPIOA
#define Motor1_DIR_Pin GPIO_PIN_2
#define Motor1_DIR_GPIO_Port GPIOA
#define Motor1_PWM_Pin GPIO_PIN_3
#define Motor1_PWM_GPIO_Port GPIOA
#define Host_INT_Pin GPIO_PIN_4
#define Host_INT_GPIO_Port GPIOA
#define Encoder2_A_Pin GPIO_PIN_6
#define Encoder2_A_GPIO_Port GPIOA
#define Encoder2_B_Pin GPIO_PIN_7
#define Encoder2_B_GPIO_Port GPIOA
#define Motor2_DIR_Pin GPIO_PIN_0
#define Motor2_DIR_GPIO_Port GPIOB
#define Motor2_PWM_Pin GPIO_PIN_1
#define Motor2_PWM_GPIO_Port GPIOB
#define Relay_Pin GPIO_PIN_10
#define Relay_GPIO_Port GPIOB
#define Encoder3_A_Pin GPIO_PIN_8
#define Encoder3_A_GPIO_Port GPIOA
#define Encoder3_B_Pin GPIO_PIN_9
#define Encoder3_B_GPIO_Port GPIOA
#define Moror3_PWM_Pin GPIO_PIN_10
#define Moror3_PWM_GPIO_Port GPIOA
#define Motor3_DIR_Pin GPIO_PIN_11
#define Motor3_DIR_GPIO_Port GPIOA
#define Stepper2_PUL_Pin GPIO_PIN_12
#define Stepper2_PUL_GPIO_Port GPIOA
#define Stepper2_DIR_Pin GPIO_PIN_4
#define Stepper2_DIR_GPIO_Port GPIOB
#define Stepper2_PULB5_Pin GPIO_PIN_5
#define Stepper2_PULB5_GPIO_Port GPIOB
#define Servo4_Pin GPIO_PIN_6
#define Servo4_GPIO_Port GPIOB
#define Servo3_Pin GPIO_PIN_7
#define Servo3_GPIO_Port GPIOB
#define Servo2_Pin GPIO_PIN_8
#define Servo2_GPIO_Port GPIOB
#define Servo1_Pin GPIO_PIN_9
#define Servo1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
