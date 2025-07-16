/*
 * app.c
 *
 *  Created on: Feb 25, 2025
 *      Author: tli101
 */
#include "stm32c0xx_hal.h"
#include "app.h"
#include "state_machine.h"
#include "task_buttons.h"
#include "task_joystick.h"
#include "task_leds.h"
#include "task_display.h"
#include "task_read_imu.h"
#include "task_buzzer.h"
#include "adc.h"

#define TICK_FREQUENCY_HZ 1000
#define HZ_TO_TICKS(FREQUENCY_HZ) (TICK_FREQUENCY_HZ / FREQUENCY_HZ)

#define JOYSTICK_PERIOD_TICKS           HZ_TO_TICKS(JOYSTICK_FREQUENCY_HZ)
#define LEDS_PERIOD_TICKS				HZ_TO_TICKS(LEDS_FREQUENCY_HZ)
#define ADC_PERIOD_TICKS           		HZ_TO_TICKS(ADC_FREQUENCY_HZ)
#define DISPLAY_PERIOD_TICKS            HZ_TO_TICKS(DISPLAY_FREQUENCY_HZ)
#define POLL_BUTTONS_PERIOD_TICKS       HZ_TO_TICKS(POLL_BUTTONS_FREQUENCY_HZ)
#define IMU_PERIOD_TICKS				HZ_TO_TICKS(IMU_FREQUENCY_HZ)
#define BUZZER_PERIOD_TICKS				HZ_TO_TICKS(BUZZER_FREQUENCY_HZ)

#define IMU_FREQUENCY_HZ				100
#define POLL_BUTTONS_FREQUENCY_HZ 		100
#define JOYSTICK_FREQUENCY_HZ 			8
#define ADC_FREQUENCY_HZ 				8
#define LEDS_FREQUENCY_HZ				4
#define DISPLAY_FREQUENCY_HZ 			4
#define BUZZER_FREQUENCY_HZ				1

static uint32_t poll_buttons_next_run 	= 0;
static uint32_t display_next_run 		= 0;
static uint32_t joystick_next_run 		= 0;
static uint32_t leds_next_run			= 0;
static uint32_t adc_next_run 			= 0;
static uint32_t imu_next_run 			= 0;
static uint32_t buzzer_next_run 		= 0;


void app_main (void)
{
	taskDisplay_Init ();
	taskButtons_Init ();
	stateMachine_Init ();
	taskLeds_Init ();
	imu_Init ();

	poll_buttons_next_run 	= HAL_GetTick () + POLL_BUTTONS_PERIOD_TICKS;
	joystick_next_run 		= HAL_GetTick () + JOYSTICK_PERIOD_TICKS;
	adc_next_run			= HAL_GetTick () + ADC_PERIOD_TICKS;
	display_next_run 		= HAL_GetTick () + DISPLAY_PERIOD_TICKS;
	imu_next_run			= HAL_GetTick () + IMU_PERIOD_TICKS;
	leds_next_run			= HAL_GetTick () + LEDS_PERIOD_TICKS;

	while (1)
	{
		uint32_t ticks = HAL_GetTick ();

		if (ticks > joystick_next_run) {
			task_joystick_execute ();
			joystick_next_run += JOYSTICK_PERIOD_TICKS;
		}

		if (ticks > leds_next_run) {
			taskLeds_Execute ();
			leds_next_run += LEDS_PERIOD_TICKS;
		}

		if (ticks > adc_next_run) {
			adc_Execute ();
			adc_next_run += ADC_PERIOD_TICKS;
		}

		if (ticks > poll_buttons_next_run) {
			taskButtons_PollExecute ();
			poll_buttons_next_run += POLL_BUTTONS_PERIOD_TICKS;
		}


		if (ticks > display_next_run) {
			taskDisplay_Execute ();
			display_next_run += DISPLAY_PERIOD_TICKS;
		}

		if (ticks > imu_next_run) {
			imu_Execute ();
			imu_next_run += IMU_PERIOD_TICKS;
		}

		if (ticks > buzzer_next_run) {
			buzzer_Execute ();
			buzzer_next_run += BUZZER_PERIOD_TICKS;
		}
	}
}

