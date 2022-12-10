#include "Lcd.h"

Lcd::Lcd()
{

}

Lcd::~Lcd()
{
}

void Lcd::setup()
{
  this->lcd.init();
  this->lcd.backlight();
  this->lcd.setCursor(0, 0);
  this->lcd.print("Make: Geigerzaehler");
}

void Lcd::loop()
{
  
}