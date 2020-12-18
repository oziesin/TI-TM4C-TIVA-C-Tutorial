/***********************Headers***********************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "stdio.h"
/***********************Defines***********************/
#define Mod_Lcd 1
#define Mod_Metin 0

/***********************Variables***********************/
volatile char mod=0, data,tx_data[20],rx_data[20],adc_data[6],saat[9];
uint16_t adcdata=2357;
volatile uint8_t i[3],j;
char x=12;
/***********************Function Declarations***********************/
void Config(void);
void UARTStringPut(uint32_t ui32Base,char*String);
int main(void){
//    sprintf(tx_data,"12:27:45\r");
    Config();
    while(1){
//        UARTCharPut(UART0_BASE, 'A');
//        SysCtlDelay(1000);

    }
}

void UART0IRQHandler()
{
//    uint32_t status = UARTIntStatus(UART0_BASE, true); //  Get interrupt status
//    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_TX); //clear the asserted interrupts
//
//    while (UARTCharsAvail(UART0_BASE))
//    {

//        rx_data[i] = UARTCharGet(UART0_BASE);
//
//        if(rx_data[i]=='*')     mod=Mod_Lcd;
//        else if(rx_data[i]== '#')   mod=Mod_Metin;
//        if (mod == Mod_Lcd && rx_data[i] != '*'){
//            saat[j]=rx_data[i];
//            if ((j == 2 || j == 5) && rx_data[i] != ':'){
//                saat[j-1]=saat[j-2];
//                saat[j-2]=0;
//                saat[j+1]=saat[j];
//                saat[j]=':';
//                j++;
//            }
//            j++;
//        }
//        if (j == 8 && rx_data[i] != '\r'){
//            saat[j - 1] = saat[j - 2];
//            saat[j - 2] = 0;
//            saat[j + 1] = saat[j];
//            saat[j] = '\r';
//            j++;
//
//        }
//        i++;
//        if(i>19)    i=0;
//        if(j>8) j=0;
//    }

}
void Config(){

    /*******************************************Clock config*******************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);   // 40Mhz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);     //  Enable UART0 clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);     //  Enable GPIOA clock
    /*******************************************GPIO config*******************************************/
    GPIOPinConfigure(GPIO_PA0_U0RX);    //  PA0 pin is Rx
    GPIOPinConfigure(GPIO_PA1_U0TX);    //  PA1 pin is Tx
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1);
    /*******************************************UART config*******************************************/
    UARTConfigSetExpClk(UART0_BASE , SysCtlClockGet(), 9600, UART_CONFIG_PAR_NONE|UART_CONFIG_STOP_ONE|UART_CONFIG_WLEN_8);// Wordlength:8bit , Stopbit:1,Paritybit:None
    UARTEnable(UART0_BASE); //  Enable uart.
    /*******************************************Interrupt config*******************************************/
    IntMasterEnable();      //  Enable interrupts.
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_TX); // Interrupt enable recieve and transmit data.
    UARTIntRegister(UART0_BASE, UART0IRQHandler);
}

void UARTStringPut(uint32_t ui32Base,char*String){

    while(String){
        UARTCharPut(ui32Base, *String++);
        if(*String =='\r') break;
    }
}
