/*
 * buzzer.c
 *
 * Sounds buzzer using PWM for set number of milliseconds
 *
 * Created on: May 16, 2025
 * Author: T. Linton, J. Legg
 */

#include "task_buzzer.h"
#include "pwm.h"

#include <stdint.h>
#include <stdbool.h>

#define BUZZER_REACHED_GOAL_TIME_MS 750
#define BUZZER_0N_CCR 255

static uint32_t buzzer_on_tick = 0;
static bool buzzer_off = true;


/* Check state of buzzer and turn off if required */
void buzzer_Execute (void)
{
	if (buzzer_off) {
		return;
	}
	if ((HAL_GetTick () - buzzer_on_tick) >= BUZZER_REACHED_GOAL_TIME_MS) {
		buzzer_TurnOff ();
	}
}


/* Turn buzzer off by setting duty cycle to zero */
void buzzer_TurnOff (void)
{
	pwm_setDutyCycle (&htim16, TIM_CHANNEL_1, 0);
	buzzer_off = true;
}


/* Turn buzzer on and start timer */
void buzzer_TurnOn (void)
{
	buzzer_on_tick = HAL_GetTick ();
	buzzer_off = false;
	pwm_setDutyCycle (&htim16, TIM_CHANNEL_1, BUZZER_0N_CCR);
}
