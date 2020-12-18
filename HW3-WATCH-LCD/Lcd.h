/*
 * Lcd.h
 *
 *  Created on: 5 Nov 2020
 *      Author: ozie
 */

#ifndef LCD_H_
#define LCD_H_

/**************************************** Defines****************************************/

#define LCD_PORT                         GPIO_PORTB_BASE
#define RS                               GPIO_PIN_0
#define E                                GPIO_PIN_1
#define D4                               GPIO_PIN_4
#define D5                               GPIO_PIN_5
#define D6                               GPIO_PIN_6
#define D7                               GPIO_PIN_7
/****************************************************************************************/
/********************************** Function Prototypes**********************************/
extern void LcdClear(void);
extern void LcdSendCmd (unsigned char CMD);
extern void LcdSendData(unsigned char DATA);
extern void LcdGotoXY(unsigned char X, unsigned char Y);
extern void LcdFuncSet( unsigned char DL,unsigned char N,unsigned char F);
extern void LcdInit(void);
extern void LcdSendString(char* arr);
/****************************************************************************************/

#endif /* LCD_H_ */
