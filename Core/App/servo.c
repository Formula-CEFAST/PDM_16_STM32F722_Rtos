 /*
 * servo.c
 *
 *  Created on: 5 de fev. de 2026
 *      Author: diogo
 */


#include "servo.h"
#include "app_data.h"


/* ================================
   Servo configuration array
================================ */

static Servo_CfgType servoCfg[MAX_SERVOS] =
{
		//Servos e Mais um ai
    {
        .canIndice = 0,
        .inicialAngle = 0,
        .maxAngle = 180.0f,
        .anguloIndice = 0,
        .minPulse = 500.0f,    // us (ou CCR mínimo)
        .maxPulse = 2500.0f,   // us (ou CCR máximo)
        .TIM_Handle = &htim2,    // será atribuído no init
        .TIM_Channel = TIM_CHANNEL_1

    },

    {
        .canIndice = 1,
        .inicialAngle = 0,
        .maxAngle = 180.0f,
        .anguloIndice = 0,
        .minPulse = 500.0f,
        .maxPulse = 2500.0f,
        .TIM_Handle = &htim2,
        .TIM_Channel = TIM_CHANNEL_2

    },
    {
            .canIndice = 2,
            .inicialAngle = 0,
            .maxAngle = 180.0f,
            .anguloIndice = 1,
            .minPulse = 1000.0f,
            .maxPulse = 2000.0f,
            .TIM_Handle = &htim4,
            .TIM_Channel = TIM_CHANNEL_3

        },
		{
			    .canIndice = 3,
			    .inicialAngle = 0,
			    .maxAngle = 100.0f,
			    .anguloIndice = 1,
			    .minPulse = 1000.0f,
			    .maxPulse = 2000.0f,
			    .TIM_Handle = &htim4,
			    .TIM_Channel = TIM_CHANNEL_1

			},
			{
						    .canIndice = 3,
						    .inicialAngle = 0,
						    .maxAngle = 100.0f,
						    .anguloIndice = 1,
						    .minPulse = 1000.0f,
						    .maxPulse = 2000.0f,
						    .TIM_Handle = &htim4,
						    .TIM_Channel = TIM_CHANNEL_2

						}
};


static uint32_t Servo_GetPulseForPercent(uint16_t indiceServo, uint8_t percent)
{
    if(indiceServo >= MAX_SERVOS)
        return 0u;

    if(percent > 100u)
        percent = 100u;

    /* Primeiro dois servos: coletor invertido com escala 0..42 */
    if(indiceServo < 2){
           __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1000*genericPWMDuty[0]);
           __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1000*genericPWMDuty[0]);}

    /* Outros: ESC estilo 1000..2000 us */
    return (uint32_t)(1000 + percent * 10u);
}

/* ================================
   Init
================================ */

void Servo_InitConfig(void)
{
    for(uint8_t i = 0; i < MAX_SERVOS; i++)
    {
        HAL_TIM_PWM_Start(servoCfg[i].TIM_Handle, servoCfg[i].TIM_Channel);

        /* inicializa o duty genérico com o ângulo inicial configurado */
        genericPWMDuty[ servoCfg[i].anguloIndice ] = (uint8_t)servoCfg[i].inicialAngle;

        /* aplica posição inicial
           mantém as funções originais para compatibilidade */
        if(i < 2){
            servoMoveTo(i);
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1000*genericPWMDuty[0]);
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1000*genericPWMDuty[0]);}
        else
            escMoveTo(i);
    }
}

/* ================================
   Move servo (lê genericPWMDuty)
================================ */

void servoMoveTo(uint16_t indiceServo)
{
    if(indiceServo >= MAX_SERVOS)
        return;

    Servo_CfgType *servo = &servoCfg[indiceServo];

    uint8_t percent = genericPWMDuty[ servo->anguloIndice ];

    uint32_t pulse = Servo_GetPulseForPercent(indiceServo, percent);

    __HAL_TIM_SET_COMPARE(servo->TIM_Handle,
                          servo->TIM_Channel,
                          pulse);
}


void escMoveTo(uint16_t indiceServo)
{
    /* escMoveTo mantém compatibilidade — usa mesma lógica que servoMoveTo */
    servoMoveTo(indiceServo);
}

/* Atualiza todos os servos (chamar periodicamente) */
void Servos_Update(void)
{
    for(uint16_t i = 0; i < MAX_SERVOS; ++i)
    {
        servoMoveTo(i);
    }
}

