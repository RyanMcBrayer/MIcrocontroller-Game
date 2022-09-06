/*
 * main.c
 *
 *  Created on: Nov 23, 2020
 *      Author: James O'Brien
 */

#include "main.h"

SemaphoreHandle_t Sem_ADC;
QueueHandle_t Queue_Hero;
QueueHandle_t Queue_Bullet;
QueueHandle_t Queue_Enemy;

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // Functions that init all hardware and task queues
    Task_Hero_Init();
    Task_Enemy_Init();
    Task_Bullet_Init();
    Task_light_init();
    Task_Joystick_Init();

    __enable_irq();

    xTaskCreate
     (   Task_Hero,
         "Task_Hero",
         configMINIMAL_STACK_SIZE,
         NULL,
         1,
         &Task_Hero_Handle
     );
    xTaskCreate
     (   Task_Enemy,
         "Task_Enemy",
         configMINIMAL_STACK_SIZE,
         NULL,
         2,
         &Task_Enemy_Handle
     );
    xTaskCreate
     (   Task_Bullet,
         "Task_Bullet",
         configMINIMAL_STACK_SIZE,
         NULL,
         2,
         &Task_Bullet_Handle
     );

    xTaskCreate
    (   Task_Button,
        "Task_Button",
        configMINIMAL_STACK_SIZE,
         NULL,
         1,
         &Task_Button_Handle
    );

    xTaskCreate
    (   Task_Light_Timer,
        "Task_Light_Timer",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &Task_Light_Timer_Handle
    );

    xTaskCreate
    (   Task_Light_Sensor,
        "Task_light_sensor",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &Task_Light_Sensor_Handle
    );

    xTaskCreate
    (   Task_Joystick_Timer,
        "Task_Joystick_Timer",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &Task_Joystick_Timer_Handle
    );

    xTaskCreate
    (   Task_Accel_Bottom_Half,
        "Task_Accel_Bottom_Half",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &Task_Accel_Handle
    );

    xTaskCreate
    (   Task_Joystick_Bottom_Half,
        "Task_Joystick",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &Task_Joystick_Handle
    );

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    while(1){};
    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}
