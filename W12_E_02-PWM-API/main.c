/***********************Headers***********************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
/***********************Variables***********************/
#define PWM_FREQ 10000 // 10kHz
#define PWM_DUTY 50    // %50 Duty
/***********************Function Declarations***********************/

void PWMInit(void);

int main(void){

    PWMInit();


    while (1){

    }
}


void PWMInit(void){

    /***********************Clock config***********************/
    SysCtlClockSet(SYSCTL_SYSDIV_1| SYSCTL_USE_PLL | SYSCTL_OSC_MAIN| SYSCTL_XTAL_16MHZ);   // 40Mhz
    SysCtlPWMClockSet(SYSCTL_PWMDIV_2); // PWM clock divided by 2.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
//    /***********************GPIO config***********************/
    GPIOPinConfigure(GPIO_PF1_M1PWM5); // Set PF1 as M1PWM5.
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
//    /***********************PWM config***********************/

    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);   //  Set pwm down counter mode
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, (SysCtlClockGet()/(2*PWM_FREQ)-1));   // Pwm frequency 10kHz
    PWMPulseWidthSet(PWM1_BASE , PWM_OUT_5, ((SysCtlClockGet()/(2*PWM_FREQ))/(100-PWM_DUTY)-1));
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true); // Enable pwm out.
    PWMGenEnable(PWM1_BASE, PWM_GEN_2); // Enable pwm generation2.
}

