/***********************Headers***********************/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
/***********************Variables***********************/
char *chr;
char send[]="oguzhan\r",veri[10];
uint8_t j=0;
/***********************Function Declarations***********************/
void I2CMultiTransmit(uint8_t addr ,char* data);
char* I2CMultiReceive(uint8_t addr);
void I2CInit(void);

int main(void){
    I2CInit();

    while (1){
        chr=I2CMultiReceive(0x55);
        I2CMultiTransmit(0x55, chr);
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
void I2CMultiTransmit(uint8_t addr, char *data){

    I2CMasterSlaveAddrSet(I2C0_BASE, 0x55, false); // Determine slave address and w/r operations.

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START); // Start transmission.
    I2CMasterDataPut(I2C0_BASE, *data);
    while (I2CMasterBusy(I2C0_BASE));    //  Wait until the transmission completes.

    while (*data++){

        if (*data == '\r'){

            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); // Stop transmission.
            I2CMasterDataPut(I2C0_BASE, *data);
            while (I2CMasterBusy(I2C0_BASE));    //  Wait until the transmission completes.
        }

        else{

            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); // Continue transmission.
            I2CMasterDataPut(I2C0_BASE, *data);
            while (I2CMasterBusy(I2C0_BASE));    //  Wait until the transmission completes.
        }
    }

}
char* I2CMultiReceive(uint8_t addr){
    char chr[10];
    uint8_t i=0;
    I2CMasterSlaveAddrSet(I2C0_BASE, 0x55, true); // Determine slave address and w/r operations.

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START); // Start transmission.
    chr[i++] = I2CMasterDataGet(I2C0_BASE);
    while (I2CMasterBusy(I2C0_BASE));    //  Wait until the transmission completes.

    while (i<9){
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT); // Continue transmission.
        chr[i] = I2CMasterDataGet(I2C0_BASE);
        while (I2CMasterBusy(I2C0_BASE));    //  Wait until the transmission completes.
        i++;
        }
    if(i==9){
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH); // Stop transmission.
        chr[i] = I2CMasterDataGet(I2C0_BASE);
        while (I2CMasterBusy(I2C0_BASE));    //  Wait until the transmission completes.
    }
    return chr;
}
