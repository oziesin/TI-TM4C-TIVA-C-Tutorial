/***********************Headers***********************/
#include <stdint.h>
#include  "inc/tm4c123gh6pm.h"
/***********************Variables***********************/

uint8_t status;
char data;
/***********************Function Declarations***********************/

uint8_t I2CTransmit(uint8_t addr ,char data);
uint8_t I2CReceive(uint8_t addr);
void I2CInit(void);

int main(void){

    I2CInit();

    while (1){

        status=I2CTransmit(0x55,'a');
        data=I2CReceive(0x55);
    }
}


void I2CInit(void){

    /***********************Clock config***********************/
    SYSCTL_RCGCI2C_R |= 1<<0;   // I2C clock enable
    SYSCTL_RCGCGPIO_R |= 1<<1;   // I2C pins(SCL SDA) GPIOB clock enable
    /***********************GPIO config***********************/
    GPIO_PORTB_DEN_R |=    (1<<2) | (1<<3);     // Set PB2 and PB3 to use different mode
    GPIO_PORTB_AFSEL_R  |= (1<<2) | (1<<3);     // Set PB2 and PB3 to alternate function.
    GPIO_PORTB_ODR_R    |= (1<<3);  // Set PB3 as open-drain.
    GPIO_PORTB_PCTL_R   |= (3<<8) | (3<<12);    // via Datasheet page->1351 alternate function table
    /***********************I2C config***********************/
    I2C0_MCR_R =0x10;   // I2C0 Master mode is enabled.
    I2C0_MTPR_R |= 7;   // TPR = (16MHz/(2*(6+4)*100000))-1;
}
uint8_t I2CTransmit(uint8_t addr ,char data){

    I2C0_MSA_R |= addr<<1+0;   // Set slave address and transmit mode.
    I2C0_MDR_R = data;
    I2C0_MCS_R = 0x7;  // Start, run, stop. 1 byte transmission.
    while (I2C0_MCS_R & (1 << 0)); //  Wait until the transmission completes.
    if(I2C0_MCS_R & 1<<1){
        return 0;
    }
    else
        return 1;
}
uint8_t I2CReceive(uint8_t addr){

    I2C0_MSA_R |= (addr << 1) + 1;   // Set slave address and transmit mode.
    I2C0_MCS_R |= 0x7;  // Start, run, stop. 1 byte transmission.
    while (I2C0_MCS_R & (1 << 0)); //  Wait until the transmission completes.
    if (I2C0_MCS_R & 1 << 1)
        return 0;

    return (char)I2C0_MDR_R;
}
