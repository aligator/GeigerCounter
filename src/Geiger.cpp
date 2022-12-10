#include "Geiger.h"

Geiger::Geiger()
{

}

Geiger::~Geiger()
{
}

void Geiger::setup(int pin)
{
  this->pin = pin;
}

void Geiger::setup(int pin, double factor)
{
  this->setup(pin);
  this->factor = factor;
}

void Geiger::loop()
{
  
}