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

/***********************Variables***********************/

/***********************Function Declarations***********************/
void Config(void);

int main(void){

    Config();
    while(1){

    }
}

void UART0IRQHandler(){
    uint32_t status=UARTIntStatus(UART0_BASE, true);//  Get interrupt status
    UARTIntClear(UART0_BASE, UART_INT_RX|UART_INT_TX); //clear the asserted interrupts

    while(UARTCharsAvail(UART0_BASE)){

        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
    }

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
