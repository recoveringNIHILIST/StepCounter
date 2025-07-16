/*
 * task_display.c
 *
 * Prints to OLED display the state of step counter
 * Uses integer math (no floats) for conversion between steps/km/m/yd
 *
 * Created on: Mar 11, 2025
 * Author: T. Linton, J. Legg
 */

#include "task_display.h"
#include "ssd1306_fonts.h"
#include "ssd1306.h"
#include "state_machine.h"
#include "rotary_pot.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define STEP_TO_METRE_X100      90        // 90 metres to 100 steps
#define METRE_TO_YARD_X100      109       // 109 yards to 100 steps
#define METRE_X100_TO_KM        100000
#define KM_TO_METRE				1000
#define DECIMAL_POINT_SCALE     100       // scale for 2 decimal places


static char buffer[32];

/* Initialise OLED display */
void taskDisplay_Init (void)
{
	ssd1306_Init ();
}


/* Write test mode status to buffer and display it */
void taskDisplay_PrintTestMode (void)
{
	if (stateMachine_TestModeEnabledGetter ()) {
		snprintf (buffer, sizeof(buffer), "Test Mode ON");
	} else {
		snprintf (buffer, sizeof(buffer), "Test Mode OFF");
	}
	ssd1306_WriteString (buffer, Font_7x10, White);
}


/* Format and write step count or percentage to buffer */
void taskDisplay_PrintSteps (void)
{
	if (stateMachine_DisplayUnitGetter () == UNITS_STEPS) {
		snprintf(buffer, sizeof(buffer), "Steps: %lu",
				stateMachine_StepCountGetter ());
	} else {
		uint16_t percent = (stateMachine_StepCountGetter () * 100) / stateMachine_GoalGetter ();
		snprintf(buffer, sizeof(buffer), "Steps: %u%%", percent);
	}

}


/* Calculate distance in km or yd and write string to buffer */
void taskDisplay_Distance (void)
{
	uint32_t dist_m_x100 = stateMachine_StepCountGetter() * STEP_TO_METRE_X100;

	if (stateMachine_DisplayUnitGetter() == UNITS_KM) {
		uint32_t km_whole = dist_m_x100 / METRE_X100_TO_KM;
		uint32_t km_frac  = (dist_m_x100 % METRE_X100_TO_KM) / KM_TO_METRE;
		snprintf(buffer, sizeof(buffer), "Dist: %lu.%02lu km", km_whole, km_frac);
	} else {
		uint32_t yards_x100 = (dist_m_x100 * METRE_TO_YARD_X100) / DECIMAL_POINT_SCALE;
		snprintf(buffer, sizeof(buffer), "Dist: %lu.%02lu yd",
				yards_x100 / DECIMAL_POINT_SCALE, yards_x100 % DECIMAL_POINT_SCALE);
	}
}


/*
 * Clears display
 * Calls print functions
 * Updates screen
 */
void taskDisplay_Execute (void)
{
	ssd1306_Fill (Black);

	ssd1306_SetCursor(0, 0);
	taskDisplay_PrintTestMode ();

	ssd1306_SetCursor (0, 24);
    switch (stateMachine_DisplayStateGetter ()) {
		case STATE_CURRENT_STEPS:
			taskDisplay_PrintSteps ();
			break;
		case STATE_GOAL_PROGRESS:
			snprintf(buffer, sizeof(buffer), "%lu steps / %lu",
					stateMachine_StepCountGetter (), stateMachine_GoalGetter ());
			break;
		case STATE_DISTANCE_TRAVELLED:
			taskDisplay_Distance ();
			break;
		case STATE_SET_GOAL:
			snprintf(buffer, sizeof(buffer), "Set Goal: %lu", rotaryPot_ReadGoal ());
			break;

		default:
			snprintf(buffer, sizeof(buffer), " ");
			break;
    }
    ssd1306_WriteString(buffer, Font_7x10, White);

    ssd1306_UpdateScreen();
}


















