/*
 * task_bullet.h
 *
 *  Created on: Dec 7, 2020
 *      Author: Jimmy O
 */
#include <main.h>

#ifndef TASK_BULLET_H_
#define TASK_BULLET_H_




extern TaskHandle_t Task_Bullet_Handle;
extern QueueHandle_t Queue_Bullet;



/******************************************************************************
 * This function will draw the bullets, initialize the bullet queue,
 * set S1 button on MKII to input, and sets up timer A for the buzzer
 ******************************************************************************/
void Task_Bullet_Init(void);

/******************************************************************************
 * This task manages bullet count and bullet display. Interacting with the
 * accelerometer and button, a shake refill your bullets, and a button press
 * shoots your gun, removing a bullet
 ******************************************************************************/
void Task_Bullet(void *pvParameters);

/******************************************************************************
 * This function Draws all the bullets when a reload occurs
 ******************************************************************************/
void Draw_All_Bullets(void);

/******************************************************************************
 * This function removes one bullet at a time as it is shot
 ******************************************************************************/
void Clear_Bullet(uint8_t count);

/******************************************************************************
 * This function makes the gun shot sound
 ******************************************************************************/
void Gun_Sound(void);

#endif /* TASK_BULLET_H_ */
