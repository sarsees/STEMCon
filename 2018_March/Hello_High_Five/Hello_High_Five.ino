#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>

float X, Y, Z;// holders for current accelerometer measurements
float old_X = 0, old_Y = 0, old_Z = 0;// holders for last seen accelerometer measurements
float accelerometer_difference;// holder for change in accelerometer measurements
float sound_measurement;// holder for current microphone measurement
int jump_detector = 0;// counter to store a detected jump 
int high_five_detector = 0;// counter to store a detected high five
uint8_t pixeln = 0;// counter to light one pixel at a time

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

void loop() {
  // Gather accelerometer measurements
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();

  // Calculate the change in accelerometer measurements from those previously recorded
  accelerometer_difference = abs(X - old_X) + abs(Y - old_Y) + abs(Z - old_Z);

  // Gather microphone measurements
  sound_measurement = CircuitPlayground.mic.peak(100);
  
  // Check the accelerometer to see if a jump (acc difference of more than 40)
  // has occured within the last 15 loops.

  if (accelerometer_difference > 40) {
    jump_detector = 15;
  } 

  // Check to see if a high five has occured within the last 15 loops. 
  // The high five is a hand gesture that occurs when two people 
  // simultaneously raise one hand each, about 
  // head-high, and push, slide, or slap the flat of their palm against
  // the flat palm of the other person. This will cause the mic to peak. 
  // Keep in mind that the mic range is from 0 to 341.

  if (sound_measurement > 340) {
    high_five_detector = 15;
  }

  // If the accelerometer has detected a jump and the mic has detected a peak, 
  // we call this a "jumping high five" scenario. The Circuit Playground needs 
  // to respond with lights and noise 

  if (jump_detector && high_five_detector) {
    CircuitPlayground.playTone(500, 100);
      CircuitPlayground.setPixelColor(pixeln++, CircuitPlayground.colorWheel(25 * pixeln));
    if (pixeln == 11) {
      pixeln = 0;
    }
  } else {
      CircuitPlayground.clearPixels();
  }

  // The current accelerometer measurements become the old
  old_X = X;
  old_Y = Y;
  old_Z = Z;

  // If no high fives are detected this loop, reduce the high five detector by 1
  if (jump_detector > 0) {
    jump_detector = jump_detector - 1; 
  }

  if (high_five_detector > 0) {
    high_five_detector = high_five_detector - 1;
  }
/*
  Serial.print("X: ");
  Serial.print(X);
  Serial.print("  Y: ");
  Serial.print(Y);
  Serial.print("  Z: ");
  Serial.println(Z);
*/
//  delay(100);


}
