/*
 * filter.c
 *
 * Fixed-point exponential low-pass filtering for IMU sensor data,
 * smoothing acceleration readings on the X, Y, and Z axes.
 *
 * Calulates the mean and scaled variance of magnitude readings
 *
 * Created on: May 6, 2025
 * Author: T. Linton, J. Legg
 */


#include "filter.h"

#include <stdint.h>

#define ALPHA_SHIFT 3
#define VAR_SCALING 23

typedef struct {
	uint32_t buffer[N_SIZE];
	uint32_t mean;
	uint32_t sum;
	uint32_t scaled_variance;
	uint64_t sum_of_sq;
	uint8_t current_index;
} buffer_t;

static buffer_t mag_buffer;

static int16_t x_filtered = 0;
static int16_t y_filtered = 0;
static int16_t z_filtered = 0;


/*
 * Initialise xyz filters
 * Initialise magnitude buffer to be empty
 */
void filter_Init (void)
{
	x_filtered = 0;
	y_filtered = 0;
	z_filtered = 0;

	// Initialise buffer to all 0
	for (int8_t i = 0; i < N_SIZE; i++) {
		mag_buffer.buffer[i] = 0;
	}
	mag_buffer.current_index = 0;
	mag_buffer.sum = 0;
	mag_buffer.sum_of_sq = 0;
	mag_buffer.mean = 0;
}


/*
 * Low-pass EMA filter
 *
 * Code adapted from:
 * https://www.luisllamas.es/en/arduino-exponential-low-pass/
 * https://github.com/RafaelReyesCarmona/EMA
 *
 * IIR filter: y[n] = y[n-1] + alpha * (x[n] - y[n-1])
 * where alpha = 1/(2^ALPHA_SHIFT)
 */
void filter_IIR (int16_t new_x, int16_t new_y, int16_t new_z, int16_t* imu_filtered)
{

	int32_t tempX = (int32_t) imu_filtered[0];
	int32_t tempY = (int32_t) imu_filtered[1];
	int32_t tempZ = (int32_t) imu_filtered[2];

	tempX += ((int32_t) new_x - tempX) >> ALPHA_SHIFT;
	tempY += ((int32_t) new_y - tempY) >> ALPHA_SHIFT;
	tempZ += ((int32_t) new_z - tempZ) >> ALPHA_SHIFT;

    imu_filtered[0] = tempX;
    imu_filtered[1] = tempY;
    imu_filtered[2] = tempZ;
}


/*
 * find the mean of the last N_SIZE magnitude readings
 * and adds the newest reading
 */
void filter_MagnitudeUpdate(uint32_t new_mag)
{
    /* Remove the reading at the end of the buffer */
    uint32_t oldest = mag_buffer.buffer[mag_buffer.current_index];
    mag_buffer.sum -= oldest;
    mag_buffer.sum_of_sq -= ((uint64_t)oldest * oldest);

    /* Add new value */
    mag_buffer.buffer[mag_buffer.current_index] = new_mag;
    mag_buffer.sum += new_mag;
    mag_buffer.sum_of_sq += ((uint64_t)new_mag * new_mag);

    /* Move index */
    mag_buffer.current_index++;
    mag_buffer.current_index = mag_buffer.current_index % N_SIZE;

    /* Calculate mean */
    mag_buffer.mean = (mag_buffer.sum / N_SIZE);

    /* Calculate scaled variance */
    uint64_t mean_sq = (uint64_t)mag_buffer.mean * mag_buffer.mean;
    mag_buffer.scaled_variance = ((mag_buffer.sum_of_sq - mean_sq) >> VAR_SCALING);
}

// return current scaled variance
uint32_t filter_MagnitudeScaledVarGetter (void)
{
	return mag_buffer.scaled_variance;
}

/* returns most recent reading */
uint32_t filter_MagnitudeCurrentGetter (void) {
    uint8_t last = (mag_buffer.current_index + N_SIZE - 1) % N_SIZE;
    return mag_buffer.buffer[last];
}


/* returns mean of the last N_SIZE magnitide readings */
uint32_t filter_MagnitudeMeanGetter (void)
{
	return mag_buffer.mean;
}

// return sum of magnitudes
uint32_t filter_MagnitudeSumGetter (void)
{
	return mag_buffer.sum;
}


