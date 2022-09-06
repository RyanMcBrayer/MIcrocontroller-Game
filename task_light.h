/*
 * task_ligh.h
 *
 *  Created on: Dec 9, 2020
 *      Author: Jimmy O
 */
#include <main.h>

#ifndef TASK_LIGH_H_
#define TASK_LIGH_H_

extern TaskHandle_t Task_Light_Sensor_Handle;
extern TaskHandle_t Task_Light_Timer_Handle;

#define I2C_OPT_SLAVE_ADDR   0x44
#define OPT_INTERRUPT_PIN 11
#define RESULT_REG 0x00
#define CONFIG_REG 0x01
#define LOWLIMIT_REG 0x02
#define HIGHLIMIT_REG 0x03
#define MANUFACTUREID_REG 0x7E
#define DEVICEID_REG 0x7F

#define DEFAULT_CONFIG 0xCC10 // 800ms
#define DEFAULT_CONFIG_100 0xC410 // 100ms

void Task_light_init(void);

void Task_Light_Timer(void *pvParameters);

float opt3001_read(void);

void Task_Light_Sensor(void *pvParameters);



#endif /* TASK_LIGH_H_ */
