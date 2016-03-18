/*************************************************** 
MUKE
 ****************************************************/

#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 1

#define data   4
#define clock   5
#define latch   6
#define oe  -1  // set to -1 to not use the enable pin (its optional)

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

int loopCount;
int mode = 3;
long previousMillis = 0;        // will store last time LED was updated
long interval = 1000;           // interval at which to blink (milliseconds)
int laserCount = 5;  // total number of lasers
int intensity = 1;

void setup() {
  Serial.begin(9600);
  
  Serial.println("TLC5974 test");
  tlc.begin();
  loopCount = 0;
}

void loop() {

  if (Serial.available() >= 2) {

    int message = Serial.read();
    if(message == 'T') {
      interval = Serial.read();
      //Serial.println(interval, DEC);
    }
    else if(message == 'M') {
      mode = Serial.read();
      //Serial.println(mode, DEC);
    }
    else if(message == 'I') {
      intensity = Serial.read() / 5;
      //Serial.println(intensity, DEC);
    }
  }

  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   
    if(mode == 1) {
      chase();
    }
    else if(mode == 2) {
      strobe();
    }
    else if(mode == 3) {
      twinkle();
    }
  }

}

void chase() {

  for(int i = 0; i < laserCount; i++) {
    if(i == loopCount) { // 
      tlc.setPWM(i, 4095);
      tlc.write();
      for(int j = 1; j < intensity; j++) {
        int currentLaser = (i +j) % laserCount;
        tlc.setPWM(currentLaser, 4095);
        tlc.write();
      }
    }
    else {
      if(i < loopCount) {
        tlc.setPWM(i, 0);
        tlc.write();
      }
      else if(i >= loopCount + intensity) {
        tlc.setPWM(i, 0);
        tlc.write();
      }
    }
  }
  // increment or reset the count to zero
  if(loopCount == laserCount -1) {
    loopCount = 0;
  }
  else {
    loopCount++;
  }
  
}

void strobe() {
  if(loopCount != 0) {
    for(int i = 0; i < laserCount; i++) {
      tlc.setPWM(i, 0);
      tlc.write();
    }
    loopCount = 0;
  }
  else {
    for(int i = 0; i < laserCount; i++) {
      tlc.setPWM(i, 4095);
      tlc.write();
    }
    loopCount++;
  }
}

void twinkle() {
  // turn off all lasers
  for(int i = 0; i < laserCount; i++) {
    tlc.setPWM(i, 0);
    tlc.write();
  }

  // turn on random ones (based on intensity)
  int randomLaser;
  for(int i = 0; i < intensity; i++) {
    randomLaser = (int) random(0, laserCount);
    tlc.setPWM(randomLaser, 4095);
    tlc.write();
  }
}

