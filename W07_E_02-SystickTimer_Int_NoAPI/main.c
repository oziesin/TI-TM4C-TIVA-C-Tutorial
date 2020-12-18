/***********************Headers***********************/

#include <stdint.h>
#include  "inc/tm4c123gh6pm.h"


/***********************Variables***********************/



/***********************Function Declarations***********************/
void SysTickIRQHandler(void){

    GPIO_PORTF_DATA_R ^= (1 << 1); //  Toggle red led
}

void Init(void);
void InitSysTick(void);
int main(void){
    Init();
    InitSysTick();
    while (1){

    }
}
void Init(void){

    SYSCTL_RCGCGPIO_R   |=   1<<5 ;                              //  Portf clock enable
    GPIO_PORTF_DIR_R    |=   1<<1 | 1<<2 | 1<<3 ;                //  Select pin 1,2,3 as output
    GPIO_PORTF_DEN_R    |=   1<<1 | 1<<2 | 1<<3;                 //  Select pin 1,2,3 enable

}

void InitSysTick(){

    NVIC_ST_RELOAD_R    = 16000000-1;     // For 1 sec.
    NVIC_ST_CURRENT_R   =0;             // Clear current value.
    NVIC_ST_CTRL_R     |= (1<<0)|(1<<1)|(1<<2); // Enable,interrupt enable, system clock source select.

}
