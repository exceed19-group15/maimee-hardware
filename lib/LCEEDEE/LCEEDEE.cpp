#include "LCEEDEE.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void LCD_setup()
{
  lcd.init();
  lcd.backlight();
}

void LCD_showMenu()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     maimee");
  lcd.setCursor(0, 1);
  lcd.print(" Choose a song");
}

void LCD_showScore(String songName, int hitCount, int noteCount)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(songName);
  lcd.setCursor(0, 1);
  lcd.print("HIT: " + String(hitCount) + "/" + String(noteCount));
}

void LCD_showGiveUp()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("YOU SUCK");
}