/*
 * enemy.h
 *
 *  Created on: Dec 7, 2020
 *      Author: Jimmy O
 */
#include <main.h>


#ifndef ENEMY_H_
#define ENEMY_H_

// positions of enemy on LCD screen
// counting down, 3 is the farthest and 0 is the enemy reaching the wall
#define NORTH_3      6
#define NORTH_2      27
#define NORTH_1      48
#define NORTH_0      52
#define WEST_3       6
#define WEST_2       27
#define WEST_1       48
#define WEST_0       52
#define SOUTH_3      126
#define SOUTH_2      107
#define SOUTH_1      84
#define SOUTH_0      80
#define EAST_3       126
#define EAST_2       107
#define EAST_1       84
#define EAST_0       80
#define MIDDLE       66


extern TaskHandle_t Task_Enemy_Handle;
extern QueueHandle_t Queue_Enemy;

// data structure to allow the level to be fill after initialization
typedef struct
{
    uint8_t level[25];
} LEVEL;

/******************************************************************************
 * This function will initialize the enemy queue and the level (which direction
 * the enemies will come).
 ******************************************************************************/
void Task_Enemy_Init(void);

/******************************************************************************
 * This task manages the movement of the enemies and their status. The queue
 * will receive a message from the bullet task if a button press occurs, then the
 * hero's direction will be checked to see if the enemy dies. If so, a new enemy
 * will spawn in until the game is over (25 enemies). We used random online number
 * generator for the order
 ******************************************************************************/
void Task_Enemy(void *pvParameters);

/******************************************************************************
 * This function gets the X coordinate of the enemy base on the direction they
 * are coming from and how many steps away from the wall they are
 ******************************************************************************/
uint8_t Get_Enemy_X_Position(DIR dir, uint8_t step);

/******************************************************************************
 * This function gets the Y coordinate of the enemy base on the direction they
 * are coming from and how many steps away from the wall they are
 ******************************************************************************/
uint8_t Get_Enemy_Y_Position(DIR dir, uint8_t step);

#endif /* ENEMY_H_ */
