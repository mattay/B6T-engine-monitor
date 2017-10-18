#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <Arduino.h>
//include classes Sensor and Debuggable
#include "Sensor.h"
// #include "Debuggable.h"

class TemperatureSensor: public Sensor {
  public:
    inline TemperatureSensor( int userPin );
    virtual int read();
  private:
    int pin;
};

#endif
