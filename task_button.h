/*
 * task_button.h
 *
 *  Created on: Dec 7, 2020
 *      Author: Jimmy O
 */
#include <main.h>

#ifndef TASK_BUTTON_H_
#define TASK_BUTTON_H_

extern TaskHandle_t Task_Button_Handle;


/******************************************************************************
 * De-bounce switch S1.  If is has been pressed, a queue is sent to the bullet
 * task to fire the gun, bullet task sends queue to enemy to see if the bullet
 * hit.
 *****************************************************************************/
void Task_Button(void *pvParameters);


#endif /* TASK_BUTTON_H_ */
