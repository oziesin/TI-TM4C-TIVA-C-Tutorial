/***********************Headers***********************/
#include <stdint.h>
 #include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/systick.h"
#include  "driverlib/pin_map.h"
#include "Lcd.h"
/***********************Variables***********************/
volatile uint8_t val=0,hour=21,min=00,sec=0;            //  Cureent time value defined
volatile char arr[8];
volatile uint32_t ADC_data=0;
/***********************Function Declarations***********************/
void Init(void);
void InitSystick(void);
void ADCInit(void);
void main(void){

    Init();
    LcdInit();
    sprintf(arr, "%02d:%02d:%02d", hour, min, sec);
    LcdGotoXY(1, 1);
    LcdSendString(arr);
    InitSystick();
    ADCInit();
    while(1){

    }
}

void ADC0SS3IRQHandler(void){


    ADCSequenceDataGet(ADC0_BASE, 3,(uint32_t*)&ADC_data);
    ADCIntClear(ADC0_BASE, 3);
    ADCProcessorTrigger(ADC0_BASE, 3);   // Start adc conversion.
}

void SysTickIRQHandler(void){
    sec++;
    if(sec>59){
        sec=0;
        min++;
    }
    if(min>59){
        min=0;
        hour++;
    }
    if(hour>23){
        hour=0;
    }
    LcdClear();
    sprintf(arr,"%02d:%02d:%02d",hour,min,sec);
    LcdGotoXY(1, 1);
    LcdSendString(arr);
    LcdGotoXY(1, 11);
    static char data [4];
    sprintf(data,"%d",ADC_data);
    LcdSendString(data);
}

void Init(void){
    /**********************************  clock settings*********************************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);          //  80Mhz system clock
    uint32_t clk=SysCtlClockGet();
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);    //  Portb clock enable for lcd pins
    /********************************** GPIO settings***********************************************************/
    GPIOPinTypeGPIOOutput(LCD_PORT,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);  //  Lcd pins
}
void InitSystick(void){
    NVIC_ST_RELOAD_R = 4000000; // For 1 sec
    NVIC_ST_CURRENT_R=0;        // Reset current value
    NVIC_ST_CTRL_R =3;          // Enable,Interrupt enable,clock source is iosc/4 selected
    SysTickIntRegister(SysTickIRQHandler);
}
void ADCInit(void){
    /**************************************Clock config**************************************/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    /**************************************Gpio config**************************************/
    GPIO_PORTE_AFSEL_R |= 1 << 1; //  Select alternate function pe1 for adc0 ain2 channel.
    GPIO_PORTE_DEN_R &= ~(1 << 1); //  Clear appropriate bit to use pin as analog.
    GPIO_PORTE_AMSEL_R |= 1 << 1;   //  Enable analog mode.
    /*******************************************ADC config*******************************************/
       ADCSequenceConfigure(ADC0_BASE , 3, ADC_TRIGGER_PROCESSOR, 0);      //  Select trigger source
       ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH2 | ADC_CTL_IE | ADC_CTL_END);//  ADC0_data=AIN2 value ,interrupt enable,end of sequence
       ADCSequenceEnable(ADC0_BASE, 3);    //  Enable SS3
    /*******************************************Interrupt config*******************************************/
       IntEnable(INT_ADC0SS3);      //  Interrupt vector enable for NVIC.
       ADCIntEnable(ADC0_BASE, 3); //   Interrupt enable for SS3.
       IntMasterEnable();          //   Enable interrupts to the processor.
       ADCIntRegister(ADC0_BASE, 3, ADC0SS3IRQHandler);
       ADCIntClear(ADC0_BASE, 3);   // Clear flag
       ADCProcessorTrigger(ADC0_BASE, 3);   // Start adc conversion.
}

