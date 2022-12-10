#ifndef LCD_H_
#define LCD_H_
#include "GeigerStats.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Lcd {
public:
    Lcd();
	virtual ~Lcd();

	void setup();
	void loop(GeigerStats stats);
private:
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

    unsigned long lastImpulses = 0;
    unsigned long cpm = 0;
    double uSv = 0.0;
};


#endif /* LCD_H_ */