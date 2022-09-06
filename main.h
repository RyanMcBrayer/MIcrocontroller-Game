/*
 * main.h
 *
 *  Created on: Oct 19, 2020
 *      Author: Joe Krachey
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* Includes */
#include <task_joystick.h>
#include <task_hero.h>
#include <task_enemy.h>
#include <task_bullet.h>
#include <task_button.h>
#include <task_accel.h>
#include <task_light.h>
#include <i2c.h>
#include <lcd.h>

/* Custom pictures */
#include <Pics/bullet.h>
#include <Pics/wall.h>
#include <Pics/enemy.h>
#include <Pics/pos_north.h>
#include <Pics/pos_south.h>
#include <Pics/pos_east.h>
#include <Pics/pos_west.h>
#include <Pics/pos_north_west.h>
#include <Pics/pos_north_east.h>
#include <Pics/pos_south_west.h>
#include <Pics/pos_south_east.h>
#include <Pics/win.h>
#include <Pics/lose.h>

extern SemaphoreHandle_t;


#endif /* MAIN_H_ */
