/***********************Headers***********************/

#include <stdint.h>
#include  "inc/tm4c123gh6pm.h"
/***********************Variables***********************/



/***********************Function Declarations***********************/

void Init(void);
void GPIOFIRQHandler(void);


int main(void){

    Init();

    while (1){

    }
}


void GPIOFIRQHandler(void){

    if (GPIO_PORTF_MIS_R & (1 << 0)){

        GPIO_PORTF_ICR_R |= (1 << 0);                     // Interrupt flag clear for pin0
        GPIO_PORTF_DATA_R =0 ;                   //  Off red led
    }
    else if(GPIO_PORTF_MIS_R & (1 << 4)){

        GPIO_PORTF_ICR_R |= (1 << 4);                     // Interrupt flag clear for pin4
        GPIO_PORTF_DATA_R |= (1 << 1);                    // On red led

    }

}

void Init(void){

    SYSCTL_RCGCGPIO_R |= 1 << 5;                          //  Portf clock enable


    GPIO_PORTF_LOCK_R = 0x4C4F434B;                     //  portf pin0 unlocked
    GPIO_PORTF_CR_R = 1 << 0;                            //  Write access enable from register


    GPIO_PORTF_DIR_R |= 1 << 1 | 1 << 2 | 1 << 3; //  Select pin 1,2,3 as output
    GPIO_PORTF_DEN_R |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4; //  Select pin 0,1,2,3,4 enable
    GPIO_PORTF_PUR_R |= 1 << 0 | 1 << 4;    //  Buton1 and buton2 pull up enable
    /**********************************Interrupt config**********************************/
    GPIO_PORTF_IM_R &= ~((1 << 0) | (1 << 4)); // Interrupt masked for pin0 and pin4
    GPIO_PORTF_IS_R &= ~((1 << 0) | (1 << 4));   // Edge detect sensitive select
    GPIO_PORTF_IBE_R &= ~((1 << 0) | (1 << 4));  // Both edges trigger disable
    GPIO_PORTF_IEV_R &= ~((1 << 0) | (1 << 4));  // Falling edge trigger  select
    GPIO_PORTF_ICR_R |= (1 << 0) | (1 << 4);    // Interrupt flags clear
    GPIO_PORTF_IM_R |= (1 << 0) | (1 << 4); // Interrupt mask clear for pin0 and pin4

    NVIC_EN0_R  |=  1<<30  ;    //  Gpio portf enable. interrupt number 30

}
