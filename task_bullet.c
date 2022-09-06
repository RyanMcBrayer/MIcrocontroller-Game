/*
 * task_bullet.c
 *
 *  Created on: Dec 7, 2020
 *      Author: Jimmy O, Ryan M
 */
#include <main.h>

TaskHandle_t Task_Bullet_Handle;
QueueHandle_t Queue_Bullet;



void Task_Bullet_Init(void) {

    // initializes queue
    Queue_Bullet = xQueueCreate(2,sizeof(uint8_t));
    Draw_All_Bullets();

    // configure button as input
    P5->DIR &= ~BIT1;

    // set buzzer to be a GPIO output pin
    P2->DIR |= BIT7;

    // the TimerA0 PWM controller will control the buzzer on the MKII
    // P2.7 <--> TA0.4
    P2->SEL0 |= BIT7;
    P2->SEL1 &= BIT7;

    // turn off TA0
    TIMER_A0->CTL = 0;

    // set period of the timer
    TIMER_A0->CCR[0] = 59999;

    // configure buzzer duty cycle to 50%
    TIMER_A0->CCR[4] = 59999/2;

    // configure TA0.4 for reset/set mode
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;

    // select the master clock as the timer source
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;
}


void Task_Bullet(void *pvParameters) {
    // max bullet count is 5
    uint8_t count = 5;
    uint8_t change = 0;
    // message that will be sent to enemy queue
    uint16_t color = LCD_COLOR_BLACK;

    while(1) {
        // waits for message from accelerometer or button
        xQueueReceive(Queue_Bullet, &change, portMAX_DELAY);

        // if message is 1, message is from button
        if (change == 1 && count > 0) {\
            // decrement count and set bullet change to 0
            count--;
            change--;

            // erases bullet based on count
            Clear_Bullet(count);

            // gun sound
            Gun_Sound();

            // sends message to enemy saying a bullet has been fired
            xQueueSendToFront(Queue_Enemy, &color, portMAX_DELAY);
        }

        // message is from accelerometer
        else if (change == 5) {
            // reset count and bullet change
            count = 5;
            change = 0;

            // draws all bullets showing reload
            Draw_All_Bullets();
        }

        // waits for other tasks to perform
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void Clear_Bullet(uint8_t count) {
    // erases bullet that was shot base on how many bullet play currently has
    switch (count) {
    case 4: {
        lcd_draw_image(
                57,
                72,
                bulletWidthPixels,
                bulletHeightPixels,
                bulletBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
        break;
    }
    case 3: {
            lcd_draw_image(
                61,
                72,
                bulletWidthPixels,
                bulletHeightPixels,
                bulletBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
            break;
    }
    case 2: {
            lcd_draw_image(
                65,
                72,
                bulletWidthPixels,
                bulletHeightPixels,
                bulletBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
            break;
    }
    case 1: {
            lcd_draw_image(
                69,
                72,
                bulletWidthPixels,
                bulletHeightPixels,
                bulletBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
            break;
    }
    case 0: {
            lcd_draw_image(
                73,
                72,
                bulletWidthPixels,
                bulletHeightPixels,
                bulletBitmaps,
                LCD_COLOR_BLACK,
                LCD_COLOR_BLACK
                );
            break;
    }
    default: {
        break;
    }
    }
}

void Draw_All_Bullets() {
    // draws all bullets showing full ammo, bullets are inside wall
    lcd_draw_image(
            57,
            72,
            bulletWidthPixels,
            bulletHeightPixels,
            bulletBitmaps,
            LCD_COLOR_GRAY,
            LCD_COLOR_BLACK
            );
    lcd_draw_image(
            61,
            72,
            bulletWidthPixels,
            bulletHeightPixels,
            bulletBitmaps,
            LCD_COLOR_GRAY,
            LCD_COLOR_BLACK
            );
    lcd_draw_image(
            65,
            72,
            bulletWidthPixels,
            bulletHeightPixels,
            bulletBitmaps,
            LCD_COLOR_GRAY,
            LCD_COLOR_BLACK
            );
    lcd_draw_image(
            69,
            72,
            bulletWidthPixels,
            bulletHeightPixels,
            bulletBitmaps,
            LCD_COLOR_GRAY,
            LCD_COLOR_BLACK
            );
    lcd_draw_image(
            73,
            72,
            bulletWidthPixels,
            bulletHeightPixels,
            bulletBitmaps,
            LCD_COLOR_GRAY,
            LCD_COLOR_BLACK
            );
}


void Gun_Sound() {
    // clear current mode control bits
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

    // set current mode to up and clear the current count
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;

    // leave sound on for 500 ms
    // stop timer
    TIMER32_1->CONTROL = 0;

    // set timer to oneshot mode, prescale 8, 16 bit (by not turning on size)
    TIMER32_1->CONTROL = TIMER32_CONTROL_ONESHOT |  TIMER32_CONTROL_PRESCALE_2;

    // loads value based on .2 sec * MCU freq / 256 (prescale of 8)
    TIMER32_1->LOAD = 18750;

    // start timer
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

    // waits for timer to finish
    while (TIMER32_1->VALUE != 0) {
    }

    // clear current mode control bits
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}
