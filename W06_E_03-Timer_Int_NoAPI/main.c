/***********************Headers***********************/

#include <stdint.h>
#include  "inc/tm4c123gh6pm.h"


/***********************Variables***********************/



/***********************Function Declarations***********************/

void Init(void);
void InitTimer(void);
int main(void){
    Init();
    InitTimer();

    while (1){

    }
}
void T0AIRQHandler(){

    TIMER0_ICR_R |= (1 << 0);            //  Clear interrupt flag for timeout interrupt
    GPIO_PORTF_DATA_R ^= (1 << 1);       //  Toggle red led

}

void Init(void){

    SYSCTL_RCGCGPIO_R |= 1 << 5;                          //  Portf clock enable
    GPIO_PORTF_DIR_R |= 1 << 1 | 1 << 2 | 1 << 3;         //  Select pin 1,2,3 as output
    GPIO_PORTF_DEN_R |= 1 << 1 | 1 << 2 | 1 << 3;         //  Select pin 1,2,3 enable

}

void InitTimer(){
    SYSCTL_RCGCTIMER_R|=1<<0;       // Enable timer0 clock
    TIMER0_CTL_R &= ~(1<<0);        //  TAEN disable
    TIMER0_CFG_R = 0x0;
    TIMER0_TAMR_R |= 1<<1;          //  Periodic timer config
    TIMER0_TAMR_R &= ~(1<<4);       //  Countdown mode enable
    TIMER0_TAILR_R = 16000000;      //  Load TIMERA current value for 1 sec
    /***********************Interrupt Configuration***********************/
    TIMER0_IMR_R    |= 1<<0;        //  Interrupt enable
    NVIC_EN0_R  |=1<<19;            //  T0A interrupt vector enable

    /*********************************************************************/

    TIMER0_CTL_R |= (1<<0);         //  TAEN enable

}
