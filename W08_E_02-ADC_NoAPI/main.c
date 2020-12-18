/***********************Headers***********************/

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"


/***********************Variables***********************/
uint32_t data=0;

/***********************Function Declarations***********************/
void ADCInit(void);
void InitRcc2(void);
int main(void){

    InitRcc2(); //  System clock 80 Mhz.
    ADCInit();

    while(1){

        if (ADC0_RIS_R & 1 << 3){

            ADC0_ISC_R |= 1 << 3;   //  Clear flag.
            data = ADC0_SSFIFO3_R;  //  Get data.
            ADC0_PSSI_R |= 1 << 3;   //  Start sampling.
        }
    }
}

void ADCInit(){
    /**************************************Clock config**************************************/
    SYSCTL_RCGCADC_R |= 1<<0;  //  Enable clock for adc0.
    SYSCTL_RCGCGPIO_R |= 1<<4; //  Enable porte clock for adc ain0 channel.
    /**************************************Gpio config**************************************/
    GPIO_PORTE_AFSEL_R |= 1 << 3; //  Select alternate function pe3 for adc0 ain0 channel.
    GPIO_PORTE_DEN_R &= ~(1 << 3); //  Clear appropriate bit to use pin as analog.
    GPIO_PORTE_AMSEL_R |= 1 << 3;   //  Enable analog mode.
    /**************************************Adc config**************************************/
    ADC0_ACTSS_R &= ~(0xF); //  Disable all sample sequencers.
    ADC0_EMUX_R &= ~(0xF << 12); //  Select processor trigger (default mode) for SS3.
    ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xF) + 0;   //  Select Channel0.
    ADC0_SSCTL3_R |= 1 << 1 | 1 << 2;   //  End=1,IE=1
    ADC0_RIS_R  &= ~ (1<<3);  //  Disable interrupt
    ADC0_ACTSS_R |= 1<<3; //  Enable SS3.
    ADC0_PSSI_R |=1<<3;   //  Start sampling.
}

void InitRcc2(void){

    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2; // The RCC2 register fields override the RCC register fields.
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2; // The system clock is derived from the OSC source and divided by the divisor specified by SYSDIV2.
    SYSCTL_RCC_R |= SYSCTL_RCC_XTAL_16MHZ;         // XTAL frequency select 16 Mhz.
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;        // Main oscillator select.
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;          // The PLL is operating normally.
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;            // 400 MHz pll output enable.
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (4 << 22); // System frequency select 80MHz therefore divisor=5 and SYSDIV2=2,SYSDIV2LSB=0
    while (!(SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS));    // Wait until pll ready.
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2; // The system clock is the PLL output clock divided by the divisor specified by SYSDIV2.
}
