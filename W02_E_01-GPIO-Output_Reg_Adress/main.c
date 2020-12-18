/***********************Headers***********************/

/***********************Variables***********************/

#define SYSCTL_RCGCGPIO    (*((volatile unsigned int* )0x400FE608))
#define GPIO_PORTF_DIR     (*((volatile unsigned int* )0x40025400))
#define GPIO_PORTF_DEN     (*((volatile unsigned int* )0x4002551C))
#define GPIO_PORTF_DATA     (*((volatile unsigned int* )0x400253FC))
/***********************Function Declarations***********************/

void Init(void);
void Delay(unsigned int time);
int main(void){

    Init();
    while (1){
        GPIO_PORTF_DATA |= (1<<3);
        Delay(1000000);
        GPIO_PORTF_DATA =0;
        Delay(1000000);
    }

}

void Init(void){

    SYSCTL_RCGCGPIO |= 1 << 5;                           //  GPIOF Clock enable
    GPIO_PORTF_DIR |= (1 << 1) | (1 << 2) | (1 << 3); //  GPIOF pin1,pin2 and pin3 selected output
    GPIO_PORTF_DEN |= (1 << 1) | (1 << 2) | (1 << 3); //  GPIOF pin1,pin2 and pin3 enable

}

void Delay(unsigned int time){
    while(time--);

}
