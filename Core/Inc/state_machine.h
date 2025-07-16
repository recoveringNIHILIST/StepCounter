/*
 * state_machine.h
 *
 *  Created on: Mar 28, 2025
 *      Author: jle182
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

#include <stdint.h>
#include <stdbool.h>


typedef enum {
    STATE_CURRENT_STEPS,
    STATE_DISTANCE_TRAVELLED,
    STATE_GOAL_PROGRESS,
    STATE_SET_GOAL
} DisplayState;

typedef enum {
    UNITS_STEPS,
    UNITS_PERCENT,
    UNITS_KM,
	UNITS_YD
} UnitDisplayMode;

typedef struct {
    DisplayState current_display_state;
    bool test_mode_enabled;
    UnitDisplayMode unit_mode;

    uint32_t step_count;
    uint32_t goal;

    bool goal_completed;
    UnitDisplayMode previous_unit;
    uint32_t saved_step_count;

} StepCounterStateMachine;

void stateMachine_Init (void);

void stateMachine_PreviousState (void);
void stateMachine_NextState (void);

void stateMachine_IncrementStepCount (uint16_t increment);
void stateMachine_DecrementStepCount (uint16_t decrement);

DisplayState stateMachine_DisplayStateGetter (void);
uint32_t stateMachine_StepCountGetter (void);
uint32_t stateMachine_GoalGetter (void);
bool stateMachine_GoalModeEnabledGetter (void);
bool stateMachine_TestModeEnabledGetter (void);
UnitDisplayMode stateMachine_DisplayUnitGetter (void);

void stateMachine_ToggleTestState (void);
void stateMachine_ToggleUnits (void);

void stateMachine_EnterSetGoalState (void);
void stateMachine_ConfirmNewGoal (void);
void stateMachine_RevertGoal (void);

#endif /* INC_STATE_MACHINE_H_ */
