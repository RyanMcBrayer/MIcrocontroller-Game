/*
 * opt3001.c
 *
 *  Created on: Dec 5, 2020
 *      Author: Ryan McBrayer
 */

#include <main.h>

TaskHandle_t Task_Light_Sensor_Handle;
TaskHandle_t Task_Light_Timer_Handle;
QueueHandle_t Queue_Enemy;
volatile float LUX = 0;

/******************************************************************************
 * Initialize the OPT3001 light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void Task_light_init(void)
{
    //initialize the i2c
    i2c_init();

    // Set the slave address
    //i2c_set_slave_address(0x44);

    //Set sensor to default config
    i2c_write_16(0x44, CONFIG_REG,  DEFAULT_CONFIG);
}

void Task_Light_Timer(void *pvParameters)
{
    BaseType_t xHigherPriorityTaskWoken;

    while(1) {

        //Send a task notification to Task_Joystick_Bottom_Half
            vTaskNotifyGiveFromISR(Task_Light_Sensor_Handle, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        // Delay 100mS
        vTaskDelay(pdMS_TO_TICKS(100));
    }

}

/******************************************************************************
 * Reads from the temperature sensor
 ******************************************************************************/
float opt3001_read(void)
{
       //create variables to store data from sensor
       uint16_t exponent = 0;
       uint32_t base = 0;
       uint16_t reading;

       //get reading from sensor
       reading = i2c_read_16(0x44, RESULT_REG);
       /*Data returned to reading is in this format:
        * MSB in hex ie. 0xF000 represents an exponent
        * LSB in hex ie. 0x0FFF represents the base result
        */
       //extract exponent and base
       base = reading&0x0FFF;
       exponent = (reading>>12)&0x000F;
       //shift base by exponent to convert base into LUX
       if(exponent == 0){
           //base^-6
           base = base>>6;
       }
       else if(exponent == 1){
           //base^-5
           base = base>>5;
       }
       else if(exponent == 2){
           //base^-4
           base = base>>4;
       }
       else if(exponent == 3){
           //base^-3
           base = base>>3;
       }
       else if(exponent == 4){
           //base^-2
           base = base>>2;
       }
       else if(exponent == 5){
           //base^-1
           base = base>>1;
       }
       else if(exponent == 6){
           //base
       }
       else if(exponent == 7){
           //base * 2
           base = base<<1;
       }
       else if(exponent == 8){
           //base * 4
           base = base<<2;
       }
       else if(exponent == 9){
           //base * 8
           base = base<<3;
       }
       else if(exponent == 10){
           //base * 16
           base = base<<4;
       }
       else if(exponent == 11){
           //base * 32
           base = base<<5;
       }
       //return base in units of LUX
       return base;

}
/*
 * This task reades a value in LUX from the sensor and
 * sends a color to the queue based on the LUX reading
 */
void Task_Light_Sensor(void *pvParameters) {
    while (1){

        //wait until task notifications is received
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        //read from sensor
        LUX = opt3001_read();
        float reading = LUX;
        uint16_t color;
        if(reading < 20){
            color = LCD_COLOR_GRAY;
        }
        else{
            color = LCD_COLOR_RED;
        }
        xQueueSend(Queue_Enemy, &color, pdMS_TO_TICKS(10));
    }
}

