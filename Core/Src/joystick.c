/*
 * joystick.c
 *
 * Calculates Joystick position as percentage
 * Detects joystick position (UP, DOWN, LEFT, RIGHT)
 *
 * Created on: May 16, 2025
 * Author: T. Linton, J. Legg
 */


#include "joystick.h"
#include "adc.h"

#define REST_X   	2100
#define REST_Y   	2200
#define MAX_X    	3950  // Left
#define MAX_Y    	4000  // Down
#define MIN_X    	290   // Right
#define MIN_Y    	260   // Up
#define MAX_PERCENT 100
#define REST_BUFFER 2
#define SCALE_10	10

static JOYSTICK_POSITION joystick_position;


/* Calculate joystick direction and percentage values */
void joystick_CalcPosition (void)
{
    static uint16_t adc_values[2];
    adc_JoystickGetter (adc_values);

    if (adc_values[0] > REST_X) {
    	joystick_position.x_direction = X_LEFT;
    }
    else {
    	joystick_position.x_direction = X_RIGHT;
    }

    if (adc_values[1] > REST_Y) {
    	joystick_position.y_direction = Y_DOWN;
    }
    else {
    	joystick_position.y_direction = Y_UP;
    }

    joystick_RawAdcToPercentage(adc_values);
	joystick_FilterPercentageValues (&adc_values[0], &joystick_position.x_direction);
	joystick_FilterPercentageValues (&adc_values[1], &joystick_position.y_direction);

	joystick_position.x_percentage = (uint8_t)adc_values[0];
	joystick_position.y_percentage = (uint8_t)adc_values[1];
}


/* Convert raw ADC values to 0-100% displacement */
void joystick_RawAdcToPercentage (uint16_t *adc_values)
{
	if (adc_values[0] > REST_X) {
		adc_values[0] = ((adc_values[0] - REST_X) * 100 / (MAX_X - REST_X));
	} else {
		adc_values[0] = ((REST_X - adc_values[0]) * 100 / (REST_X - MIN_X));
	}

	if (adc_values[1] > REST_Y)
	{
		adc_values[1] = ((adc_values[1] - REST_Y) * 100 / (MAX_Y - REST_Y));
	} else {
		adc_values[1] = ((REST_Y - adc_values[1]) * 100 / (REST_Y - MIN_Y));
	}
}


/*
 * Filter Percentage values to be within bounds (0-100%)
 * If at rest, set the direction to be = POS_REST
 */
void joystick_FilterPercentageValues (uint16_t *adc_value, JOYSTICK_DIRECTION *direction)
{
	if (*adc_value > MAX_PERCENT) {
		*adc_value = MAX_PERCENT;
	}
	if (*adc_value < REST_BUFFER) {
		*direction = POS_REST;
		*adc_value = 0;
	}
}



uint8_t joystick_xPercentageGetter (void)
{
	return joystick_position.x_percentage;
}


uint8_t joystick_yPercentageGetter (void)
{
	return joystick_position.y_percentage;
}


JOYSTICK_DIRECTION joystick_xDirectionGetter(void)
{
    return joystick_position.x_direction;
}


JOYSTICK_DIRECTION joystick_yDirectionGetter(void)
{
    return joystick_position.y_direction;
}

/*
 * Return the scaled and then squared displacement for Y direction
 */
uint16_t joystick_yScaledDirectionGetter (void)
{
	uint16_t displace_mod10 = joystick_yPercentageGetter() / SCALE_10;
	return (displace_mod10 * displace_mod10);
}
