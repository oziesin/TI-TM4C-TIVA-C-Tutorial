/***********************Headers***********************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
/***********************Variables***********************/
char data;
/***********************Function Declarations***********************/
void I2CTransmit(uint8_t addr ,char data);
char I2CReceive(uint8_t addr);
void I2CInit(void);

int main(void){

    I2CInit();


    while (1){
        I2CTransmit(0x55, 'A');
        SysCtlDelay(100000);
        data=I2CReceive(0x55);
        SysCtlDelay(100000);
    }
}


void I2CInit(void){

    /***********************Clock config***********************/
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);   // 40Mhz
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0 );
//    /***********************GPIO config***********************/
    GPIOPinConfigure(GPIO_PB3_I2C0SDA); // Set PB3 as I2C0SDA
    GPIOPinConfigure(GPIO_PB2_I2C0SCL); // Set PB2 as I2C0SCL
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_OD); // Set PB3 as open-drain.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE,GPIO_PIN_3 );
//    /***********************I2C config***********************/
    I2CMasterEnable(I2C0_BASE);
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);    // Set I2C 100Kbps.
}
void I2CTransmit(uint8_t addr, char data){

    I2CMasterSlaveAddrSet(I2C0_BASE, 0x55, false);  // Determine slave address and w/r operations.
    I2CMasterDataPut(I2C0_BASE, data);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND); // Start, run, stop. 1 byte transmission.
    while (I2CMasterBusy(I2C0_BASE));    //  Wait until the transmission completes.
}


char I2CReceive(uint8_t addr){

    I2CMasterSlaveAddrSet(I2C0_BASE, 0x55, true); // Determine slave address and w/r operations.
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); // Start, run, stop. 1 byte transmission.
    char chr = I2CMasterDataGet(I2C0_BASE);
    while (I2CMasterBusy(I2C0_BASE));    //  Wait until the transmission completes.
    return chr;
}
