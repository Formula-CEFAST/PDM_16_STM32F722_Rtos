/*
 * servo.h
 *
 *  Created on: 5 de fev. de 2026
 *      Author: diogo
 */

#ifndef APP_SERVO_H_
#define APP_SERVO_H_


#include "tim.h"
#include "gpio.h"
#include "app_data.h"

#define MAX_SERVOS 5
typedef struct

{
	uint16_t canIndice;
	uint16_t inicialAngle;
	float maxAngle;
	uint8_t anguloIndice; /* index dentro de genericPWMDuty */
	float minPulse;
	float maxPulse;
    TIM_HandleTypeDef *TIM_Handle;
    uint32_t TIM_Channel;


} Servo_CfgType;

void servoMoveTo(uint16_t indiceServo);
void escMoveTo(uint16_t indiceServo);

/* Atualiza todos os servos (chamar periodicamente) */
void Servos_Update(void);

void Servo_InitConfig(void);


#endif /* APP_SERVO_H_ */
