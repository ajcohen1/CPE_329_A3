#include "msp.h"
#include <string.h>

//control codes
#define RS 0x20
#define RW 0x40
#define EN 0x80

//command codes
#define CLEAR_SCREEN 1
#define RETURN_CURSOR_HOME 2
#define INCREMENT_CURSOR_RIGHT 6
#define DISPLAY_ON_CURSOR_BLINK 0xF
#define CURSOR_FIRST_LINE 0x80
#define CURSOR_SECOND_LINE 0xC0
#define FOUR_BIT_MODE 0x38
#define EIGHT_BIT_MODE 0x28

//LCD info
#define COLUMNS 16
#define ROWS 2

void delayMs(int n);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void Write_char_LCD(char character);
void Home_LCD(void);
void Clear_LCD(void);
void Write_string_LCD(unsigned int location, char* string);
void Move_Cursor(unsigned int location);
void nibble_write(unsigned char data);
void nibble_command(unsigned char data);

void Nybble(void);
void LCD_init_V2(void);
void LCD_command_V2(unsigned char command);
void LCD_write_V2(unsigned char data);
/**
 * main.c
 */
int main(void)
{
    LCD_init();

    while(1)
        {
        LCD_command(1);  //clear display
        delayMs(500);

        Write_string_LCD(0,"Hello World");
        Write_string_LCD(16, "Assignment 2");

        delayMs(500);
    }
}

void Write_string_LCD(unsigned int location, char* string) {
    Move_Cursor(location);
    int str_length = strlen(string);
    int index;
    for(index = 0; index < str_length; index++) LCD_data(string[index]);
}

void Move_Cursor(unsigned int location) {
    // location ranges from 0 - 31;
    //First row is 0 - 15
    //Second row is 16-31
    uint8_t on_first_row = location < 16;
    unsigned int testLocation;

    if(on_first_row)
        testLocation = 0x80+location;
    else
        testLocation = 0xC0+(location-16);
////    else if(on_first_row) {
////        Home_LCD();
////        for(num_times_move_right = 0; num_times_move_right < location; num_times_move_right++) LCD_data(' ');
////    }
////    else if(on_second_row){
////        LCD_command(CURSOR_SECOND_LINE);
////        for (num_times_move_right = 16; num_times_move_right < location; num_times_move_right++) LCD_data(' ');
////    }
    LCD_command(testLocation);
}


void Home_LCD(void) {
    LCD_command(0xC0);
}

void Clear_LCD(void) {
    LCD_command(1);
}

void Write_char_LCD(char character) {
    LCD_data(character);
}

void LCD_init(void) {
    P3->DIR |= RS | RW | EN;    //port 3 pins output for control
    P4-> DIR= 0xFF; //Port 4 output for data

    delayMs(30);
    LCD_command(0x20);
    delayMs(10);
    LCD_command(0x20);
    delayMs(1);
    LCD_command(0x20);

    LCD_command(0x28);
    LCD_command(0x06);
    LCD_command(0x01);
    LCD_command(0xF);
}

void LCD_command(unsigned char command) {
//    P3->OUT &= ~(RS | RW);
//    P4->OUT = command;
//    P3->OUT |= EN;
//    delayMs(0);
//    P3->OUT &= ~EN;
    nibble_command(command & 0xF0);
    nibble_command(command << 4);

    if (command < 4) delayMs(4);
    else delayMs(1);
}

void LCD_data(unsigned char data) {
//    P3->OUT |= RS;
//    P3->OUT &= ~RW;
//    P4->OUT = data ;
//    P3->OUT |= EN;
//    delayMs(0);
//    P3->OUT &= ~EN;
    nibble_write(data & 0xF0);
    nibble_write(data << 4);

    delayMs(1);
}

void nibble_write(unsigned char data) {
    P3->OUT |= RS;
    P3->OUT &= ~RW;
    P4->OUT = data ;
    P3->OUT |= EN;
    delayMs(0);
    P3->OUT &= ~EN;
}

void nibble_command(unsigned char command) {
    P3->OUT &= ~(RS | RW);
    P4->OUT = command;
    P3->OUT |= EN;
    delayMs(0);
    P3->OUT &= ~EN;
}



void delayMs(int n) {
    int i,j;

    for(j = 0; j < n; j++)
        for (i = 750; i > 0; i--);
}


