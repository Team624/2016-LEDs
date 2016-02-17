#include <Adafruit_NeoPixel.h>
#include "Wire.h"

#define PIN 5
#define PINZ 2
#define MULTIPLEX_PIN 420
#define ROBOT_STATE_PIN 12
#define ROBOT_STATE_PIN_TWO 22
#define ROBOT_STATE_PIN_THREE 28

Adafruit_NeoPixel strip = Adafruit_NeoPixel(31, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripz = Adafruit_NeoPixel(31, PINZ, NEO_GRB + NEO_KHZ800);

#include "utilities.h"

void setup()
{
  pinMode(PIN, OUTPUT);
  pinMode(PINZ, OUTPUT);

  strip.begin();
  strip.show();

  stripz.begin();
  stripz.show();
  Serial.begin(9600);
}

#define STATE_TELEOP 0b000
#define STATE_AUTONOMOUS 0b001
#define STATE_SHOOT 0b010
#define STATE_BROWNOUT 0b100
#define STATE_LOST_COMMS 0b101
#define STATE_DISABLED 0b111

bool first_loop = false;
uint8_t robot_state, old_state;

bool gear;
bool shooterReady;
bool intakeActive;
bool ballLoaded;
bool alliance;

void loop()
{  
  uint8_t multi_read = analogRead(MULTIPLEX_PIN) >> 31;
  
  bool state_pin1 = digitalRead(ROBOT_STATE_PIN);
  bool state_pin2 = digitalRead(ROBOT_STATE_PIN_TWO);
  bool state_pin3 = digitalRead(ROBOT_STATE_PIN_THREE);

  gear = (multi_read & 1);
  shooterReady = (multi_read >> 1) & 1;
  intakeActive = (multi_read >> 2) & 1;
  ballLoaded = (multi_read >> 3) & 1;
  alliance = (multi_read >> 4) & 1;
  
  Serial.println();
  Serial.print(state_pin3);
  Serial.print(state_pin2);
  Serial.print(state_pin1);

  robot_state = ((state_pin3 << 2) & 0b100) | ((state_pin2 << 1) & 0b10) | ((state_pin1) & 0b1);

  Serial.println();
  Serial.println(robot_state);

  if (robot_state != old_state)
  {
    first_loop = false;
  }
  old_state = robot_state;

  if (robot_state == STATE_TELEOP || robot_state == STATE_SHOOT)
  {
    //statusLights();
  }
  else if (robot_state == STATE_AUTONOMOUS)
  {
    sweep();
  }
  else if (robot_state == STATE_BROWNOUT)
  {
    fillStrip(strip.Color(139, 69, 19), 255);
    fillStripZ(stripz.Color(139, 69, 19), 255);
  }
  else if (robot_state == STATE_LOST_COMMS)
  {
    fillStrip(strip.Color(255, 0, 0), 255);
    fillStripZ(stripz.Color(255, 0, 0), 255);
  }
  //31 LEDs on the 20 in. strip
  strip.show();
  stripz.show();
}
/*
void statusLight()
{
  uint8_t blue = strip.Color(0, 0, 255);
  uint8_t green = strip.Color(#06F000);
  uint8_t red = strip.Color(255, 0, 0);
  uint8_t yellow = strip.Color(255, 255, 0);

  //intake indicators
  for (uint8_t i = 0; i < strip.numPixels() / 6; i ++)
  {
    if (ballLoaded == true)
    {
      strip.setPixelColor(i, green);
      stripz.setPixelColor(i, green);
    }
    else
    {
      strip.setPixelColor(i, red);
      stripz.setPixelColor(i, red);
    }
  }
  for (uint8_t i = 0; i < strip.numPixels() / 3; i ++)
  {

  }
  //drivers indicators
  for (uint8_t = (strip.numPixels() / 3) * 2; i < (strip.numPixels() / 3) * 2; i ++)
  {
    if (gear == false)
    {
      strip.setPixelColor(i, blue);
      stripz.setPixelColor(i, blue);
    }
    else
    {
      strip.setPixelColor(i, green);
      stripz.setPixelColor(i, green);
    }
  }

  //shooter indicators
  for (uint8_t i = (strip.numPixels() / 3) * 2; i < (strip.numPixels() / 3) * 2; i ++)
  {

  }
}
*/
void lavalamp()
{
  uint16_t t = millis() / 40;
  for (uint8_t i = 0; i < 33; i++) 
  {
    uint8_t brightness = sin((t + i) / 4.0) * 255;
    uint8_t other = cos((t * 1.1 + i) / 3.5) * 255;
    brightness = brightness - other;
    uint8_t rrr = cos((t / 1 + i / 2) << 8) * 255;
    strip.setPixelColor(i, strip.Color(0, brightness, 0));
    stripz.setPixelColor(i, strip.Color(0, brightness, 0));
  }
  strip.show();
  stripz.show();
}

uint8_t rate1 = -1;
uint8_t rate2 = -1;
uint8_t pos1 = -1;
uint8_t pos2 = -1;

void sweep()
{
  int green = strip.Color(0,255,0);
  int blue = strip.Color(0,0,255);
  int red = strip.Color(255,0,0);
  int white = strip.Color(255,255,255);
  
  fillStripZ(0,0);
  fillStrip(0,0);
  
  if(first_loop)
  {
    rate1 = 0;
    rate2 = 0;
    pos1 = 0;
    pos2 = strip.numPixels();
    first_loop = false; 
  }
  else
  {
    if(pos2 > strip.numPixels() / 2)
      rate2 --;
    else
      rate2 ++;
    if(pos1 > strip.numPixels() / 2)  
      rate1 --;
    else
      rate1 ++;
    
    if(pos1 == 0)
      delay(100);
    
    pos1 += rate1;
    pos2 += rate2;
    delay(50);
    
    for(uint8_t i = pos1 - 8; i < pos1 + 8; i ++)
    {
      strip.setPixelColor(i, green); 
      stripz.setPixelColor(i, green);
    }
   
    for(uint8_t i = pos2 - 8; i < pos2 + 8; i ++)
    {
      if(alliance)
      {
        strip.setPixelColor(i, blue); 
        stripz.setPixelColor(i, blue);
      }
      else
      {
        strip.setPixelColor(i, red);
        stripz.setPixelColor(i, red); 
      }
    }
   
    for(uint8_t i = pos2 + 8; i < pos1 - 8; i ++)
    {
      strip.setPixelColor(i, white);
      stripz.setPixelColor(i, white);
    }
  }    
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  
   for (j = 0; j < 256; j++) {
     for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i + j) & 255));
   }
   for (i = 0; i < strip.numPixels(); i++) {
     strip.setPixelColor(i, Wheel((i + j) & 255));
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
  uint32_t ii, c, w, r, o, y, g ,b ,i ,v;
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
    fillStrip(strip.Color(120, 120, 120), 255); //grey
    fillStripZ(stripz.Color(120, 120, 120), 255);
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
      //stripz.setPixelColor(i-w,strip.Color(120,120,120)); //grey

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
    strip.show();
    stripz.show();
  }
}

