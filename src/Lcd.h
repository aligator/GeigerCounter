#ifndef LCD_H_
#define LCD_H_

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Lcd {
public:
    Lcd();
	virtual ~Lcd();

	void setup();
	void loop();
private:
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
};


#endif /* LCD_H_ */