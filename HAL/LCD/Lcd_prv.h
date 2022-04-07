/*
 * Lcd_prvh.h
 *
 *  Created on: Mar 14, 2022
 *      Author: fady
 */

#ifndef LCD_PRV_H_
#define LCD_PRV_H_

/*------------------------------TYPES----------------------------------*/


/*This union is used to access bit by bit*/
typedef union
{
    u8 LCD_u8Comunication;

    struct bits
    {
        u8 b0 : 1;
        u8 b1 : 1;
        u8 b2 : 1;
        u8 b3 : 1;
        u8 b4 : 1;
        u8 b5 : 1;
        u8 b6 : 1;
        u8 b7 : 1;

    }LCD_strBitCommunication;

}LCD_tunByteAcces;

/*------------------------------DEFINES----------------------------------*/


/*This Variable is used in initialization Function for LCD*/
#define LCD_FUNCSET				CONC(0, 0, 1, LCD_DATA_LENGTH, LCD_NUMBER_LINES, LCD_CHARACTER_FONT, 0, 0)
#define LCD_DISPLAYCONTROL		CONC(0, 0, 0, 0, 1, LCD_SETON_DISPLAY, LCD_CURSOR_ON, LDC_CURSOR_BLINKING)
#define LCD_CURSOR_DISPLAY_SH	CONC(0, 0, 0, 1, LCD_SC, LCD_RL, 0, 0)
#define LCD_ENTRY_MODE			CONC(0, 0, 0, 0, 0, 1, LCD_ID, LCD_SH)


/*-----------------------Prototype Static Function-----------------*/


/*Enable delay to read the data*/
void Lcd_EnableDelay(void);

#endif /* LCD_PRV_H_ */
