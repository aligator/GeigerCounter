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
    this->lcd.print("...BUFFERING...");
    this->lcd.setCursor(0, 1);
    this->lcd.print("Last 15s: ");      
    this->lcd.print(0);
    this->lcd.setCursor(0, 2); 
    this->lcd.print("     CPM: ");      
    this->lcd.print(0);
    this->lcd.setCursor(0, 3); 
    this->lcd.print("   uSv/h: ");
    this->lcd.print(0, 4);
}

void Lcd::loop(bool isFullAccuracy, unsigned long lastImpulses, unsigned long cpm, double uSv)
{
    if (lastImpulses != this->lastImpulses ||
        cpm != this->cpm ||
        uSv != this->uSv
    ) {
        if (isFullAccuracy) {
            this->lcd.setCursor(0, 0);
            this->lcd.print("Geiger Counter      ");
        }

        this->lastImpulses = lastImpulses;
        this->cpm = cpm;
        this->uSv = uSv;

        this->lcd.setCursor(10, 1);
        this->lcd.print("          ");
        this->lcd.setCursor(10, 1);
        this->lcd.print(lastImpulses);
        this->lcd.setCursor(10, 2); 
        this->lcd.print("          ");
        this->lcd.setCursor(10, 2);
        this->lcd.print(cpm);
        this->lcd.setCursor(10, 3); 
        this->lcd.print("          ");
        this->lcd.setCursor(10, 3);
        this->lcd.print(uSv, 4);
    }
}