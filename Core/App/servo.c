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
		//Servos 1 e 2
    {
        .canIndice = 0,
        .inicialAngle = 0,
        .maxAngle = 180.0f,
        .anguloIndice = 0,
        .minPulse = 500.0f,    // us (ou CCR mínimo)
        .maxPulse = 2500.0f,   // us (ou CCR máximo)
        .TIM_Handle = &htim2,    // será atribuído no init
        .TIM_Channel = TIM_CHANNEL_3

    },

    {
        .canIndice = 1,
        .inicialAngle = 0,
        .maxAngle = 180.0f,
        .anguloIndice = 0,
        .minPulse = 500.0f,
        .maxPulse = 2500.0f,
        .TIM_Handle = &htim2,
        .TIM_Channel = TIM_CHANNEL_4

    },


	//Spare PWM Signals and main ESC signal output
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

        uint32_t scaled_42 = (percent * 42u) / 100u; // 0..42
        //inverted pulse calculation
        uint32_t pulse_width = servoCfg[indiceServo].maxPulse - (scaled_42 * (servoCfg[indiceServo].maxPulse- servoCfg[indiceServo].minPulse)) / 100u;        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,pulse_width);

        return pulse_width;

    }

    // Outros: Standard output linear curve
    return (uint32_t)(servoCfg[indiceServo].minPulse + percent * 10u);
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


        if(i < 2){
            servoMoveTo(i);
      }
        else
        	__HAL_TIM_SET_COMPARE(servoCfg[i].TIM_Handle,
        			servoCfg[i].TIM_Channel,
                    1000);
            //escMoveTo(i);
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

