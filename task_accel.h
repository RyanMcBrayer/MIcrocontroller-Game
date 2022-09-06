/*
 * task_accel.h
 *
 *  Created on: Dec 8, 2020
 *      Author: Jimmy O
 */
#include <main.h>

#ifndef TASK_ACCEL_H_
#define TASK_ACCEL_H_


extern TaskHandle_t Task_Accel_Handle;
extern volatile uint16_t ACCEL_X_DIR;
extern volatile uint16_t ACCEL_Y_DIR;
extern volatile uint16_t ACCEL_Z_DIR;

// reading found through experimental result between a 16 bit value and the ADC returning
// a 12 bit value
#define ACCEL_LOW_THRES     1600
#define ACCEL_HIGH_THRES    2200


/******************************************************************************
* Examines the ADC data from the accelerometer on the MKII
******************************************************************************/
void Task_Accel_Bottom_Half(void *pvParameters);

/******************************************************************************
* Helper function that converts the ADC reading to a state : 0-2 to indicate
* low threshold, middle, and high threshold
******************************************************************************/
uint8_t Update_Accel(uint16_t dir);

#endif /* TASK_ACCEL_H_ */
