/***********************Headers***********************/

#include <stdint.h>
 #include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include  "inc/hw_memmap.h"


/***********************Variables***********************/



/***********************Function Declarations***********************/


void main(void){

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    uint32_t Clock = SysCtlClockGet();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE  , GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    while(1){

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2,GPIO_PIN_1|GPIO_PIN_2);
        SysCtlDelay(20000000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2,0);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,GPIO_PIN_3);

        SysCtlDelay(20000000);

    }
}
