/*
 * task_button.c
 *
 *  Created on: Dec 7, 2020
 *      Author: Jimmy O
 */
#include <main.h>

TaskHandle_t Task_Button_Handle;

void Task_Button(void *pvParameters)
{
    // variable sent to bullet task, is set to 1 if button press is detected
    uint8_t change = 0;

    // variable for de-bouncing S1
    uint8_t debounce_state = 0x00;
    while(1)
    {

        // Shift the de-bounce variable to the left
        debounce_state = debounce_state << 1;

        // If S1 is being pressed, set the LSBit of debounce_state to a 1;
        if ((P5->IN & BIT1) == 0) {
            debounce_state |= 0x01;

            // message is updated to show 1 bullet was fired
            change = 1;
        }

        // If the de-bounce variable is equal to 0x7F, send the queue message
        if (debounce_state == 0x7F) {

            xQueueSend(Queue_Bullet, &change, portMAX_DELAY);
            change = 0;
        }

        // Delay for 10mS using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}