void bouncelaser()
{
  uint16_t i, c, w;
  c = strip.Color(0, 255, 0); //green

  w = 12;

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

void pixelate()
{
  uint8_t i, px, pxs, pxss, pxz, pxsz, pxssz, color;
  uint32_t g, w, b, c;
  g = strip.Color(255, 255, 49); //greenish-yellow
  b = strip.Color(255, 255, 0); //yellow
  w = strip.Color(255, 255, 255); //white
  c = strip.Color(0, 0, 0); //black

  uint32_t gz, wz, bz, cz;
  gz = stripz.Color(255, 255, 49);
  bz = stripz.Color(255, 255, 0);
  wz = stripz.Color(255, 255,255);
  cz = stripz.Color(0, 0, 0);

  px = random(0, strip.numPixels());
  pxs = random(0, strip.numPixels());
  pxss = random(0, strip.numPixels());
  color = random(0,4) ;
  if (color == 0)
  {
    strip.setPixelColor(px, g);
    strip.setPixelColor(pxs, g);
    strip.setPixelColor(pxss, g);

    stripz.setPixelColor(px, gz);
    stripz.setPixelColor(pxs, gz);
    stripz.setPixelColor(pxss, gz);
  }
  else if (color ==1)
  {
    strip.setPixelColor(px, b);
    strip.setPixelColor(pxs,b);
    strip.setPixelColor(pxss, b);

    stripz.setPixelColor(px, bz);
    stripz.setPixelColor(pxs, bz);
    stripz.setPixelColor(pxss, bz);
  }
  else if (color == 2)
  {
    strip.setPixelColor(px, c);
    strip.setPixelColor(pxs, c);
    strip.setPixelColor(pxss, c);

    stripz.setPixelColor(px, cz);
    stripz.setPixelColor(pxs, cz);
    stripz.setPixelColor(pxss, cz);
  }
  else
  {
    strip.setPixelColor(px, w);
    strip.setPixelColor(pxs, w);
    strip.setPixelColor(pxss, w);

    stripz.setPixelColor(px, wz);
    stripz.setPixelColor(pxs, wz);
    stripz.setPixelColor(pxss, wz);
  }
strip.show();
stripz.show();
}

void strobe()
{
  uint16_t i, c;
  c = strip.Color(0, 255, 0);

  if(on)
  {
    fillStrip(c, 255);
    //fillStripZ(c, 255);
    on = false;
  }
  else
  {
    fillStrip(strip.Color(0, 0, 0), 255); //black
    on = true;
  }
  delay(300);
  strip.show();
}

void sixtwofour()
{
  uint16_t i, c, s, t, f;
  c = strip.Color(0, 255, 0); //green
  s = 6;
  t = 2;
  f = 4;

  for (i = 0; i < (strip.numPixels() + s + t + f + 15); i++)
  {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i - s, 0);
    strip.setPixelColor(i - s - 5, c);
    strip.setPixelColor(i - s - 5 - t, 0);
    strip.setPixelColor(i - s - 5 - t - 5, c);
    strip.setPixelColor(i - s - 5 - t - 5 - f, 0);
    strip.setPixelColor(i - s - 5 - t - 5 - f - 5, 0);
    //strip.setPixelColor(i+5,0);
    strip.show();
    delay(50);
  }
}

