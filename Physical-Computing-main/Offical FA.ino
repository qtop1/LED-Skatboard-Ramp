//Currently led just goes across
//but want to follow back and forth along ramp
//so store and compare the positions touched

////////////////////Prep for LED
//**
//REFERENCE :https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
#include <Adafruit_NeoPixel.h>


#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
/////

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 36
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//**
int j = 0 ;
///////////////////PREP FOR MPR 121
//**
//REFERENCE : Example code from Arduino's MPR 121 example code
#include <Wire.h>
#include "Adafruit_MPR121.h"
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

uint16_t lasttouched = 0;
uint16_t currtouched = 0;
//**
int pre_pos = -1;
int b = 20;
int no = NUMPIXELS;




void setup() {

  // put your setup code here, to run once:
  //set the MPR 121. Returns true if sensor found
  cap.begin(0x5A);

  //set all pixels
  pixels.begin();
}

void loop() {
  pixels.clear();
  //** //REFERENCE : Example code from Arduino's MPR 121 example code
  currtouched = cap.touched();
  // put your main code here, to run repeatedly:
  for (uint8_t i = 0; i < 12; i++) {
    //dependent is currently touched and differs from previous
    //position. Then turn led and go/prepare for next position
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
      //**

      //as user goes across the led then changes from pink to blue
      pixels.setPixelColor(j, pixels.Color(255, 0, b));
      if (j == 0) {
        b = 20;
      }
      pixels.show();
      // delay(100);
      //j += 1;
      if (i < pre_pos) {
        j -= 1;
        b -= 5;
      } else {
        j += 1;
        b += 5;
      }
      pre_pos = i  ;
      //Serial.println(i);
      //if at first position & b = 255


      //METAL POLE EFFECT
     //colors change oposite ends
      //if user touches pin 7(attached to pole) the two colours on opposite ends interchange
      if (i == 7) {
        pixels.clear();
        //repeats action 5 times 
        for (int repeat = 0; repeat < 5; repeat++) {
          //nested loop to isolate and get each pos from oppposite ends and change pos every second
          for (int q = 0; q < NUMPIXELS; q++) {
            for (int i = 0; i < NUMPIXELS; i++) {
              pixels.setPixelColor(q , 0, 0, 50);
              pixels.setPixelColor(no , 10, 10, 10);
            }
            no -= 1;
            pixels.show();
            delay(50);

            if (no == 0) {
              no = NUMPIXELS;
            }

          }


        }

      }
    }
  }

  //restarts pixels to start when surpass total num of pixels
  if (j > NUMPIXELS) {
    j = 0;
  }

  //reset.Else cause flashing effect as last touch wont change
  lasttouched = currtouched;



}
