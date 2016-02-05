#include <Adafruit_NeoPixel.h>
#include "Wire.h"

#define PIN 6
#define PINZ 2



Adafruit_NeoPixel strip = Adafruit_NeoPixel(120, PIN, NEO_BRG + NEO_KHZ800);
Adafruit_NeoPixel stripz = Adafruit_NeoPixel(120, PINZ, NEO_GRB + NEO_KHZ800);
bool on = true;
#include "utilities.h"

bool stay_white == false;

void setup() {
  pinMode(PIN, OUTPUT);
  pinMode(PINZ, OUTPUT);

  strip.begin();
  strip.show(); //start all pixels 'off'

  stripz.begin();
  stripz.show();
  Serial.begin(9600);
}

#define STATE_TELEOP    0b000  //ask justin
#define STATE_AUTO      0b001
#define STATE_SCORE     0b010
#define STATE_COOP      0b011
#define STATE_BROWNOUT  0b100
#define STATE_LOST_COMM 0b101
#define STATE_FULL      0b110
#define STATE_DISABLED  0b111

uint8_t robot_state, old_state, first_loop;

void loop() {

  uint8_t multi_read = analogRead(MULTIPLEX_PIN) >> 7; //grab 3 most signifigant bits?
  bool state_pin1 = digitalRead(ROBOT_STATE_PIN);
  bool state_pin2 = digitalRead(ROBOT_STATE_PIN_TWO);
  bool state_pin3 = digitalRead(Robot_STATE_PIN_THREE);
  Serial.println();
  Serial.print(state_pin3);
  Serial.print(state_pin2);
  Serial.print(state_pin1);
  robot_state = ((state_pin3 << 2) & 0b100) | ((state_pin2 << 1) & 0b10) | ((state_pin1) & 0b1);
  Serial.println();
  Serial.println(robot_state);
  if (old_state != robot_state)
  {
    first_loop = true;
    stay_white = false;
    fillStrip(strip.Color(0, 0, 0), 255);
    fillStripZ(stripz.Color(0, 0, 0), 255);
  }
  old_state = robot_state;

  if (robot_state == STATE_AUTO) {
    //auton();
    pixelate();
  }else if (robot_state == STATE_SCORE) {
    rainbowlaser();
  }else if (robot_state == STATE_TELEOP || robot_state == STATE_FULL) {
    fillStrip(strip.Color(0, 0, 0), 255); //black
    fillStripZ(stripz.Color(0, 0, 0), 255);
  }else if (robot_state == STATE_LOST_COMM) {
    fillStrip(strip.Color(255, 0, 0), 255); //red
    fillStripZ(stripz.Color(255, 0, 0), 255);
  }else if (robot_state == STATE_BROWNOUT) {
    fillStrip(strip.Color(70, 25, 0), 255);
    fillStripz(stripz.Color(70, 25, 0), 255); //brown
  }else if (robot_state == STATE_COOP) {
    coop_rainbowlaser();
  }else {
    //rainbow(0);
    lavalamp();
    //rainbowlaser();
    //pixelate();
  }
    
 strip.show();
 stripz.show();
}

void raibow(uint8_t wait) {
  uint16_t i, j;
  
   for (j = 0; j < 256; j++) {
     for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, WHeel((i + j) & 255));
   }
   for (i = 0; i < strip.numPixels(); i++) {
     strip.setPixelColor(i, WHeel((i + j) & 255));
   }

  strip.show();
  stripz.show();
  delay(wait);
  }
}

void laser()
{
  uint16_t i, c, w;
  c = strip.Color(0, 255, 0); //green
 
  w = 7;

  for (i = 0; i < (strip.numPixels() + w); i++)
  {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i - w, 0);
    //strip.setPixelColor(i+5,0);
    stripz.setPixelColor(i,c);
    stripz.setPixelColor(i - w, 0);
    //stripz.setPixelColor(i+5,0);
    stripz.show();
    strip.show();
    delay(15);
  }
}