void happylittlewave()
{
  uint32_t b, d, dg, g, wg;
  b = strip.Color(0, 0, 0) //black
  d = strip.Color(0, 50, 0) //darker green
  dg = strip.Color(0, 100, 0) //dark green
  g = strip.Color(0, 255, 0) //green
  wg = strip.Color(75, 255, 75) //light green
  
}

void clearStrip()
{
  for(int i = 0; i < strip.numPixels(); i ++)
  {
    strip.setPixelColor(i, 0);
    strip.show();
    delay(1);
  }
  for(int i = 0; i < stripz.numPixels(); i ++)
  {
    stripz.setPixelColor(i, 0);
    stripz.show();
    delay(1);
  }
}


void auton()
{
  fillStrip(strip.Color(0, 255, 0), 255);
  fillStripZ(stripz.Color(0, 255, 0), 255);
}

/*void score()
{
  fillStrip(strip.Color(255,80,800, 255); //pink
  fillStripZ(stripz.Color(255,80,80), 255);
 }*/

void score()
{
  for (byte i = 0; i < 64; i++) {
    byte whiteness = i * 4;
    fillStrip(strip.Color(whiteness, 255, whiteness), 255);
    fillStripZ(stripz.Color(whiteness, 255, whiteness), 255);
    strip.show();
    stripz.show();
  }
}


