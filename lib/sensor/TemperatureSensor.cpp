// #include <Arduino.h>
// //include classes Sensor and Debuggable
// #include "Sensor.h"
// // #include "Debuggable.h"

#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor (int userPin){
  pin = userPin;
  // maxVoltage = userMaxVoltage;
}

//avarage a temperature and return the avaraged sum
int TemperatureSensor::read(){
  return analogRead(pin);
}
