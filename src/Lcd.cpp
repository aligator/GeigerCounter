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

void Lcd::loop(GeigerStats stats)
{
    if (stats.lastImpulses != this->lastImpulses ||
        stats.cpm != this->cpm ||
        stats.uSv != this->uSv
    ) {
        if (stats.isFullAccuracy) {
            this->lcd.setCursor(0, 0);
            this->lcd.print("Geiger Counter      ");
        }

        this->lastImpulses = stats.lastImpulses;
        this->cpm = stats.cpm;
        this->uSv = stats.uSv;

        this->lcd.setCursor(10, 1);
        this->lcd.print("          ");
        this->lcd.setCursor(10, 1);
        this->lcd.print(stats.lastImpulses);
        this->lcd.setCursor(10, 2); 
        this->lcd.print("          ");
        this->lcd.setCursor(10, 2);
        this->lcd.print(stats.cpm);
        this->lcd.setCursor(10, 3); 
        this->lcd.print("          ");
        this->lcd.setCursor(10, 3);
        this->lcd.print(stats.uSv, 4);
    }
}