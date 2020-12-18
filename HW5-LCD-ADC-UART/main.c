/***********************Headers***********************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/systick.h"
#include  "driverlib/pin_map.h"
#include "Lcd.h"

/***********************Defines***********************/
#define Mod_Lcd 1
#define Mod_Metin 0
#define Saatgirildi 1
#define Saatbekleniyor 0
/***********************Variables***********************/
volatile uint8_t i=0,j=0; //
volatile char arr[8],mod=0,saat_mod=0,saat[9],hourb,hour,minb,min,secb,sec;
volatile uint32_t ADC_data = 0,rx_data[20];
/***********************Function Declarations***********************/
void Init(void);
void InitSystick(void);
void ADCInit(void);
void UARTInit(void);
void EXTIInit(void);
void main(void){

    Init();
    UARTInit();
    ADCInit();
    InitSystick();
    EXTIInit();
    LcdInit();
    ADCProcessorTrigger(ADC0_BASE, 3);   // Start adc conversion.
    while(1){

    }
}

void UART0IRQHandler(){
    uint8_t k=0;
    uint32_t status = UARTIntStatus(UART0_BASE, true); //  Get interrupt status
    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_TX); //clear the asserted interrupts

    while (UARTCharsAvail(UART0_BASE)){

        if (i > 19) i = 0;
        if (j > 8)  j = 0;
        rx_data[i] = UARTCharGet(UART0_BASE);
        if (rx_data[i] == '*')      mod = Mod_Lcd;
        else if (rx_data[i] == '#') mod = Mod_Metin;
        /***************LCD modu saat kýsmý***************/
        if (mod == Mod_Lcd && rx_data[i] != '*'){


            saat[j] = rx_data[i];
            if ((j == 2 || j == 5) && rx_data[i] != ':'){

                saat[j - 1] = saat[j - 2];
                saat[j - 2] = 0;
                saat[j + 1] = saat[j];
                saat[j] = ':';
                j++;
            }

            if (mod == Mod_Lcd && j !=8 && saat[j] == '\r'){

                saat[j ] = saat[j - 1];
                saat[j - 1] = 0;
                saat[j + 2] = saat[j+1];
                saat[j+1] = '\r';
                j++;
            }
            j++;

        }
        if(mod==Mod_Lcd && j>=8){
//        hour=saat[0];hourb=saat[1];min=saat[3];minb=saat[4];sec=saat[6];secb=saat[7];
        saat_mod=Saatgirildi;
        }

        /****************************************/
        /***************Metin modu***************/
        if(mod==Mod_Metin && rx_data[i]=='#' && rx_data[i-1]=='#'){  //   LCD 2. satýrýný temizleme
            LcdGotoXY(2, 1);
            for (k = 0;k <= 16; k++) {
                LcdSendData(' ');
            }
        }
        if(mod==Mod_Metin){

            LcdGotoXY(2, 1);
            LcdSendData(rx_data[i]);
        }
        /****************************************/

        i++;

    }

}

void EXTI4IRQHandler(void){

    if (GPIOIntStatus(GPIO_PORTF_BASE, true)& 1 << 4)
        UARTStringPut(UART0_BASE, "*MERHABA DUNYA\r");
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
}

void ADC0SS3IRQHandler(void){

    ADCSequenceDataGet(ADC0_BASE, 3,(uint32_t*)&ADC_data);
    ADCIntClear(ADC0_BASE, 3);
    ADCProcessorTrigger(ADC0_BASE, 3);   // Start adc conversion.
}

void SysTickIRQHandler(void){


    if(saat_mod==Saatgirildi){
        saat[7]++;
        if (saat[7] > 48+9){
            saat[7] = 48+0;
            saat[6]++;
        }
        if (saat[6] > 48+5){
            saat[6] = 48+0;
            saat[4]++;
        }
        if (saat[4] > 48+9){
            saat[4] = 48+0;
            saat[3]++;
        }
        if (saat[3] > 48+5){
            saat[3] =48+ 0;
            saat[1]++;
        }
        if (saat[1] > 48+9){
            saat[1] = 48+0;
            saat[0]++;
        }
        if (saat[0] == 48+2 && saat[1] >48+ 3){
            saat[0] = 48+0;
            saat[1] = 48+0;
        }
        LcdGotoXY(1, 1);
        LcdSendString(saat);

    }

    LcdGotoXY(1, 10);
    static char data [6];
    sprintf(data,"#%d",ADC_data);
    LcdSendString(data);
    UARTStringPut(UART0_BASE,data); // Every second sending adc data to serial port app.
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
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); //  Select trigger source
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0,ADC_CTL_CH2 | ADC_CTL_IE | ADC_CTL_END); //  ADC0_data=AIN2 value ,interrupt enable,end of sequence
    ADCSequenceEnable(ADC0_BASE, 3);    //  Enable SS3
    /*******************************************Interrupt config*******************************************/
    IntEnable(INT_ADC0SS3);      //  Interrupt vector enable for NVIC.
    ADCIntEnable(ADC0_BASE, 3); //   Interrupt enable for SS3.
    IntMasterEnable();          //   Enable interrupts to the processor.
    ADCIntRegister(ADC0_BASE, 3, ADC0SS3IRQHandler);
    ADCIntClear(ADC0_BASE, 3);   // Clear flag
}

void UARTInit(void){
    /*******************************************Clock config*******************************************/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);     //  Enable UART0 clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);     //  Enable GPIOA clock
    /*******************************************GPIO config*******************************************/
    GPIOPinConfigure(GPIO_PA0_U0RX);    //  PA0 pin is Rx
    GPIOPinConfigure(GPIO_PA1_U0TX);    //  PA1 pin is Tx
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    /*******************************************UART config*******************************************/
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8); // Wordlength:8bit , Stopbit:1,Paritybit:None
    UARTEnable(UART0_BASE); //  Enable uart.
    /*******************************************Interrupt config*******************************************/
    IntMasterEnable();      //  Enable interrupts.
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_TX); // Interrupt enable recieve and transmit data.
    UARTIntRegister(UART0_BASE, UART0IRQHandler);
   }

void EXTIInit(void){
    /**********************************  clock settings*********************************************************/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    /********************************** GPIO settings***********************************************************/
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    /********************************** Interrupt settings******************************************************/
    IntEnable(INT_GPIOF);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
    IntMasterEnable();
    IntRegister(INT_GPIOF, EXTI4IRQHandler);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
}
void UARTStringPut(uint32_t ui32Base, char *String){

    while (String){

        UARTCharPut(ui32Base, *String++);
        if (*String == '\r'){
            UARTCharPut(ui32Base, *String);
            break;
        }
    }
}
