/*
 * task_hero.h
 *
 *  Created on: Dec 6, 2020
 *      Author: Jimmy O
 */

#include <main.h>

#ifndef TASK_HERO_H_
#define TASK_HERO_H_

typedef enum {
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST,
    NORTH,
    NORTH_EAST
} DIR;


extern TaskHandle_t Task_Hero_Handle;
extern QueueHandle_t Queue_Hero;

// These variables are used with the enemy task as to check if the hero is aiming
// at an enemy, and if the game has reached an end condition
extern DIR HERO_DIR;
extern volatile GAME_STATUS;

/******************************************************************************
 * This function will initialize the wall and hero, and will initialize the queue
 ******************************************************************************/
void Task_Hero_Init(void);

/******************************************************************************
 * This task manages the movement of the hero, bullet count, and core game
 * mechanics. The joystick will update the direction the hero is facing,
 * the button will take away one bullet at a time, and the accelerometer
 * will reload the hero's gun.
 ******************************************************************************/
void Task_Hero(void *pvParameters);

/******************************************************************************
 * This function erases the old hero's position
 ******************************************************************************/
void Clear_Hero(DIR dir);

/******************************************************************************
 * This function draws the new hero's position
 ******************************************************************************/
void Draw_Hero(DIR dir);


#endif /* TASK_HERO_H_ */
