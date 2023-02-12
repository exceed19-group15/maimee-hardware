#include <Wire.h>
#include <LiquidCrystal_I2C.h>


void LCD_setup(LiquidCrystal_I2C *lcd)
{
  lcd->init();
  lcd->backlight();
}

void LCD_showMenu(LiquidCrystal_I2C *lcd)
{
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("     maimee");
  lcd->setCursor(0, 1);
  lcd->print(" Choose a song");
}

void LCD_showScore(LiquidCrystal_I2C *lcd,String songName, int hitCount, int noteCount)
{
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print(songName);
  lcd->setCursor(0, 1);
  lcd->print("HIT: " + String(hitCount) + "/" + String(noteCount));
}

void LCD_showGiveUp(LiquidCrystal_I2C *lcd)
{
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("YOU SUCK");
}