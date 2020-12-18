/***********************Headers***********************/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "inc/tm4c123gh6pm.h"

/***********************Variables***********************/
volatile uint32_t ADC_data[4],EMG_value=0;
/***********************Function Declarations***********************/
void Config(void);

int main(void){

    Config();
    ADCProcessorTrigger(ADC0_BASE, 1);   // Start adc conversion.
    while(1){

    }
}

void ADC0SS1IRQHandler(void){

    ADCSequenceDataGet(ADC0_BASE,1,ADC_data);
    EMG_value=(ADC_data[0]+ADC_data[1]+ADC_data[2]+ADC_data[3])/4;
    ADCIntClear(ADC0_BASE, 1);
    ADCProcessorTrigger(ADC0_BASE, 1);   // Start adc conversion.
}

void Config(){

    /*******************************************Clock config*******************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN| SYSCTL_XTAL_16MHZ);   // 40Mhz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    /**************************************Gpio config**************************************/
    GPIO_PORTE_AFSEL_R |= 1 << 1; //  Select alternate function pe1 for adc0 ain2 channel.
    GPIO_PORTE_DEN_R &= ~(1 << 1); //  Clear appropriate bit to use pin as analog.
    GPIO_PORTE_AMSEL_R |= 1 << 1;   //  Enable analog mode.
    /*******************************************ADC config*******************************************/
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0); //  Select trigger source
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0,ADC_CTL_CH2); //  ADC0_data=AIN2 value ,interrupt enable,end of sequence
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1,ADC_CTL_CH2 ); //  ADC0_data=AIN2 value ,interrupt enable,end of sequence
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2,ADC_CTL_CH2 ); //  ADC0_data=AIN2 value ,interrupt enable,end of sequence
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3,ADC_CTL_CH2 | ADC_CTL_IE | ADC_CTL_END); //  ADC0_data=AIN2 value ,interrupt enable,end of sequence
    ADCSequenceEnable(ADC0_BASE, 1);    //  Enable SS1
    /*******************************************Interrupt config*******************************************/
    IntEnable (INT_ADC0SS1);      //  Interrupt vector enable for NVIC.
    ADCIntEnable(ADC0_BASE, 1); //   Interrupt enable for SS1.
    IntMasterEnable();          //   Enable interrupts to the processor.
    ADCIntRegister(ADC0_BASE, 1, ADC0SS1IRQHandler);
    ADCIntClear(ADC0_BASE, 1);   // Clear flag
}
