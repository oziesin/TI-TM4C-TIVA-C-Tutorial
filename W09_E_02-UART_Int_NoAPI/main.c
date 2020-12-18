/***********************Headers***********************/

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"


/***********************Variables***********************/
char data=0;
/***********************Function Declarations***********************/
void UARTInit(void);
void InitRcc(void);
int main(void){

    InitRcc(); //  System clock 80 Mhz.
    UARTInit();

    while(1){

    }

}

void UART0IRQHandler(){

    if(UART0_FR_R & 1<<6){ // Recieve interrupt occur
        UART0_ICR_R |= 1<<4;    // Clear flag
        data=UART0_DR_R;
        UART0_DR_R=data;
    }
}

void UARTInit(){
    /**************************************Clock config**************************************/
    SYSCTL_RCGCGPIO_R |= 1 << 0;   //  Enable GPIOA clock
    SYSCTL_RCGCUART_R |= 1 << 0;   //  Enable UART0 clock
    /**************************************Gpio config**************************************/
    GPIO_PORTA_AFSEL_R |= 1 << 0 | 1 << 1; // Select alternate function PA0 and PA1.
    GPIO_PORTA_PCTL_R |= 1 << 0 | 1 << 4; // PA0 pin is U0Rx and PA1 pin is U0Tx.
    GPIO_PORTA_DEN_R |= 1 << 0 | 1 << 1;   // PA0 and PA1 pins are digital.
    /**************************************Uart config**************************************/
    UART0_CTL_R &= ~(1 << 0); //  Uart0 disable
    /*************BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)*************/
    UART0_IBRD_R |= 520;    // Baudrate integer coefficients
    UART0_FBRD_R |= 54; // Baudrate fractional coefficients     UARTFBRD[DIVFRAC] = integer(BRDF * 64 + 0.5)
    UART0_LCRH_R |= 3 << 5; // Word length:8bits.
    UART0_CC_R &= ~(0xF);    // Select UART clock source is system clock source.

    /**************************************Interrupt config**************************************/
    UART0_IM_R |= 1 << 4 | 1 << 5; //  Enable tx and rx interrupt.
    NVIC_EN0_R |= 1 << 5;    //  Enable UART0 global interrupt.
    UART0_CTL_R |= 1 << 0 | 1 << 8 | 1 << 9; //  Uart0 enable,Transmit enable,Receive enable.
}

void InitRcc(void){

    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2; // The RCC2 register fields override the RCC register fields.
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2; // The system clock is derived from the OSC source and divided by the divisor specified by SYSDIV2.
    SYSCTL_RCC_R |= SYSCTL_RCC_XTAL_16MHZ;      // XTAL frequency select 16 Mhz.
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;        // Main oscillator select.
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;      // The PLL is operating normally.
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;           // 400 MHz pll output enable.
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (4 << 22); // System frequency select 80MHz therefore divisor=5 and SYSDIV2=2,SYSDIV2LSB=0
    while (!(SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS));    // Wait until pll ready.
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2; // The system clock is the PLL output clock divided by the divisor specified by SYSDIV2.
}
