#include <EEPROM.h>
#include <FastLED.h>

#define trigPin 10
#define echoPin 11
#define NUM_LEDS 30 // How many leds in your strip?
#define DATA_PIN 3

// Define the array of leds
CRGB leds[NUM_LEDS];

float duration, sensordistance;
int b;
int z;
int n;
int MAXR;
int MAXY;
int MAXG;
int MAXRR;
int buttoncount;
int flashcount;
int saveddistance;
int addr = 0;

void setup() {

  buttoncount  = 0;
  saveddistance = 0;
  sensordistance = 0;
  MAXR = 0;
  MAXY = 0;
  MAXG = 0;
  MAXRR = 0;
  Serial.begin (9600);
  Serial.println("SETUP HAS STARTED");

  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  FastLED.clear();  // clear all pixel data
  FastLED.show();
  
  saveddistance = EEPROM.read(addr);
  Serial.print("Reading from EEPROM...");
  Serial.println(saveddistance);

  if(saveddistance == 255)
  {
    ShowError();
    return;
  }

  if(saveddistance > 0) //we found value in EEPROM. Now lets calcutae other distances
    CalcDistances();

  Serial.println("SETUP HAS ENDED***");
}

void loop() {

  //read the pushbutton value into a variable
  int sensorVal = digitalRead(2);

  if (sensorVal == LOW) // when button is pressed
  { 
    buttoncount++;
    Serial.println(buttoncount);
    if(buttoncount >= 3)
      StartCalib();
    else
      delay(1000);
  }
  
  CallSensor();
}

void StartCalib()
{
  Serial.println("Calib Started");
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  
  for (n = 0; n < NUM_LEDS; n++){
    leds[n] = CRGB (100, 100, 100);
    FastLED.show();
    delay(150);
  }

  digitalWrite(13, HIGH);
  Serial.println("SENSOR IS HIGH");
 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  sensordistance = (duration / 2) * 0.0344;

  saveddistance = round(sensordistance+1); //rounding up

  Serial.print("Writing distance to EEPROM...");
  Serial.println(saveddistance);
  EEPROM.write(addr, saveddistance);

  CalcDistances();

  buttoncount = 0;
  flashcount = 0; //reset flashing count when calib has restarted

  Serial.println("Calib ended. buttoncount is 0");

}

void CalcDistances()
{
  MAXRR = saveddistance;
  MAXR = MAXRR + NUM_LEDS;
  MAXY = MAXR + NUM_LEDS;
  MAXG = MAXY + NUM_LEDS;
  
  Serial.println(MAXRR);
  Serial.println(MAXR);
  Serial.println(MAXY);
  Serial.println(MAXG);

}

void CallSensor(){
  
  if (saveddistance <= 0 || saveddistance == 255)
  {
    ShowError();
    return;
  }

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  sensordistance = (duration / 2) * 0.0344;

  Serial.println(sensordistance);

  delay(200);

  if (sensordistance > MAXG) {
    fill_solid( leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    return;
  }

  if (sensordistance > MAXY && sensordistance < MAXG) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    ChangePixelColors(MAXG, sensordistance, 0, 255, 0);
  }
  else if (sensordistance > MAXR && sensordistance < MAXY) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    ChangePixelColors(MAXY, sensordistance, 255, 126, 0);
  }
  else if (sensordistance > MAXRR && sensordistance < MAXR) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    ChangePixelColors (MAXR, sensordistance, 255, 0, 0);
  }
  else if (sensordistance < MAXRR) 
  {
    if (flashcount > 30) //stop flashing after 30 times
    {
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      return;
    }

    ShowFlashingRed();
    flashcount++;
    Serial.println(flashcount);
  }

}

void ShowFlashingRed()
{
  fill_solid( leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
  delay (350);
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay (200);
}

void ChangePixelColors(int MAX, float distance, int R, int G, int B)
{
  b = (MAX - distance) ;
  z = round(b);
  for (n = 0; n <= z; n++)
    leds[n] = CRGB (R, G, B);
  FastLED.show();
  flashcount = 0;
}

void ShowError()
{
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(250);
  fill_solid( leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  delay(250);
}

