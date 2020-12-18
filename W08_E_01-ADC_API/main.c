/***********************Headers***********************/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"


/***********************Variables***********************/
uint32_t ADC0Value[4];
volatile uint32_t tempavg=0,tempvalC=0,tempvalF=0;


/***********************Function Declarations***********************/
void Config(void);

int main(void){

    Config();
    while(1){
        ADCIntClear(ADC0_BASE, 1);  //  clear flag
        ADCProcessorTrigger(ADC0_BASE, 1);  //  start adc conversion
        while(! ADCIntStatus(ADC0_BASE, 1, false));  // Wait until conversion complete
        ADCSequenceDataGet(ADC0_BASE, 1, ADC0Value); // Get data
        tempavg=(ADC0Value[0]+ADC0Value[1]+ADC0Value[2]+ADC0Value[3] +2)/4; // Average adc values
        tempvalC= (1475-((2475*tempavg))/4096)/10;      //  Calculate celsius from datasheet formula
        tempvalF = ((tempvalC * 9) + 160) / 5;  //  Calculate fahrenheit from datasheet formula
    }
}

void Config(){

    /*******************************************Clock config*******************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);   // 40Mhz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);     //  Enable ADC0 clock

    /*******************************************ADC config*******************************************/
    ADCSequenceConfigure(ADC0_BASE , 1, ADC_TRIGGER_PROCESSOR, 0);      //  Select trigger source
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);  //  ADC0Value[0]=temp sensor value
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);  //  ADC0Value[1]=temp sensor value
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);  //  ADC0Value[2]=temp sensor value
    ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);//  ADC0Value[3]=temp sensor value ,interrupt enable,end of sequence
    ADCSequenceEnable(ADC0_BASE, 1);    //  Enable adc
}
