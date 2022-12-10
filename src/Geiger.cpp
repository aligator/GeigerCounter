#include <Arduino.h>
#include "Geiger.h"

unsigned long interruptImpulses = 0;

// Interrupt-Routine wenn Impuls eintrifft
IRAM_ATTR void tube_impulse(){  //IRAM_ATTR schreibt Routine ins RAM
    interruptImpulses++;
    Serial.print("+");
}

Geiger::Geiger()
{
    this->clear();
}

Geiger::~Geiger()
{
}

void Geiger::clear()
{
    for (size_t i = 0; i < COUNTERS_SIZE; i++) {
        this->counters[i] = 0;
    }

    interruptImpulses = 0;
}

void Geiger::setup(int pin)
{
    this->lastShift = millis();

    this->clear();

    attachInterrupt(digitalPinToInterrupt(pin), tube_impulse, FALLING);
}

void Geiger::setup(int pin, double factor)
{
    this->setup(pin);
    this->factor = factor;
}

void Geiger::loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - this->lastShift >= 15000) {
        Serial.println();
        noInterrupts();
        byte newPulses = interruptImpulses;
        interruptImpulses = 0;
        interrupts();

        this->counters[this->counterIndex] = newPulses;
        this->lastShift = currentMillis;
        Serial.println(this->counters[this->counterIndex]);
        if (this->counterIndex == COUNTERS_SIZE-1) {
            this->isFullAccuracy = true;
        }
        this->counterIndex = (this->counterIndex + 1) % COUNTERS_SIZE;
        this->counters[this->counterIndex] = 0;
    }
}

unsigned long Geiger::getLastImpulses()
{
    size_t index = this->counterIndex;
    if (index == 0) {
        index = COUNTERS_SIZE - 1;
    } else {
        index--;
    }

    return this->counters[index];
}

unsigned long Geiger::getCPM() 
{
    unsigned long impulses = 0;
    for (size_t i = 0; i < COUNTERS_SIZE; i++) {
        if (i != this->counterIndex) {
        impulses += this->counters[i];
        }
    }

    return impulses;
}

double Geiger::getUSv()
{
    return this->getCPM() * this->factor;
}

GeigerStats Geiger::getStats() 
{
    struct GeigerStats stats;
    stats.isFullAccuracy = this->isFullAccuracy;
    stats.lastImpulses = this->getLastImpulses();
    stats.cpm = this->getCPM();
    stats.uSv = this->getUSv();

    return stats;
}