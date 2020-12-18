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
#include "driverlib/timer.h"
/***********************Variables***********************/

/***********************Function Declarations***********************/
void Init(void);
void Timer0IRQHandler(void);
void main(void){

    Init();

    while(1){

    }
}

void Timer0IRQHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)){

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,0);

    }
    else
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,GPIO_PIN_1);

}

void Init(void){
    /**********************************  clock settings*********************************************************/
    SysCtlClockSet(
            SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
                    | SYSCTL_OSC_MAIN);
    uint32_t Clock = SysCtlClockGet();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    /*********************************  PORTF pin0 unlocked*****************************************************/
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE+ GPIO_O_CR) |= 0x01;
    /********************************** GPIO settings***********************************************************/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0,GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    /********************************** Interrupt settings******************************************************/
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();
    IntRegister(INT_TIMER0A, Timer0IRQHandler);
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    /********************************** Timer settings******************************************************/
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());
    TimerEnable(TIMER0_BASE, TIMER_A);
}
