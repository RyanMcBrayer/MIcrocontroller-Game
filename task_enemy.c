/*
 * enemy.c
 *
 *  Created on: Dec 7, 2020
 *      Author: Jimmy O. Ryan M
 */

#include <main.h>
TaskHandle_t Task_Enemy_Handle;
QueueHandle_t Queue_Enemy;
LEVEL level;
void Task_Enemy_Init() {
    // initialize queue
    Queue_Enemy = xQueueCreate(2,sizeof(uint16_t));

    // fills the level with a sequence from an online random number generator
    // further work would be to replace this array with a random number generator is C
    level = (LEVEL){7, 6, 4, 6, 2, 1, 7, 5, 3, 3, 1, 3, 0, 5, 7, 0, 2, 4, 3, 4, 6, 2, 5, 0, 3};
}

void Task_Enemy(void *pvParameters)
{
    uint16_t color;
    uint8_t step = 4;
    uint8_t count = 0;
    uint8_t dir = level.level[count];
    while(1) {
        // waits for a message to be sent from the light sensor task, or the bullet task
        xQueueReceive(Queue_Enemy, &color, portMAX_DELAY);

        // if the message is the color black, it came from the bullet task, will check if
        // the hero is facing the enemy
        if (color == LCD_COLOR_BLACK && HERO_DIR == dir) {

            // erases the current enemy
            lcd_draw_image(
                    Get_Enemy_X_Position(dir, step),
                    Get_Enemy_Y_Position(dir, step),
                    enemyWidthPixels,
                    enemyHeightPixels,
                    enemyBitmaps,
                    LCD_COLOR_BLACK,
                    LCD_COLOR_BLACK
                    );
            // increments enemy count, steps step to 4 (will be decremented soon)
            count++;
            step = 4;

            // loads new direction, sets color to gray as enemies are far away a difficult to see
            dir = level.level[count];
            color = LCD_COLOR_GRAY;
        }

        // message came from light sensor, will erase the old enemy position to be drawn with message colorS
        else {
            lcd_draw_image(
                    Get_Enemy_X_Position(dir, step),
                    Get_Enemy_Y_Position(dir, step),
                    enemyWidthPixels,
                    enemyHeightPixels,
                    enemyBitmaps,
                    LCD_COLOR_BLACK,
                    LCD_COLOR_BLACK
                    );
        }

        // enemy moves closer
        step--;

        // checks if the game is over
        if (count == 25) {
            // end game win
            GAME_STATUS = 2;
        }
        // Draws the enemy, then checks if game is over to show enemy reaching wall
        else {
            lcd_draw_image(
                    Get_Enemy_X_Position(dir, step),
                    Get_Enemy_Y_Position(dir, step),
                    enemyWidthPixels,
                    enemyHeightPixels,
                    enemyBitmaps,
                    color,
                    LCD_COLOR_BLACK
                    );

            if (step == 0) {
                // end game lose
                GAME_STATUS = 1;
            }
        }

        // wait a long time to space enemy movements
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

uint8_t Get_Enemy_X_Position(DIR dir, uint8_t step) {
    // if enemy has no direction, position is set to off screen
    uint8_t result = 135;

    // gets x position based on direction and step
    switch (dir) {
    // enemy is coming from east (left)
    case 7 :{}
    case 0 :{}
    case 1 :{
        switch (step) {
        case 3: {
            result = EAST_3;
            break;
        }
        case 2: {
            result = EAST_2;
            break;
        }
        case 1: {
            result = EAST_1;
            break;
        }
        default: {
            result = EAST_0;
            break;
        }
        }
        break;
    }

    // enemy is coming from west (right)
    case 3 :{}
    case 4 :{}
    case 5 :{
        switch (step) {
        case 3: {
            result = WEST_3;
            break;
        }
        case 2: {
            result = WEST_2;
            break;
        }
        case 1: {
            result = WEST_1;
            break;
        }
        default: {
            result = WEST_0;
            break;
        }
        }
        break;
    }

    // enemy is coming from north or south (no x component)
    case 6 :{}
    case 2 :{
        result = MIDDLE;
        break;
    }
    default :{
        break;
    }
    }
    return result;
}


uint8_t Get_Enemy_Y_Position(DIR dir, uint8_t step) {
    // if enemy has no direction, position is set to off screen
    uint8_t result = 135;

    // gets y position based on direction and step
    switch (dir) {
    // enemy is coming from south (bottom)
    case 1 :{}
    case 2 :{}
    case 3 :{
        switch (step) {
        case 3: {
            result = SOUTH_3;
            break;
        }
        case 2: {
            result = SOUTH_2;
            break;
        }
        case 1: {
            result = SOUTH_1;
            break;
        }
        default: {
            result = SOUTH_0;
            break;
        }
        }
        break;
    }

    // enemy is coming from north (top)
    case 5 :{}
    case 6 :{}
    case 7 :{
        switch (step) {
        case 3: {
            result = NORTH_3;
            break;
        }
        case 2: {
            result = NORTH_2;
            break;
        }
        case 1: {
            result = NORTH_1;
            break;
        }
        default: {
            result = NORTH_0;
            break;
        }
        }
        break;
    }

    // enemy is coming from east or west (no y component)
    case 0 :{}
    case 4 :{
        result = MIDDLE;
        break;
    }
    default :{
        break;
    }
    }
    return result;
}
