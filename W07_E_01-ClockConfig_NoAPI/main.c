/***********************Headers***********************/
#include <stdint.h>
#include  "inc/tm4c123gh6pm.h"
/***********************Variables***********************/



/***********************Function Declarations***********************/
void InitRcc(void);
void InitRcc2(void);
int main(void){
    InitRcc();

    while (1){

    }
}


void InitRcc(void){

    SYSCTL_RCC_R |= SYSCTL_RCC_BYPASS;              // The system clock is derived from the OSC source and divided by the divisor specified by SYSDIV.
    SYSCTL_RCC_R &= ~SYSCTL_RCC_USESYSDIV;          // The system clock is used undivided.
    SYSCTL_RCC_R |= SYSCTL_RCC_XTAL_16MHZ;          // XTAL frequency select 16 Mhz.
    SYSCTL_RCC_R &= ~SYSCTL_RCC_OSCSRC_M;           // Main oscillator select.
    SYSCTL_RCC_R &= ~SYSCTL_RCC_PWRDN;              // The PLL is operating normally.
    SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x07800000)+ (4<<23);  // Divisor select 5 therefore system frequency is 40MHz.
    SYSCTL_RCC_R |= SYSCTL_RCC_USESYSDIV;           // The system clock divider is the source for the system clock.
    while(!(SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS));    // Wait until pll ready.
    SYSCTL_RCC_R &= ~SYSCTL_RCC_BYPASS;             // The system clock is the PLL output clock divided by the divisor specified by SYSDIV.
}

void InitRcc2(void){
    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;           // The RCC2 register fields override the RCC register fields.
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;           // The system clock is derived from the OSC source and divided by the divisor specified by SYSDIV2.
    SYSCTL_RCC_R  |= SYSCTL_RCC_XTAL_16MHZ;         // XTAL frequency select 16 Mhz.
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;        // Main oscillator select.
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;           // The PLL is operating normally.
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;            // 400 MHz pll output enable.
    SYSCTL_RCC2_R =  (SYSCTL_RCC2_R &~0x1FC00000)+(4<<22); // System frequency select 80MHz therefore divisor=5 and SYSDIV2=2,SYSDIV2LSB=0
    while(!(SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS));    // Wait until pll ready.
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;          // The system clock is the PLL output clock divided by the divisor specified by SYSDIV2.
}
