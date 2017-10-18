#include <Arduino.h>
#include <SPI.h> // Hardware-specific library
#include <SD.h> // Hardware-specific library
#include <TFT.h> // Hardware-specific library

// Set LED_BUILTIN if it is not defined by Arduino framework
// #define LED_BUILTIN 13

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_DC     9
#define TFT_RST    8  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK 52   // set these to be whatever pins you like!
#define TFT_MOSI 51   // set these to be whatever pins you like!
// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Color definitions
#define BLACK    0x0000
// #define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
// #define CYAN     0x07FF
// #define MAGENTA  0xF81F
// #define YELLOW   0xFFE0
// #define WHITE    0xFFFF
// #define MAZDA_BLUE     0,179,255    // '#00B3FF' 00 725
// #define SYMBOL_SILVER  138, 141, 143, // '#00B3FF'
// #define SYMBOL_GREY    124, 135, 142    // '#7C878E'
// #define WHITE          '#FFFFFF'
// #define BLACK          '#000000'


#define DEBUG true
#define INPUT_ANALOG 1

// Analog input pins
#define PIN_WATER_TEMP    A0
#define PIN_AIR_TEMP      A1
#define PIN_OIL_PRESSURE  A2

#define RESOLUTION_VOLTAGE 1023.0
#define VOLTAGE_WATER_TEMP 5.0
#define VOLTAGE_AIR_TEMP 5.0

int display_width  = 0;
int display_height = 0;

TFT tft = TFT(TFT_CS, TFT_DC, TFT_RST);

// initial position of the point is the middle of the screen
int xPos = 80;
int yPos = 64;
// direction and speed
// int xDir = 1;
// int yDir = 1;
// variables to keep track of the point's location
// int xPrev = xPos;
// int yPrev = yPos;

void debugMessage (String message){
  if (DEBUG) {
    Serial.print("[DEBUG] ");
    Serial.println(message);
  }
}

float voltage (int value, float max_voltage) {
  return value * (max_voltage / RESOLUTION_VOLTAGE);
}

int sensorValue(int pin) {
  // Reads an Anolog voltage.
  return analogRead(pin);
}

int simulateSensorValue(int previousValue) {
  // Serial.println(previousValue);
  return random(previousValue-20, previousValue+20);
}

float getWaterVoltage () {
  int value = sensorValue(PIN_WATER_TEMP);

  return voltage(value, VOLTAGE_WATER_TEMP);
}

int senval = 600;
int peviousSenVal = senval;
// Printer
char sensorPrintout[6];
char previousSensorPrintout[6];
String volts;
int anologVolts = 0;
int previousAnologVolts = 0;
//Change
char changePrintout[12];
char previousChangePrintout[12];
String direction;

void displaySenorValue () {
  peviousSenVal = senval;
  if (INPUT_ANALOG) {
    senval = sensorValue(A0);
  } else {
    senval = simulateSensorValue(senval);
  }

  if (senval != peviousSenVal ) {
    // Serial.print(peviousSenVal);
    // Serial.print(" -- ");
    // Serial.println(senval);

    volts = String(voltage(senval, 5.0));
    volts = volts + "v";
    volts.toCharArray(sensorPrintout, 6);
    // Serial.println(volts);

    // Clear previous value
    tft.stroke(0, 179, 255);
    // tft.stroke(BLACK);
    tft.text(previousSensorPrintout, 8, 42);
    // Draw new value
    tft.stroke(255,255,255);
    tft.text(sensorPrintout, 8, 42);

    strcpy( previousSensorPrintout, sensorPrintout);
    strcpy( previousChangePrintout, changePrintout);
  }
}

void initialize_tft () {
  tft.begin();
  tft.background(0, 179, 255);  // clear the screen with Mazda Blue

  display_width  = tft.width()-1;
  display_height = tft.height()-1;

  String message = "Display Size: " + String(display_width) +"x"+ String(display_height) ;
  debugMessage(message);
}

void splashSreen () {
  // debugMessage("WHITE");
  // tft.background(255,255,255);  // clear the screen with black
  // debugMessage("SYMBOL_SILVER");
  // tft.background(138, 141, 143);  // clear the screen with black
  // debugMessage("SYMBOL_GREY");
  // tft.background(124, 135, 142);  // clear the screen with black
  debugMessage("MAZDA_BLUE");
  tft.background(0, 179, 255);  // clear the screen with black
}

