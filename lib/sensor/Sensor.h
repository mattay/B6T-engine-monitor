#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{
  public:
    // Sensor( int pin );
    // inline virtual void begin(){ /*nothing*/ };
    virtual int read ();
  private:
    int _pin;
};

#endif
