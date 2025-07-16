/*
 * rotary_pot.c
 *
 *  Created on: Apr 23, 2025
 *      Author: NIHILIST
 */

#include "rotary_pot.h"
#include "adc.h"

#define MIN_ROTARY_VAL		100
#define MAX_ROTARY_VAL		4000

#define MIN_GOAL			500
#define MAX_GOAL			15000
#define NUM_GOAL_INCREMENTS	400

#define RANGE_ROTARY 		(MAX_ROTARY_VAL - MIN_ROTARY_VAL)
#define STEP_SIZE 			(MAX_GOAL - MIN_GOAL) / (NUM_GOAL_INCREMENTS)

// read potentiometer adc value and map to goal range
uint32_t rotaryPot_ReadGoal (void)
{
	uint16_t potentiometer_adc;
	adc_PotentiometerGetter (&potentiometer_adc);

	uint16_t index = (potentiometer_adc - MIN_ROTARY_VAL) * (NUM_GOAL_INCREMENTS) / RANGE_ROTARY;
	uint32_t goal = MIN_GOAL + (index * STEP_SIZE);


	if (goal < MIN_GOAL) {
		goal = MIN_GOAL;
	} else if (goal > MAX_GOAL) {
		goal = MAX_GOAL;
	}

	return goal;
}
