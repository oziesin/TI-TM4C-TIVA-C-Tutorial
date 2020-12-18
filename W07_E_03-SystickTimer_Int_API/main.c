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
/***********************Variables***********************/
volatile uint8_t val=0;
/***********************Function Declarations***********************/
void Init(void);
void SysTickIRQHandler(void);
void main(void){

    Init();

    while(1){

    }
}

void SysTickIRQHandler(void){

    val ^= 1 << 1;
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, val);

}

void Init(void){
    /**********************************  clock settings*********************************************************/
    SysCtlClockSet(SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);
    uint32_t clk=SysCtlClockGet();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    /********************************** GPIO settings***********************************************************/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    /********************************** Systick settings******************************************************/
    SysTickPeriodSet(SysCtlClockGet());
    SysTickEnable();
    SysTickIntEnable();
    SysTickIntRegister(SysTickIRQHandler);
}
