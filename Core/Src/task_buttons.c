/*
 * task_buttons.c
 *
 * Poll buttons execute their tasks depending on the state
 *
 * Created on: Mar 7, 2025
 * Author: T. Linton, J. Legg
 */

#include <stdint.h>
#include <stdbool.h>

#include "buttons.h"
#include "state_machine.h"
#include "stm32c0xx_hal.h"

#define STEP_INCREMENT 80


/* Initialise button module */
void taskButtons_Init (void)
{
	buttons_init ();
}

/* Psoll buttons and handle joystick and button events */
void taskButtons_PollExecute (void)
{
    buttons_update ();
    joystick_event_t joystick_event = buttons_CheckHold ();

    /* Button operation dependent on step counter state */
    if (stateMachine_DisplayStateGetter () == STATE_GOAL_PROGRESS) {
        if (joystick_event == JOYSTICK_LONG_PRESS) {
            stateMachine_EnterSetGoalState ();
        }
    } else if (stateMachine_DisplayStateGetter() == STATE_SET_GOAL) {
        if (joystick_event == JOYSTICK_LONG_PRESS) {
            stateMachine_ConfirmNewGoal();
        } else if (joystick_event == JOYSTICK_SHORT_PRESS) {
            stateMachine_RevertGoal ();
        }
        joystick_event = JOYSTICK_NONE;
        return; // Disable other buttons when setting goal
    }
    joystick_event = JOYSTICK_NONE;


    if (buttons_checkButton (UP) == PUSHED) {
        stateMachine_IncrementStepCount (STEP_INCREMENT);
    }


    if (buttons_CheckDoublePush ()) {
        stateMachine_ToggleTestState ();
    }
}



