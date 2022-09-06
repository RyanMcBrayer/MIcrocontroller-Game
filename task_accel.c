/*
 * task_accel.c
 *
 *  Created on: Dec 8, 2020
 *      Author: Jimmy O, Ryan M
 */

#include <main.h>

TaskHandle_t Task_Accel_Handle;

volatile uint16_t ACCEL_X_DIR = 0;
volatile uint16_t ACCEL_Y_DIR = 0;
volatile uint16_t ACCEL_Z_DIR = 0;


void Task_Accel_Bottom_Half(void *pvParameters) {
    // variables that keep track of accelerometer movement
    uint8_t prev_x = 1;
    uint8_t prev_y = 1;
    uint8_t prev_z = 1;
    uint8_t x;
    uint8_t y;
    uint8_t z;

    // number of movements
    uint8_t movement_count = 0;

    // message that will be sent to bullet queue
    uint8_t change = 5;

    while(1) {
        //Wait until we get a task notification from the ADC14 ISR
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // calls helper function to get regions of accelerometer
        // 0 being below minimum threshold (x-left, y-down, z-right side up)
        // 1 being in between or centered (x-center, y-center, z-"0 gravity")
        // 2 being above threshold (x-right, y-up, z-upside down)
        x = Update_Accel(ACCEL_X_DIR);
        y = Update_Accel(ACCEL_Y_DIR);
        z = Update_Accel(ACCEL_Z_DIR);

        // if any change has happened, movement counter is incremented and direction is updated
        if (prev_x != x) {
            movement_count++;
            prev_x = x;
        }
        else if (prev_y != y) {
            movement_count++;
            prev_y = y;
        }
        else if (prev_z != z) {
            movement_count++;
            prev_z = z;
        }

        // every 4 movements will reload the hero
        movement_count = movement_count % 4;
        if (movement_count == 3) {
            // sends message to bullet queue to refill ammo
            xQueueSend(Queue_Bullet, &change, portMAX_DELAY);
        }
    }
}


uint8_t Update_Accel(uint16_t dir) {
    // takes analog reading, and comparing it to experimental values to return direction
    if (dir < ACCEL_LOW_THRES) {
        return 0;
    }
    else if (dir > ACCEL_HIGH_THRES) {
        return 2;
    }
    else {
        return 1;
    }

}
