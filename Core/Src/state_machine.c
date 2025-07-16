/*
 * state_machine.c
 *
 * Stores step counting data for all modules to access
 *
 * Created on: Mar 28, 2025
 * Author: T. Linton, J. Legg
 */

#include "state_machine.h"
#include "rotary_pot.h"
#include <stdio.h>
#include <task_buzzer.h>

#define DEFAULT_GOAL 1000

static StepCounterStateMachine step_counter;
static uint32_t temp_set_goal;


/* Initialise state machine with default values */
void stateMachine_Init (void)
{
	step_counter.current_display_state 	= STATE_CURRENT_STEPS;
	step_counter.test_mode_enabled 		= false;
	step_counter.unit_mode 				= UNITS_STEPS;
	step_counter.step_count 			= 0;
	step_counter.goal 					= DEFAULT_GOAL;
	step_counter.goal_completed 		= false;
}



void stateMachine_PreviousState(void)
{
	switch (step_counter.current_display_state) {
		case STATE_CURRENT_STEPS:
			step_counter.current_display_state = STATE_DISTANCE_TRAVELLED;
			break;
		case STATE_DISTANCE_TRAVELLED:
			step_counter.current_display_state = STATE_GOAL_PROGRESS;
			break;
		case STATE_GOAL_PROGRESS:
			step_counter.current_display_state = STATE_CURRENT_STEPS;
			break;
		case STATE_SET_GOAL:
			break;
	}
}


void stateMachine_NextState(void)
{
	switch (step_counter.current_display_state) {
		case STATE_CURRENT_STEPS:
			step_counter.current_display_state = STATE_GOAL_PROGRESS;
			break;
		case STATE_DISTANCE_TRAVELLED:
			step_counter.current_display_state = STATE_CURRENT_STEPS;
			break;
		case STATE_GOAL_PROGRESS:
			step_counter.current_display_state = STATE_DISTANCE_TRAVELLED;
			break;
		case STATE_SET_GOAL:
			break;
	}
}


void stateMachine_IncrementStepCount (uint16_t increment)
{
	step_counter.step_count += increment;

	/* If in test mode, do not allow steps to increase beyond the goal*/
	if (step_counter.test_mode_enabled && step_counter.step_count > step_counter.goal) {
		step_counter.step_count = step_counter.goal;
	}

	/* alert user if they reached the goal for the first time */
	if (!step_counter.goal_completed && step_counter.step_count >= step_counter.goal) {
		step_counter.goal_completed = true;
		buzzer_TurnOn ();
	}
}


void stateMachine_DecrementStepCount (uint16_t decrement)
{
	/* Do not decrement below 0 */
	if (step_counter.step_count <= decrement) {
		step_counter.step_count = 0;
	} else {
		step_counter.step_count -= decrement;
	}

	/* reset goal-completed status */
	if (step_counter.step_count < step_counter.goal) {
		step_counter.goal_completed = false;
	}
}


DisplayState stateMachine_DisplayStateGetter (void)
{
	return step_counter.current_display_state;
}


uint32_t stateMachine_StepCountGetter (void)
{
	return step_counter.step_count;
}


uint32_t stateMachine_GoalGetter (void)
{
    return step_counter.goal;
}


bool stateMachine_GoalModeEnabledGetter (void)
{
	return step_counter.current_display_state == STATE_SET_GOAL;
}

// check if test mode is enabled
bool stateMachine_TestModeEnabledGetter (void)
{
	return step_counter.test_mode_enabled;
}


UnitDisplayMode stateMachine_DisplayUnitGetter (void)
{
	return step_counter.unit_mode;
}

/*
 * Enter/Exit test mode
 * Saves and restores previous state
 */
void stateMachine_ToggleTestState(void)
{
    if (!step_counter.test_mode_enabled) {
        step_counter.previous_unit = step_counter.unit_mode; //saves unit
        step_counter.saved_step_count = step_counter.step_count; //saves the step count
    } else {
        step_counter.unit_mode = step_counter.previous_unit;
        step_counter.step_count = step_counter.saved_step_count;
    }

    step_counter.test_mode_enabled = !step_counter.test_mode_enabled;
}

/* Toggle units used for display */
void stateMachine_ToggleUnits (void)
{
    if (step_counter.current_display_state == STATE_DISTANCE_TRAVELLED) {
        // Toggle between KM and YD
        step_counter.unit_mode = (step_counter.unit_mode == UNITS_KM) ? UNITS_PERCENT : UNITS_KM;
    } else {
        // Toggle between STEPS and PERCENT
        step_counter.unit_mode = (step_counter.unit_mode == UNITS_STEPS) ? UNITS_PERCENT : UNITS_STEPS;
    }
}

/*
 * Enter set-goal-state
 * Save current goal so that it can be restored
 */
void stateMachine_EnterSetGoalState (void)
{
	step_counter.current_display_state = STATE_SET_GOAL;
	temp_set_goal = step_counter.goal;
}

/* Confirm new goal from rotary pot and reset goal status */
void stateMachine_ConfirmNewGoal (void)
{
	step_counter.current_display_state = STATE_GOAL_PROGRESS;
    step_counter.goal = rotaryPot_ReadGoal();

    /* reset goal_completed status */
    if (step_counter.goal > step_counter.step_count) {
    	step_counter.goal_completed = false;
    }
}


/* Exit "set goal state" without updating the goal */
void stateMachine_RevertGoal (void)
{
	step_counter.current_display_state = STATE_GOAL_PROGRESS;
    step_counter.goal = temp_set_goal;
}


