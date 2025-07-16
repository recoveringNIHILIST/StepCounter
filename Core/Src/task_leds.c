/*
 * task_leds.c
 *
 * Use DS1,2,3 & 4 to show the goal progress
 * 0-25%, 50%, 75% and 100% goal completed
 *
 * Created on: Mar 7, 2025
 * Author: T. Linton, J. Legg
 */

#include "rgb.h"
#include "pwm.h"
#include "state_machine.h"

#include <stdint.h>

#define SCALE_1000 	1000
#define PERCENT_25 	250
#define PERCENT_50 	500
#define PERCENT_75 	750
#define PERCENT_100 1000
#define DUTY_FULL 	100

static uint32_t goal_progress;


/*
 * Initialises leds to be all off and white
 * Initialises PWM settings for DS3
 */
void taskLeds_Init (void)
{
	rgb_led_all_off ();
	rgb_colour_all_on ();
	pwm_init ();
	pwm_setDutyCycle (&htim2, TIM_CHANNEL_3, 0); // DS3
}


/* Calculate goal progress as parts per thousand */
static void taskLeds_CalcProgress (void)
{
	uint32_t current_steps = stateMachine_StepCountGetter ();
	uint32_t goal_steps = stateMachine_GoalGetter ();

	goal_progress = (current_steps * (uint32_t) SCALE_1000) / goal_steps;
	if (goal_progress > (uint32_t) SCALE_1000) {
		goal_progress = (uint32_t) SCALE_1000;
	}
}

/* Update led and pwm output based on goal progress */
void taskLeds_Execute (void)
{
	taskLeds_CalcProgress ();
	rgb_led_all_off ();

	if (goal_progress == PERCENT_100) {
			rgb_led_on (RGB_LEFT);
			rgb_led_on (RGB_DOWN);
			rgb_led_on (RGB_RIGHT);
			pwm_setDutyCycle (&htim2, TIM_CHANNEL_3, DUTY_FULL);

		} else if (goal_progress >= PERCENT_75) {
			rgb_led_on (RGB_LEFT);
			rgb_led_on (RGB_DOWN);
			pwm_setDutyCycle (&htim2, TIM_CHANNEL_3, DUTY_FULL);

		} else if (goal_progress >= PERCENT_50) {
			rgb_led_on (RGB_LEFT);
			pwm_setDutyCycle (&htim2, TIM_CHANNEL_3, DUTY_FULL);

		} else if (goal_progress >= PERCENT_25) {
			pwm_setDutyCycle (&htim2, TIM_CHANNEL_3, DUTY_FULL);

		} else {
			pwm_setDutyCycle (&htim2, TIM_CHANNEL_3, goal_progress);
		}
}



