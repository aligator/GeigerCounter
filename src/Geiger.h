#ifndef GEIGER_H_
#define GEIGER_H_
#include <Arduino.h>
#include "GeigerStats.h"

#define COUNTERS_SIZE 5

class Geiger {
public:
    Geiger();
	virtual ~Geiger();

	void setup(int pin);
    void setup(int pin, double factor);
	void loop();
    GeigerStats getStats();
    
private:
    double factor = 0.0067;

    bool isFullAccuracy = false;

    /**
     * These counters contain the "counts per 15 seconds".
     * 4 of them always represent the counts of the last minute.
     * One cell is always the current counting cell.
     */
    unsigned long counters[COUNTERS_SIZE];

    /**
     * This index is the position in the counters array, where new events will be written to.
     */
    byte counterIndex = 0;

    unsigned long lastShift = 0;

    void clear();
    unsigned long getLastImpulses();
    unsigned long getCPM();
    double getUSv();
};

#endif /* GEIGER_H_ */