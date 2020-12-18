/***********************Headers***********************/

#include <stdint.h>
#include  "inc/tm4c123gh6pm.h"


/***********************Variables***********************/



/***********************Function Declarations***********************/

void Init(void);

int main(void){
    Init();
    unsigned int value =0;

    value=GPIO_PORTF_DATA_R;
    while(1){

        if (! value & (1 << 4)){

            GPIO_PORTF_DATA_R |= 1 << 1;
        }
        else if (! value & (1 << 0)){

            GPIO_PORTF_DATA_R |= 1 << 2;
        }
    }
}
void Init(void){

    SYSCTL_RCGCGPIO_R   |=   1<<5 ;                              //  Portf clock enable
    GPIO_PORTF_CR_R     |=   1<<0 ;                         //  Write access enable from register
    GPIO_PORTF_LOCK_R   |= 0x4C4F434B;                                    //  portf pin0 unlocked
    GPIO_PORTF_DIR_R    |=   1<<1 | 1<<2 | 1<<3 ;                //  Select pin 1,2,3 as output
    GPIO_PORTF_DEN_R    |=   1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 ;  //  Select pin 0,1,2,3,4 enable
    GPIO_PORTF_PUR_R    |=   1<<0 | 1<<4 ;                       //  Buton1 and buton2 pull up enable


}
