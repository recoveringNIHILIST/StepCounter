/*
 * peak_detection.c
 *
 * Increments step count using filtered IMU data
 *
 * Created on: May 6, 2025
 * Author: T. Linton, J. Legg
 */

#include "peak_detection.h"
#include "filter.h"
#include "state_machine.h"

#define VAR_THRESHOLD			50000
#define DELTA_MEAN_THRESHOLD	2700
#define COOLDOWN_SAMPLES 	 	30
#define STEP_COUNT_INCREMENT 	1
#define MIN_SAMPLES				(3*N_SIZE)

static uint8_t  samples_taken	  	= 0;
static uint8_t  samples_since_step 	= COOLDOWN_SAMPLES;
static uint32_t prev_mag     	  	= 0;
static uint32_t mean_threshold;


/*
 * Counts a step on the falling edge of peak in magnitude
 * Uses variance to limit sensitivity when standing still
 * Waits for COOLDOWN_SAMPLES number of samples before counting a second step
 */
void peakDetection_Execute(void)
{
    uint32_t current = filter_MagnitudeCurrentGetter ();

    /* wait for mean & variance to settle before counting steps */
    if (samples_taken < MIN_SAMPLES) {
        samples_taken++;
        prev_mag = current;
        return;
    }

    if (samples_since_step < COOLDOWN_SAMPLES) {
    	samples_since_step++;
    	prev_mag = current;
    	return;
    }

    uint32_t variance = filter_MagnitudeScaledVarGetter ();
    uint32_t mean = filter_MagnitudeMeanGetter ();
    mean_threshold = mean + (uint32_t) DELTA_MEAN_THRESHOLD;

    /* detect downward crossing of mean+delta */
    if (	prev_mag > mean_threshold						// previous value was above the mean+DELTA
		&& 	current <= mean									// current value is below mean
        && 	variance > (uint32_t) VAR_THRESHOLD) 			// current value is above variance threshold
    {
        stateMachine_IncrementStepCount (STEP_COUNT_INCREMENT);
        samples_since_step = 0;
    }

    prev_mag = current;
}
