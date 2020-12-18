/*
 * Lcd.c
 *
 *  Created on: 5 Nov 2020
 *      Author: ozie
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.c"
#include "stdio.h"

#include "lcd.h"

void LcdSendCmd (unsigned char CMD){

    GPIOPinWrite(LCD_PORT, RS, 0);                                  //  RS=0 for instruction
    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (CMD & 0xF0));        //  Bits sending high nibble
    GPIOPinWrite(LCD_PORT, E, E);                                   //  E=1 data write start
    SysCtlDelay(2000);
    GPIOPinWrite(LCD_PORT, E, 0);                                   //  E=0 data write end

    SysCtlDelay(2000);

    GPIOPinWrite(LCD_PORT, RS, 0);                                  //  RS=0 for instruction
    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (CMD & 0xF)<<4);    //  Bits sending low nibble
    GPIOPinWrite(LCD_PORT, E, E);                                   //  E=1 data write start
    SysCtlDelay(2000);
    GPIOPinWrite(LCD_PORT, E, 0);                                   //  E=0 data write end
}

void LcdSendData(unsigned char DATA){

    GPIOPinWrite(LCD_PORT, RS, RS);                                   //  RS=1 for data
    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (DATA & 0xF0));         //  Bits sending high nibble
    GPIOPinWrite(LCD_PORT, E, E);                                    //  E=1 data write start
    SysCtlDelay(2000);
    GPIOPinWrite(LCD_PORT, E, 0);                                    //  E=0 data write end

    SysCtlDelay(2000);

    GPIOPinWrite(LCD_PORT, RS, RS);                                   //  RS=1 for data
    GPIOPinWrite(LCD_PORT, D4 | D5 | D6 | D7, (DATA & 0xF) << 4);     //  Bits sending low nibble
    GPIOPinWrite(LCD_PORT, E, E);                                    //  E=1 data write start
    SysCtlDelay(2000);
    GPIOPinWrite(LCD_PORT, E, 0);                                    //  E=0 data write end

}

void LcdInit(void){
    SysCtlDelay(1000000);
    SysCtlDelay(200000);
    LcdSendCmd(0x30);
    SysCtlDelay(100000);
    LcdSendCmd(0x30);
    SysCtlDelay(1000);
    LcdSendCmd(0x30);
    SysCtlDelay(2000);
    LcdSendCmd(0x20);
    SysCtlDelay(2000);

    LcdFuncSet(4, 2, 7);    //  4bit , 2 line 5x7 character
    SysCtlDelay(2000);
    LcdSendCmd(0xF);        //  Display on,cursor on, blink on
    SysCtlDelay(2000);
    LcdSendCmd(0x6);        //  Increment on,display shift off
    SysCtlDelay(2000);
    LcdClear();             //  Display clear
    SysCtlDelay(2000);

    LcdGotoXY(1, 1);
    SysCtlDelay(100000);

}

void LcdClear(void){

    LcdSendCmd(0x1);
    SysCtlDelay(100000);
}

void LcdFuncSet( unsigned char DL,unsigned char N,unsigned char F){
    uint8_t cmd=0;
    if(DL==4){              //  4 bit mode
        cmd|=0x20;
    }
    else if(DL==8){         //  8 bit mode
        cmd|=0x30;
    }
    if(N==1){               //  1 line mode
        cmd|=0x20;
    }
    else if(N==2){          //  2 line mode
        cmd|=0x28;
    }
    if(F==7){               //  5x7 character pattern
        cmd|=0x20;
    }
    else if(F==10){         //  5x10 character pattern
        cmd|=0x24;
    }

    LcdSendCmd(cmd);
    SysCtlDelay(100000);
}

void LcdGotoXY(unsigned char X, unsigned char Y){

    LcdSendCmd(0x80 |(((X-1)<<6)+Y-1));    // Line1 begin 00h,end 27h for 4 bit mode. Line2 begin 40h,end 67h for 4 bit mode.
    SysCtlDelay(100000);
}

void LcdSendString(char* arr){

    while(*arr !=0){
        LcdSendData(*arr++);
    }

}