void setup() {
  Serial.begin(9600);
  Serial.println("initialize_tft...");
  initialize_tft();
  // Serial.println("splashSreen...");
  // splashSreen();
  Serial.println("Setup Complete");
}

int lHeight = tft.height()/2; // Last drawn
int cHeight = tft.height()/2; // Current drawn
int nHeight = tft.height()/2; // Next to draw
int shift = 3;

void loop() {
  displaySenorValue();

  lHeight = cHeight;
  cHeight = nHeight;
  nHeight = map(senval, 0, 1023, 0, tft.height());

  int lchange = (lHeight - cHeight) * -1;
  int nchange = (cHeight - nHeight) * -1;
  int lPos = tft.height() - lHeight;
  int yPos = tft.height() - cHeight;

  // draw a line in a nice color
  tft.stroke(255,255,255);

  if (lchange < -1) {
    direction = "DOWN ";
    // tft.stroke(GREEN);
    tft.line(xPos - shift, lPos+1, xPos, yPos+1);
    // tft.point(xPos+1, yPos);

  } else if (lchange > 1) {
    direction = "UP   ";
    // tft.stroke(RED);
    tft.line(xPos - shift, lPos, xPos, yPos);
    // tft.point(xPos+1, yPos);
    // tft.point(xPos, yPos+2);

  } else {
    direction = "SAME ";
    // tft.stroke(255,255,255);
    tft.line(xPos - shift, lPos, xPos, yPos);
    // tft.point(xPos-1, yPos);
    // tft.point(xPos+1, yPos); // Second
  }
  // Serial.println("[Change] " + String(change) + "  [prev] "  + peviousSenVal + "  [val] " + senval);
  // int drawHeight = map(senval, 0, 1023, 0, tft.height());
  // Serial.println("[direction] " + direction + "  [drawHeight] " + String(drawHeight));
  // if ( vPos3 - vPos2 <= 1 && vPos3 - vPos2 >= -1) {
  //   Serial.println(" SAME " + String(vPos2 - vPos3));
  //   tft.point(xPos,vPos2);
  //   tft.point(xPos+1,vPos2);
  // // }
  // else if ( vPos3 - vPos2 < -1 ) {
  //   Serial.println(" UP " + String(vPos2 - vPos3));
  //   tft.line(xPos, vPos3+1, xPos, vPos2);
  //   tft.point(xPos+1,vPos2);
  // }
  // else if ( vPos3 - vPos2 > 1 ) {
  //   Serial.println(" DOWN " + String(vPos2 - vPos3));
  //   tft.line(xPos, vPos3-1, xPos, vPos2);
  //   tft.point(xPos+1,vPos2);
  // }

  Serial.print("L " + String(lHeight) +
    "  C " + String(cHeight) +
    "  N " + String(nHeight) +
    "  [direction] " + direction +
    "  l " + lPos + "  y " + yPos +
    "  [lchange] " + lchange +
    "  [nchange] " + nchange
  );

  String message = direction + String(lchange);
  message.toCharArray(changePrintout, 12);
  // Clear previous value
  tft.stroke(0, 179, 255);
  // tft.stroke(BLACK);
  tft.text(previousChangePrintout, 8, 22);
  // Draw new value
  tft.stroke(255,255,255);
  tft.text(changePrintout, 8, 22);

   // if the graph has reached the screen edge
   // erase the screen and start again
   if (xPos >= 160) {
    xPos = 0;
    tft.background(0, 179, 255);
    // tft.background(BLACK);
   } else {
     // increment the horizontal position:
     xPos += shift;
     delay(300);
   }

    //  delay(5000);
    Serial.println("");
}

// } else if ( vPos2 - vPos1 < 1 ) {
//   tft.line(xPos, vPos2, xPos, vPos1);
//
// } else if ( vPos1 - vPos2 < 1 ) {
//   tft.line(xPos, vPos2, xPos, vPos1);
// }
// tft.point(xPos,yPos);
// tft.point(xPos,yPos-1);
// tft.line(xPos, tft.height() - drawHeight, xPos, tft.height());


// float senorValueVoltage (int pin, float max_voltage) {
//   if (INPUT_ANALOG == 0) {
//     return 0.0;
//   }
//   // Reads an Anolog voltage and retuns a float.
//   int sensorValue = analogRead(pin);
//   float voltage = sensorValue * (max_voltage / RESOLUTION_VOLTAGE);
//   return voltage;
// }
