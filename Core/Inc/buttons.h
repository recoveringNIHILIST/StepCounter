#ifndef BUTTONS_H_
#define BUTTONS_H_

// *******************************************************
// buttons.h
//
// Support for a set of FIVE specific buttons on the NUCLEO boards.
// The buttons are UP, DOWN, LEFT, RIGHT and JOYSTICK_CLICK.
//
// Created by P.J. Bones, UC ECE
// Updated by Le Yang & F. Youssif, UC ECE. 15/01/2025
// Updated by T. Linton 23/04/2025
//
// *******************************************************

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
// Constants
//*****************************************************************************
typedef enum butNames {UP = 0, DOWN, LEFT, RIGHT, JOYSTICK_CLICK, NUM_BUTTONS} buttonName_t;
typedef enum butStates {RELEASED = 0, PUSHED, NO_CHANGE} buttonState_t;

typedef enum {
    JOYSTICK_NONE,
    JOYSTICK_SHORT_PRESS,
    JOYSTICK_LONG_PRESS
} joystick_event_t;

// Debouncing algorithm: A finite state machine (FSM) is associated with each button.
// A state change can be declared only after NUM_BUT_POLLS consecutive polls have
// read the pin in the opposite condition, before the state changes and
// a flag is set. Set NUM_BUT_POLLS according to the polling rate.

// *******************************************************
// buttons_init: Initialize the variables associated with the set of buttons
// defined by the constants above.
void buttons_init (void);

// *******************************************************
// buttons_update: Function designed to be called regularly. It polls all
// buttons once and updates variables associated with the buttons if
// necessary. It is efficient enough to be part of an ISR for e.g.,
// a SysTick interrupt.
void buttons_update (void);

// *******************************************************
// buttons_checkButton: Function returns the new button state if the button state
// (PUSHED or RELEASED) has changed since the last call, otherwise returns
// NO_CHANGE. The argument butName should be one of constants in the
// enumeration butStates, excluding 'NUM_BUTS'. Safe under interrupt.
buttonState_t buttons_checkButton (buttonName_t butName);

joystick_event_t buttons_CheckHold(void);
bool buttons_CheckDoublePush (void);

#endif /*BUTTONS_H_*/
