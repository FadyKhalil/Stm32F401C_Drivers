/*
 * Lcd.h
 *
 *  Created on: Apr 6, 2022
 *      Author: fady
 */

#ifndef LCD_H_
#define LCD_H_

#include "Std_types.h"


typedef struct
{
	void* Port;
	u16 Pin;
	u8 Speed;

}Lcd_tstrPinConfiguration;

typedef enum
{
	Lcd_enuProccessing = 0,
	Lcd_enuBusy

}Lcd_tenuState;

typedef struct
{
	u8* string;
	u8 LineNumber;
	u8 ColoneNumber;

}Lcd_tstrDisplayChrPosition;

/*------------------------------DEFINES----------------------------------*/


/*Port Connected*/
#define PORTA	GPIO_A
#define PORTB	GPIO_B
#define PORTC	GPIO_C
#define PORTD	GPIO_D
#define PORTE	GPIO_E
#define PORTH	GPIO_H


/*Led Speed Pin*/
#define LCD_LOW_SPEED 		GPIO_Speed_Low
#define LCD_MED_SPEED 		GPIO_Speed_Medium
#define LCD_HIGH_SPEED 		GPIO_Speed_High
#define LCD_VERYHIGH_SPEED 	GPIO_Speed_Very_High


/*This Variables are used for function
 * @Lcd_vidSendCommand()
 * */
#define CLEAR			0x01
#define RETURN_HOME		0x02
#define HOME_POSITION	0x80
#define LINE2			0xC0


/*------------------------------FUNCTION PROTOTYPES--------------------*/


/*
 * Public Function			:	Lcd_vidSendCommand
 *
 * Description				:	This function used to send command to the LCD
 *
 * Input Parameter			:	Copy_u8Command
 * 								@ CLEAR 		-> 	To Clear Screen and go to main (0,0)
 * 								@ RETURN_HOME 	->	To Return the cursor to main (0,0) without deleting
 * 													the contents
 * 								@ HOME_POSITION	->	To go to line 1
 * 								@ LINE2			->	To go to line 2
 *
 * Return 					:	-> the Function return void
 *
 * Input/Output 			:	-> Not applicable
 * */
Lcd_tenuState Lcd_vidSendCommand(u8 Copy_u8Command);

void Lcd_vidInitProcess(void);

/*
 * Public Function			:	Lcd_vidDisplayCharacter
 *
 * Description				:	This function to print on the lcd
 *
 * Input Parameter			:	Copy_u8Character
 * 								@ You can add the ascii character as display character
 * 								@ However to print special character you have to create Custom
 * 								  character using function
 * 								  --> Lcd_vidCreateCustomCharacter
 *
 * Return 					:	-> the Function return void
 *
 * Input/Output 			:	-> Not applicable
 * */
Lcd_tenuState Lcd_vidDisplayStringPosition(Lcd_tstrDisplayChrPosition* Copy_strChrLocation);


void Lcd_vidTask(void);





//
///*
// * Public Function			:	Lcd_vidCreateCustomCharacter
// *
// * Description				:	This function used to send command to the LCD
// *
// * Input Parameter			:	=> Copy_u8CustomeCharacter
// * 								@ the input of the address of CGRAM
// * 									in range (0 -> 7)
// * 								=> Add_pu8CustomCharacter
// * 								@ the input of the custom character size of 8 Byte
// * 									in range {7 in row, 5 in colon}
// * 									example:
// *
// * 										0bXXX10101 ->> Only access in the first 5 bit
// * 										0bXXX10101
// * 										0bXXX10101
// * 										0bXXX10101
// * 										0bXXX10101
// * 										0bXXX10101
// * 										0bXXX10101
// * 										0bXXX10101
// * 										0bXXX10101
// *
// *
// * Return 					:	-> the Function return void
// *
// * Input/Output 			:	-> Not applicable
// * */
//void Lcd_vidCreateCustomCharacter(u8 Copy_u8CgramAddress, pu8 Add_pu8CustomCharacter);
//
//
//
///*
// * Public Function			:	Lcd_vidDisplayString
// *
// * Description				:	This function to print string on lcd
// *
// * Input Parameter			:	Copy_u8String
// * 								@ To print array of character
// *
// * Return 					:	-> the Function return void
// *
// * Input/Output 			:	-> Not applicable
// * */
//void Lcd_vidDisplayString(pu8 add_pu8String);
//
//
//
///*
// * Public Function			:	Lcd_vidGoto
// *
// * Description				:	This function to jump the cursor
// *
// * Input Parameter			:	=> Copy_u8Line
// * 								@ To chose the line
// * 								=> Copy_u8Colon
// * 								@ To chose the colon
// *
// * Return 					:	-> the Function return void
// *
// * Input/Output 			:	-> Not applicable
// * */
//void Lcd_vidGoto(u8 copy_u8Line, u8 copy_u8Colon);



#endif /* LCD_H_ */
