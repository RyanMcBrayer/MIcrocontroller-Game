/*
 * task_led.c
 *
 *  Created on: Nov 23, 2020
 *      Author: James O'Brien, Ryan McBrayer
 */

#include <main.h>


 TaskHandle_t Task_Joystick_Handle;
 TaskHandle_t Task_Joystick_Timer_Handle;

 volatile uint32_t JOYSTICK_X_DIR = 0;
 volatile uint32_t JOYSTICK_Y_DIR = 0;

/******************************************************************************
* This function will run the same configuration code that you would have
* written for HW02.
******************************************************************************/
 void Task_Joystick_Init(void)
 {
     // Configure the joystick X direction as an analog input pin.
     P6->SEL0 |= BIT0;
     P6->SEL1 |= BIT0;

     // Configure the joystick Y direction as an analog input pin.
     P4->SEL0 |= BIT4;
     P4->SEL1 |= BIT4;

     // Configure the accelerometer X direction as an analog input pin.
     P6->SEL0 |= BIT1;
     P6->SEL1 |= BIT1;

     // Configure the accelerometer Y direction as an analog input pin.
     P4->SEL0 |= BIT0;
     P4->SEL1 |= BIT0;

     // Configure the accelerometer Z direction as an analog input pin.
     P4->SEL0 |= BIT2;
     P4->SEL1 |= BIT2;

     // Configure CTL0 to sample 16-times in pulsed sample mode.
     // NEW -- Indicate that this is a sequence of samples.
     ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1;

     // Configure CTL1 to return 12-bit values
     ADC14->CTL1 = ADC14_CTL1_RES_2;         // Use sampling timer, 12-bit conversion results

     // Associate the X direction analog signal with MEM[0]
     ADC14->MCTL[0] |= ADC14_MCTLN_INCH_15;

     // Associate the Y direction analog signal with MEM[1]
     ADC14->MCTL[1] |= ADC14_MCTLN_INCH_9;

     // Associate the X accelerometer analog signal with MEM[2]
     ADC14->MCTL[2] |= ADC14_MCTLN_INCH_14;

     // Associate the Y accelerometer analog signal with MEM[3]
     ADC14->MCTL[3] |= ADC14_MCTLN_INCH_13;

     // Associate the Z accelerometer analog signal with MEM[4]
     // end of a sequence.
     ADC14->MCTL[4] |= ADC14_MCTLN_INCH_11 | ADC14_MCTLN_EOS;

     // Enable interrupts AFTER a value is written into MEM[1].
     ADC14->IER0 |= ADC14_IER0_IE1 ;

     // Enable ADC Interrupt
     NVIC_EnableIRQ(ADC14_IRQn);

     // Interrupt priorities must NOT be set to 0.  It is preferred if they are set to a value that is > than 1 too.
     NVIC_SetPriority(ADC14_IRQn, 3);

     // Turn ADC ON
     ADC14->CTL0 |= ADC14_CTL0_ON;

 }

 /******************************************************************************
 * Used to start an ADC14 Conversion
 ******************************************************************************/
 void Task_Joystick_Timer(void *pvParameters)
 {
     while(1) {
         // Start the ADC conversion
         ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

         //Delay 50mS
         vTaskDelay(pdMS_TO_TICKS(50));
     }

 }


/******************************************************************************
* Bottom Half Task.  Examines the ADC data from the joy stick on the MKII
******************************************************************************/
void Task_Joystick_Bottom_Half(void *pvParameters)
{
    // Direction that will be sent to the Queue_Hero
    DIR dir;
    while(1) {

        //Wait until we get a task notification from the ADC14 ISR
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);



        // Joystick X left
        if(JOYSTICK_X_DIR < VOLT_0P85)
        {
            // Joystick Y up
            if (JOYSTICK_Y_DIR > VOLT_2P50) {
                dir = NORTH_WEST;
            }
            // Joystick Y down
            else if (JOYSTICK_Y_DIR < VOLT_0P85) {
                dir = SOUTH_WEST;
            }
            // Joystick Y middle
            else {
                dir = WEST;
            }

        }

        // Joystick X right
        else if(JOYSTICK_X_DIR > VOLT_2P50)
        {
            // Joystick Y up
            if (JOYSTICK_Y_DIR > VOLT_2P50) {
                dir = NORTH_EAST;
            }
            // Joystick Y down
            else if (JOYSTICK_Y_DIR < VOLT_0P85) {
                dir = SOUTH_EAST;
            }
            // Joystick Y middle
            else {
                dir = EAST;
            }

        }
        // Joystick Y up
        else if (JOYSTICK_Y_DIR > VOLT_2P50) {
            dir = NORTH;
        }
        // Joystick Y down
        else if (JOYSTICK_Y_DIR < VOLT_0P85) {
            dir = SOUTH;
        }

        // Sends the task to the queue
        xQueueSend(Queue_Hero, &dir, portMAX_DELAY);
    }
}


/******************************************************************************
* Top Half of ADC14 Handler.
******************************************************************************/
void ADC14_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken;

    JOYSTICK_X_DIR = ADC14->MEM[0]; // Read the value and clear the interrupt
    JOYSTICK_Y_DIR = ADC14->MEM[1]; // Read the value and clear the interrupt
    ACCEL_X_DIR = ADC14->MEM[2]; // Read the value and clear the interrupt
    ACCEL_Y_DIR = ADC14->MEM[3]; // Read the value and clear the interrupt
    ACCEL_Z_DIR = ADC14->MEM[4]; // Read the value and clear the interrupt


    //Send a task notification to Task_Joystick_Bottom_Half
    vTaskNotifyGiveFromISR(Task_Joystick_Handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    //Send a task notification to Task_Accel_Bottom_Half
    vTaskNotifyGiveFromISR(Task_Accel_Handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}



