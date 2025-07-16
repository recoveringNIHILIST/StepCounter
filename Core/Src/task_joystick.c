/*
 * task_joystick.c
 *
 * Poll joystick and handle input based on state of step counter
 *
 * Created on: Mar 7, 2025
 * Author: T. Linton, J. Legg
 */

#include "task_joystick.h"
#include "state_machine.h"
#include "joystick.h"

#include <stdint.h>

#define MOVE_BUFFER 90


/* Handles Joystick status when in TestMode */
static void joystick_test_mode (void)
{
	uint16_t step_change = joystick_yScaledDirectionGetter ();

	if (joystick_yDirectionGetter () == Y_UP) {
		stateMachine_IncrementStepCount (step_change);
	}
	else if (joystick_yDirectionGetter() == Y_DOWN) {
		stateMachine_DecrementStepCount (step_change);
	}
}

/* Poll joystick, navigate states, toggle units, and apply test mode changes */
void task_joystick_execute (void)
{
	joystick_CalcPosition ();

	if (stateMachine_DisplayStateGetter () == STATE_SET_GOAL) {
		// Ignore all other inputs
		return;
	}

	if (joystick_xPercentageGetter () > MOVE_BUFFER) {
		if (joystick_xDirectionGetter () == X_RIGHT) {
			stateMachine_NextState ();

		} else if (joystick_xDirectionGetter () == X_LEFT) {
			stateMachine_PreviousState ();
		}

	} else if (!stateMachine_TestModeEnabledGetter()
				&& joystick_yPercentageGetter () > MOVE_BUFFER
	    		&& joystick_yDirectionGetter () == Y_UP) {
	       stateMachine_ToggleUnits();
	}

	if (stateMachine_TestModeEnabledGetter ()) {
		joystick_test_mode ();
	}
}






