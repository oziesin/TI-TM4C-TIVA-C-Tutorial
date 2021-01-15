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
#include "driverlib/hibernate.h"

/***********************Variables***********************/
volatile uint32_t  value=0;

/***********************Function Declarations***********************/
void HibernateInit(void);

void main(void){

    HibernateInit();

    while(1){

    }
}
void GPIOFHandler(void){

    value=GPIOIntStatus(GPIO_PORTF_BASE, true);

    if(value & 1<<4 ){

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 ,0);
        HibernateRTCSet(0);     // sets the value of rtc counter.
        HibernateRTCMatchSet(0, 5); //  Wake up when passed 3 sec.
        HibernateRequest(); //  Hibernate mod active
    }
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_4);
}

void HibernateInit(void){
    /**********************************  clock settings*********************************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    /********************************** GPIO settings***********************************************************/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 , GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);  // led on
    /********************************** Interrupt settings***********************************************************/
    IntEnable(INT_GPIOF);
    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
    IntMasterEnable();
    IntRegister(INT_GPIOF, GPIOFHandler);


    /********************************** Sleep mode***********************************************************/
//    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOF);
//    SysCtlPeripheralClockGating(true);
//    SysCtlSleep();
    /********************************** Hibernate settings******************************************************/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);    //  hibernate clock enable
    HibernateEnableExpClk(SysCtlClockGet());   //   hibernate module clock setting
    HibernateGPIORetentionEnable();
    SysCtlDelay(10000000);        //   delay
    HibernateRTCEnable();
    HibernateWakeSet(HIBERNATE_WAKE_PIN|HIBERNATE_WAKE_RTC);       //  Wake up via wake up pin or rtc counter.
}
