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
int32_t delay=50000;

/***********************Function Declarations***********************/
void Init(void);

void main(void){

    Init();

    while(1){

        if(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)&(1<<4))){

            delay -= 100000;
            if (delay <= 175000)
                delay = 175000;
        }

        else if(!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)&(1<<0)))
            delay+=100000;


        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,GPIO_PIN_1);
        SysCtlDelay(delay);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,0);
        SysCtlDelay(delay);
    }
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
}
