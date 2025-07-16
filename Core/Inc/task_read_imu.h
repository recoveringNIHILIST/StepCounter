/*
 * task_read_imu.h
 *
 *  Created on: May 6, 2025
 *      Author: tli101
 */

#ifndef INC_TASK_READ_IMU_H_
#define INC_TASK_READ_IMU_H_

#include <stdint.h>

void imu_Init (void);
void imu_Execute (void);

int16_t imu_xAccGetter (void);
int16_t imu_xFilteredGetter (void);
int16_t imu_yAccGetter (void);
int16_t imu_yFilteredGetter (void);
int16_t imu_zAccGetter (void);
int16_t imu_zFilteredGetter (void);

#endif /* INC_TASK_READ_IMU_H_ */
