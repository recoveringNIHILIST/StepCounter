/*
 * joystick.h
 *
 *  Created on: May 16, 2025
 *      Author: NIHILIST
 */

#ifndef INC_JOYSTICK_H_
#define INC_JOYSTICK_H_

#include <stdint.h>

typedef enum {
	POS_REST = 0,
	X_LEFT,
	X_RIGHT,
	Y_UP,
	Y_DOWN
} JOYSTICK_DIRECTION;

typedef struct {
    uint8_t x_percentage;
    uint8_t y_percentage;
    JOYSTICK_DIRECTION x_direction;
    JOYSTICK_DIRECTION y_direction;
} JOYSTICK_POSITION;

void joystick_CalcPosition (void);
void joystick_RawAdcToPercentage ( uint16_t *adc_values);
void joystick_FilterPercentageValues (uint16_t *adc_value, JOYSTICK_DIRECTION *direction);

uint8_t joystick_xPercentageGetter (void);
uint8_t joystick_yPercentageGetter (void);
JOYSTICK_DIRECTION joystick_xDirectionGetter(void);
JOYSTICK_DIRECTION joystick_yDirectionGetter(void);
uint16_t joystick_yScaledDirectionGetter ();


#endif /* INC_JOYSTICK_H_ */
