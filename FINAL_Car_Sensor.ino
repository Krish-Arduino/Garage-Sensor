#define trigPin 10
#define echoPin 11
#include <FastLED.h>


// How many leds in your strip?
#define NUM_LEDS 30

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 3
#define CLOCK_PIN 13
#define MAXG 135
#define MAXY 105
#define MAXR 75
#define MAXRR 45

// Define the array of leds
CRGB leds[NUM_LEDS];
//Define LED_NUMBER OF LEDS
#define LED_1
int b;
int a;
int z;
int n;
int count;
void setup() {
  Serial.begin (9600);

  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //for (b = NUM_LEDS -1 ; b > 0; b--) {
  //leds[b] = CRGB::Black;
  FastLED.clear();  // clear all pixel data
  FastLED.show();
  //    }
}
void loop() {

  float duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.0344;
  Serial.println(distance);

  delay(100);

  if(distance <= 0)
    return;

  if (distance > MAXG){
    fill_solid( leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    return;
  }
  

  if (distance > MAXY && distance < MAXG) {
    ChangePixelColors(MAXG, distance, 0, 255, 0);
  }
  else if (distance > MAXR && distance < MAXY) {
    ChangePixelColors(MAXY, distance, 255, 126, 0);
  }
  else if (distance > MAXRR && distance < MAXR) {
    ChangePixelColors (MAXR, distance, 255, 0, 0);  
  }
  else if (distance < MAXRR){
    if (count > 30){
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      return;
    }
 
    Serial.println("RR***********************************************************RR");
    fill_solid( leds, NUM_LEDS, CRGB(255,0,0));
    FastLED.show();
    delay (500);
    fill_solid( leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay (500);
    count++;
  }

}

void ChangePixelColors(int MAX, float distance, int R, int G, int B)
{
    //Serial.println("Y***********************************************************Y");
    b = (MAX - distance) ;
    z = round(b);
    for (n = 0; n <= z; n++)
      leds[n] = CRGB (R, G, B);
    FastLED.show();
    count = 0;
}
