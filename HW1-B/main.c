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


/***********************Variables***********************/
volatile int32_t delay=1000000;
volatile uint32_t value=0;
/***********************Function Declarations***********************/
void Init(void);
void GPIOFHandler(void);

void main(void){

    Init();

    while(1){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,GPIO_PIN_1);
        SysCtlDelay(delay);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,0);
        SysCtlDelay(delay);
    }
}

void GPIOFHandler(void){

    value=GPIOIntStatus(GPIO_PORTF_BASE, true);

    if(!(value & 1<<4 )){
        delay -= 100000;
        if (delay <= 175000)
            delay = 175000;
    }

    else if(!(value & 1<<0)){
        delay+=100000;
    }
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_4);
}

void Init(void){
    /**********************************  clock settings*********************************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);
    uint32_t Clock = SysCtlClockGet();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    /*********************************  PORTF pin0 unlocked*****************************************************/
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK)=GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+ GPIO_O_CR) |= 0x01;
    /********************************** GPIO settings***********************************************************/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPU);
    /********************************** Interrupt settings******************************************************/
    IntEnable(INT_GPIOF);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_4);
    IntMasterEnable();
    IntRegister(INT_GPIOF, GPIOFHandler);

}
