/*
 * filter.h
 *
 *  Created on: May 6, 2025
 *      Author: tli101
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

#include "task_read_imu.h"
#include <stdint.h>

#define N_SIZE 64 // Size of magnitude/variance buffer

void filter_Init (void);

void filter_IIR (int16_t new_x, int16_t new_y, int16_t new_z, int16_t* imu_filtered);

void filter_MagnitudeUpdate (uint32_t new_mag);
uint32_t filter_MagnitudeCurrentGetter (void);
uint32_t filter_MagnitudeScaledVarGetter (void);
uint32_t filter_MagnitudeMeanGetter (void);
uint32_t filter_MagnitudeSumGetter (void);
#endif /* INC_FILTER_H_ */
