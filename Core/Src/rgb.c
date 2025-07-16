/*
 * rgb.c
 *
 *  Created on: Dec 5, 2024
 *      Author: fsy13
 */

#include <stdbool.h>
#include <stdio.h>

#include "rgb.h"
#include "gpio.h"

typedef struct
{
	GPIO_TypeDef* port;
	uint16_t pin;
	bool active_high;
} rgb_gpio_config_t;

const rgb_gpio_config_t RGB_LEDS[RGB_NUM_LEDS] =
 {
	[RGB_LEFT] = {
			.port = GPIOF,
			.pin = GPIO_PIN_3,
			.active_high = false
	},
	[RGB_DOWN] = {
			.port = GPIOC,
			.pin = GPIO_PIN_2,
			.active_high = false
	},
	[RGB_UP] = {
			.port = GPIOC,
			.pin = GPIO_PIN_6,
			.active_high = false
	},
	[RGB_RIGHT] = {
			.port = GPIOC,
			.pin = GPIO_PIN_12,
			.active_high = false
	}
 };

const rgb_gpio_config_t RGB_COLOURS[RGB_NUM_COLOURS] =
 {
	[RGB_RED] = {
			.port = GPIOD,
			.pin = GPIO_PIN_3,
			.active_high = true
	},
	[RGB_GREEN] = {
			.port = GPIOD,
			.pin = GPIO_PIN_2,
			.active_high = true
	},
	[RGB_BLUE] = {
			.port = GPIOD,
			.pin = GPIO_PIN_4,
			.active_high = true
	},
 };


// turn led on using active high or active low logic

void rgb_led_on(rgb_led_t led)
{
	GPIO_PinState state = RGB_LEDS[led].active_high ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(RGB_LEDS[led].port, RGB_LEDS[led].pin, state);
}
// turn colour on using active high or active low logic

void rgb_colour_on(rgb_colour_t colour)
{
	GPIO_PinState state = RGB_COLOURS[colour].active_high ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(RGB_COLOURS[colour].port, RGB_COLOURS[colour].pin, state);
}
// turn led off using active high or active low logic
void rgb_led_off(rgb_led_t led)
{
	printf("Turning OFF LED index %d\n", led);
	GPIO_PinState state = RGB_LEDS[led].active_high ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(RGB_LEDS[led].port, RGB_LEDS[led].pin, state);
}
// turn colour off using active high or active low logic
void rgb_colour_off(rgb_colour_t colour)
{
	GPIO_PinState state = RGB_COLOURS[colour].active_high ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(RGB_COLOURS[colour].port, RGB_COLOURS[colour].pin, state);
}
// toggle led state
void rgb_led_toggle(rgb_led_t led)
{
	HAL_GPIO_TogglePin(RGB_LEDS[led].port, RGB_LEDS[led].pin);
}
// toggle colour state
void rgb_colour_toggle(rgb_colour_t colour)
{
	HAL_GPIO_TogglePin(RGB_COLOURS[colour].port, RGB_COLOURS[colour].pin);
}

// turn all leds on sequentially
void rgb_led_all_on(void)
{
	for (uint8_t i = 0; i < RGB_NUM_LEDS; i++)
	{
		rgb_led_on(i);
	}
}
// turn all leds off sequentially
void rgb_led_all_off(void)
{
	for (uint8_t i = 0; i < RGB_NUM_LEDS; i++)
	{
		rgb_led_off(i);
	}
}
// turn all colours on sequentially
void rgb_colour_all_on(void)
{
	for (uint8_t i = 0; i < RGB_NUM_COLOURS; i++)
	{
		rgb_colour_on(i);
	}
}
// turn all colours off sequentially
void rgb_colour_all_off(void)
{
	for (uint8_t i = 0; i < RGB_NUM_COLOURS; i++)
	{
		rgb_colour_off(i);
	}
}
