/***********************Headers***********************/
#include <stdint.h>
 #include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/systick.h"
#include "Lcd.h"
/***********************Variables***********************/
volatile uint8_t val=0,hour=21,min=00,sec=0;            //  Cureent time value defined
volatile char arr[8];
/***********************Function Declarations***********************/
void Init(void);
void InitSystick(void);
void main(void){

    Init();
    LcdInit();
    sprintf(arr, "%02d:%02d:%02d", hour, min, sec);
    LcdGotoXY(1, 5);
    LcdSendString(arr);
    InitSystick();
    while(1){

    }
}

void SysTickIRQHandler(void){
    sec++;
    if(sec>59){
        sec=0;
        min++;
    }
    if(min>59){
        min=0;
        hour++;
    }
    if(hour>23){
        hour=0;
    }
    sprintf(arr,"%02d:%02d:%02d",hour,min,sec);
    LcdGotoXY(1, 5);
    LcdSendString(arr);

}

void Init(void){
    /**********************************  clock settings*********************************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);          //  80Mhz system clock
    uint32_t clk=SysCtlClockGet();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);    //  Portb clock enable for lcd pins
    /********************************** GPIO settings***********************************************************/
    GPIOPinTypeGPIOOutput(LCD_PORT,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);  //  Lcd pins
}
void InitSystick(void){
    NVIC_ST_RELOAD_R = 4000000; // For 1 sec
    NVIC_ST_CURRENT_R=0;        // Reset current value
    NVIC_ST_CTRL_R =3;          // Enable,Interrupt enable,clock source is iosc/4 selected
    SysTickIntRegister(SysTickIRQHandler);
}