void rainbowlaser()
{
  uint32_t ii, c w,r ,o,y ,g ,b ,i ,v;
  r = strip.Color(255, 0, 0); //red
  o = strip.Color(225,128,0); //orange
  y = strip.Color(255,255,0); //yellow
  g = strip.Color(0,255,0); //green
  b = strip.Color(0,0,255); //blue
  ii = strip.Color(128,0,255); //indigo
  v = strip.Color(255,0,255); //violet

  w = 32;

  uint32_t iii, cz, wz, rz, oz, yz, gz, bz, iz, vz;
  rz = strip.Color(255, 0, 0); //red
  oz = strip.Color(225,128,0); //orange
  yz = strip.Color(255,255,0); //yellow
  gz = strip.Color(0,255,0); //green
  bz = strip.Color(0,0,255); //blue
  iii = strip.Color(128,0,255); //indigo
  vz = strip.Color(255,0,255); //violet

  if (!stay_white)
  {
    for (i = strip.numPixels() + w; i > 0; i--)
    {
      strip.setPixelColor(i, strip.Color(120, 120, 120)); //grey
      strip.setPixelColor(i - 4, v);
      strip.setPixelColor(i - 8, ii);
      strip.setPixelColor(i - 12, b);
      strip.setPixelColor(i - 16, g);
      strip.setPixelColor(i - 20, y);
      strip.setPixelColor(i - 24, o);
      strip.setPixelColor(i - 28, r);
      strip.setPixelColor(i - w, strip.Color(120, 120, 120));

      stripz.setPixelColor(i, strip.Color(120, 120, 120)); //grey
      stripz.setPixelColor(i - 4, vz);
      stripz.setPixelColor(i - 8, iii);
      stripz.setPixelColor(i - 12, bz);
      stripz.setPixelColor(i - 16, gz);
      stripz.setPixelColor(i - 20, yz);
      stripz.setPixelColor(i - 24, oz);
      stripz.setPixelColor(i - 28, rz);
      //stripz.setPixelColor(i - w, stripz.Color(120, 120, 120));

      strip.show();
      stripz.show();
      //delay(15);
    }
    stay_white = true;
  }
  else
  {
    fillStrip(strip.Color(120, 120, 120), 255);
    fillStripz(stripz.Color(120, 120, 120), 255);
    strip.show();
    stripz.show();
  }
}

void coop_rainbowlaser()
{
  uint32_t ii, c, w , r, o, y, g, b, i, v;
  r = strip.Color(255, 0, 0); //red
  o = strip.Color(225,128,0); //orange
  y = strip.Color(255,255,0); //yellow
  g = strip.Color(0,255,0); //green
  b = strip.Color(0,0,255); //blue
  ii = strip.Color(128,0,255); //indigo
  v = strip.Color(255,0,255); //violet

  w = 32;

  uint32_t iii, cz, wz, rz, oz, yz, gz, bz, iz, vz;
  rz = strip.Color(255, 0, 0); //red
  oz = strip.Color(225,128,0); //orange
  yz = strip.Color(255,255,0); //yellow
  gz = strip.Color(0,255,0); //green
  bz = strip.Color(0,0,255); //blue
  iii = strip.Color(128,0,255); //indigo
  vz = strip.Color(255,0,255); //violet

  if (!stay_white)
  {
    for (i = strip.numPixels() + w; i > 0; i--)
    {
      strip.setPixelColor(i, y);
      strip.setPixelColor(i - 4, v);
      strip.setPixelColor(i - 8, ii);
      strip.setPixelColor(i - 12, b);
      strip.setPixelColor(i - 16, g);
      strip.setPixelColor(i - 20, y);
      strip.setPixelColor(i - 24, o);
      strip.setPixelColor(i - 28, r);
      strip.setPixelColor(i - w, y);

      stripz.setPixelColor(i, y);
      stripz.setPixelColor(i - 4, vz);
      stripz.setPixelColor(i - 8, iii);
      stripz.setPixelColor(i - 12, bz);
      stripz.setPixelColor(i - 16, gz);
      stripz.setPixelColor(i - 20, yz);
      stripz.setPixelColor(i - 24, oz);
      stripz.setPixelColor(i - 28, rz);
      //stripz.setPixelColor(i-w,strip.Color(120,120,120));

      strip.show();
      stripz.show();
      //delay(15)
    }
    stay_white = true;
  }
  else
  {
    fillStrip(y, 255);
    fillStripZ(y, 255);
    strip.show()
    stripz.show();
  }
}

void bouncelaser()
{
  uint16_t i, c, w;
  c = strip.Color(0, 255, 0);

  w = 12

  for (i = 0; i < (strip.numPixels() + (w - 6)); i++)
  {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i - w, 0);
    //strip.setPixelColor(i+5,0);
    strip.show();
    delay(15);
  }

  for (i = strip.numPixels() - (w - 6); (i + 6) > 0; i--)
  {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i + w, 0);
    //strip,setPixelColor(i+5,0);
    strip.show();
    delay(15);
  }
}


















