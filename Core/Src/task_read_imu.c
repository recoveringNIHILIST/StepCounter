/*
 * task_read_imu.c
 *
 * Read IMU data
 * Scale by measured offsets
 * Detect peaks using peak_detection.c
 *
 * Created on: May 6, 2025
 * Author: T. Linton, J. Legg
 */

#include "task_read_imu.h"
#include "imu_lsm6ds.h"
#include "filter.h"
#include "peak_detection.h"

#include <stdint.h>

#define X_OFFSET 245
#define Y_OFFSET 140
#define Z_OFFSET (-300)

#define BIT_SHIFT_SCALE 10 // scale magnitude by 2^10 = 1024

static int16_t raw_x_acc;
static int16_t raw_y_acc;
static int16_t raw_z_acc;

static int16_t imu_filtered[3];
static uint32_t acc_mag;


/* Initialise filter and imu sensor settings */
void imu_Init (void)
{
	filter_Init();
	imu_lsm6ds_write_byte(CTRL1_XL, CTRL1_XL_HIGH_PERFORMANCE);
}


/* Calculate and scale acceleration magnitude from filtered data */
void imu_CalcAccMagnitude (void)
{
	acc_mag	= ((uint32_t) imu_filtered[0] * imu_filtered[0]) 	// = x_acc_filtered^2
			+ ((uint32_t) imu_filtered[1] * imu_filtered[1])	// + y_acc_filtered^2
			+ ((uint32_t) imu_filtered[2] * imu_filtered[2]);	// + z_acc_filtered^2

	// Scale to avoid overflow //
	acc_mag = acc_mag >> BIT_SHIFT_SCALE;
}

/* Read raw acceleration data from imu */
void imu_ReadRawData (void)
{
	uint8_t acc_x_low 	= imu_lsm6ds_read_byte (OUTX_L_XL);
	uint8_t acc_x_high 	= imu_lsm6ds_read_byte (OUTX_H_XL);
	uint8_t acc_y_low 	= imu_lsm6ds_read_byte (OUTY_L_XL);
	uint8_t acc_y_high 	= imu_lsm6ds_read_byte (OUTY_H_XL);
	uint8_t acc_z_low 	= imu_lsm6ds_read_byte (OUTZ_L_XL);
	uint8_t acc_z_high 	= imu_lsm6ds_read_byte (OUTZ_H_XL);

	raw_x_acc = (int16_t) ((acc_x_high << 8) | acc_x_low);
	raw_y_acc = (int16_t) ((acc_y_high << 8) | acc_y_low);
	raw_z_acc = (int16_t) ((acc_z_high << 8) | acc_z_low);
}


/* apply sensor offsets to raw data */
void imu_ScaleRawData (void)
{
	raw_x_acc += X_OFFSET;
	raw_y_acc += Y_OFFSET;
	raw_z_acc += Z_OFFSET;
}


/* read, scale, filter, update magnitude, and detect peaks */
void imu_Execute (void)
{
	imu_ReadRawData ();
	imu_ScaleRawData ();
	filter_IIR (raw_x_acc, raw_y_acc, raw_z_acc, imu_filtered);
	imu_CalcAccMagnitude ();
	filter_MagnitudeUpdate (acc_mag); // finds mean of previous magnitudes
	peakDetection_Execute ();
}


int16_t imu_xAccGetter (void)
{
	return raw_x_acc;
}


int16_t imu_xFilteredGetter (void)
{
	return imu_filtered[0];
}


int16_t imu_yAccGetter (void)
{
	return raw_y_acc;
}


int16_t imu_yFilteredGetter (void)
{
	return imu_filtered[1];
}



int16_t imu_zAccGetter (void)
{
	return raw_z_acc;
}


int16_t imu_zFilteredGetter (void)
{
	return imu_filtered[2];
}




