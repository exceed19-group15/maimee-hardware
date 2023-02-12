#ifndef LCEEDEE_h
#define LCEEDEE_h

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

void LCD_setup();
void LCD_showMenu();
void LCD_showScore(String songName, int hitCount, int noteCount);
void LCD_showGiveUp();


#endif