/*
 * task_hero.c
 *
 *  Created on: Dec 6, 2020
 *      Author: Jimmy O, Ryan M
 */

#include <main.h>

TaskHandle_t Task_Hero_Handle;
QueueHandle_t Queue_Hero;
DIR HERO_DIR;
volatile GAME_STATUS;


void Task_Hero_Init(void)
{
    // initialize queue
    Queue_Hero = xQueueCreate(2,sizeof(DIR));

    // initialize the LCD
    Crystalfontz128x128_Init();

    // draw wall
    lcd_draw_image(
            66,
            66,
            wallWidthPixels,
            wallHeightPixels,
            wallBitmaps,
            LCD_COLOR_BROWN,
            LCD_COLOR_BLACK
            );
    // draw hero starting position (east)
    lcd_draw_image(
            66,
            63,
            pos_eastWidthPixels,
            pos_eastHeightPixels,
            pos_eastBitmaps,
            LCD_COLOR_WHITE,
            LCD_COLOR_BLACK
            );
}

void Task_Hero(void *pvParameters)
{
    DIR dir;
    DIR prev_dir;

    // 0 if game is being played, 1 if the player lost, 2 if the player won
    GAME_STATUS = 0;

    while(1) {
        // waits to recieve a queue from the joystick bottom half task
        xQueueReceive(Queue_Hero, &dir, portMAX_DELAY);

        // if the game is being played and the hero changed directions, the old
        // direction is erased, the new on is drawn, and the global and local vars
        // are updated
        if (GAME_STATUS == 0) {
            if (dir != prev_dir) {
                Clear_Hero(prev_dir);
                Draw_Hero(dir);
                HERO_DIR = dir;
                prev_dir = dir;
            }
        }

        // end game condition is detected, will draw the win or lose screen
        else {
            if (GAME_STATUS == 1) {
                lcd_draw_image(
                        66,
                        66,
                        loseWidthPixels,
                        loseHeightPixels,
                        loseBitmaps,
                        LCD_COLOR_BLACK,
                        LCD_COLOR_BLACK
                        );
                lcd_draw_image(
                        66,
                        66,
                        loseWidthPixels,
                        loseHeightPixels,
                        loseBitmaps,
                        LCD_COLOR_WHITE,
                        LCD_COLOR_BLACK
                        );
            }
            else {
                lcd_draw_image(
                        66,
                        66,
                        winWidthPixels,
                        winHeightPixels,
                        winBitmaps,
                        LCD_COLOR_BLACK,
                        LCD_COLOR_BLACK
                        );
                lcd_draw_image(
                        66,
                        63,
                        winWidthPixels,
                        winHeightPixels,
                        winBitmaps,
                        LCD_COLOR_WHITE,
                        LCD_COLOR_BLACK
                        );
            }

            // ends the scheduler as the game is over
            vTaskEndScheduler();
        }

        // if scheduler is still active, will pause this task to allow other tasks to run
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void Clear_Hero(DIR dir) {
    // checks the direction and erases the old image
    switch (dir) {
    case 0: {
        lcd_draw_image(
                66,
                63,
                pos_eastWidthPixels,
                pos_eastHeightPixels,
                pos_eastBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 1: {
        lcd_draw_image(
                66,
                63,
                pos_south_eastWidthPixels,
                pos_south_eastHeightPixels,
                pos_south_eastBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 2: {
        lcd_draw_image(
                66,
                63,
                pos_southWidthPixels,
                pos_southHeightPixels,
                pos_southBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 3: {
        lcd_draw_image(
                66,
                63,
                pos_south_westWidthPixels,
                pos_south_westHeightPixels,
                pos_south_westBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 4: {
        lcd_draw_image(
                66,
                63,
                pos_westWidthPixels,
                pos_westHeightPixels,
                pos_westBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 5: {
        lcd_draw_image(
                66,
                63,
                pos_north_westWidthPixels,
                pos_north_westHeightPixels,
                pos_north_westBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 6: {
        lcd_draw_image(
                66,
                63,
                pos_northWidthPixels,
                pos_northHeightPixels,
                pos_northBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 7: {
        lcd_draw_image(
                66,
                63,
                pos_north_eastWidthPixels,
                pos_north_eastHeightPixels,
                pos_north_eastBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
        break;
    }
    default :{
        break;
    }
    }
}

void Draw_Hero(DIR dir) {
    // checks the new direction and draws the new image
    switch (dir) {
    case 0: {
        lcd_draw_image(
                66,
                63,
                pos_eastWidthPixels,
                pos_eastHeightPixels,
                pos_eastBitmaps,
                LCD_COLOR_WHITE,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 1: {
        lcd_draw_image(
                66,
                63,
                pos_south_eastWidthPixels,
                pos_south_eastHeightPixels,
                pos_south_eastBitmaps,
                LCD_COLOR_WHITE,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 2: {
        lcd_draw_image(
                66,
                63,
                pos_southWidthPixels,
                pos_southHeightPixels,
                pos_southBitmaps,
                LCD_COLOR_WHITE,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 3: {
        lcd_draw_image(
                66,
                63,
                pos_south_westWidthPixels,
                pos_south_westHeightPixels,
                pos_south_westBitmaps,
                LCD_COLOR_WHITE,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 4: {
        lcd_draw_image(
                66,
                63,
                pos_westWidthPixels,
                pos_westHeightPixels,
                pos_westBitmaps,
                LCD_COLOR_WHITE,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 5: {
        lcd_draw_image(
                66,
                63,
                pos_north_westWidthPixels,
                pos_north_westHeightPixels,
                pos_north_westBitmaps,
                LCD_COLOR_WHITE,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 6: {
        lcd_draw_image(
                66,
                63,
                pos_northWidthPixels,
                pos_northHeightPixels,
                pos_northBitmaps,
                LCD_COLOR_WHITE,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 7: {
        lcd_draw_image(
                66,
                63,
                pos_north_eastWidthPixels,
                pos_north_eastHeightPixels,
                pos_north_eastBitmaps,
                LCD_COLOR_WHITE,
                LCD_COLOR_BLACK
                );
        break;
    }
    default :{
        break;
    }
    }
}

