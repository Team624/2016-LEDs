#include <Adafruit_NeoPixel.h>
#include "Wire.h"

#define PIN 5
#define PINZ 2
#define MULTIPLEX_PIN 420 //change 
#define STATUS_PIN 1738 //change
#define ROBOT_STATE_PIN 12
#define ROBOT_STATE_PIN_TWO 22
#define ROBOT_STATE_PIN_THREE 28

Adafruit_NeoPixel strip = Adafruit_NeoPixel(31, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripz = Adafruit_NeoPixel(31, PINZ, NEO_GRB + NEO_KHZ800);

bool on = true;
bool stay_white = false;

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
#define STATE_CLIMB 0b011

bool first_loop = false;
uint8_t robot_state, old_state;

bool gear;
bool shooterReady;
bool intakeActive;
bool ballLoaded;
bool alliance;

bool intakeMan;
bool intakeSafe;
bool intakeAuto;

bool shooterMan;
bool shooterSafe;
bool shooterAuto;

void loop()
{
  uint8_t multi_read = analogRead(MULTIPLEX_PIN) >> 31;
  uint8_t status_read = analogRead(STATUS_PIN) >> 63;

  bool state_pin1 = digitalRead(ROBOT_STATE_PIN);
  bool state_pin2 = digitalRead(ROBOT_STATE_PIN_TWO);
  bool state_pin3 = digitalRead(ROBOT_STATE_PIN_THREE);

  shooterReady = (multi_read & 1);
  ballLoaded = (multi_read >> 1) & 1;
  intakeActive = (multi_read >> 2) & 1;
  gear = (multi_read >> 3) & 1;
  alliance = (multi_read >> 4) & 1;
  
  intakeMan = (status_read & 1);
  intakeSafe = (status_read >> 1) & 1;
  intakeAuto = (status_read >> 2) & 1;
  shooterMan = (status_read >> 3) & 1;
  shooterSafe = (status_read >> 4) & 1;
  shooterAuto = (status_read >> 5) & 1;

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

  if(robot_state == STATE_DISABLED)
  {
    lavalamp();
  }
  else if (robot_state == STATE_TELEOP)
  {
    statusLight();
  }
  else if(robot_state == STATE_SHOOT)
  {
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
  else if (robot_state == STATE_CLIMB)
  {
    climbCRAZE();
  }
  else
  {
    disabled();
  }

  strip.show();
  stripz.show();
}



void climbCRAZE()
{
  clearStrip();
   
   int i, g, l, dg, lg, t, pone, ptwo, ptre, pfor, pfiv, pnum, doops, colour;
   g = strip.Color(0,255,0);
   l = strip.Color(255,255,255);
   dg = strip.Color(0, 100, 0);
   lg = strip.Color(150, 255, 150);
   
  
   t = strip.numPixels();
   pone = (int)random(0, t);
   ptwo = (int)random(0, t);
   ptre = (int)random(0, t);
   pfor = (int)random(0, t);
   pfiv = (int)random(0, t);
   pnum = (int)random(6, 10);
   colour = (int) random(0,3);
   
  
     for(i = pnum; i > 0; i--)
     {
       strip.setPixelColor(i + pone, lg);
       strip.setPixelColor(i + pone * pnum, lg);
       strip.setPixelColor(i + pone + pone, g);
       strip.setPixelColor(i + pone + pone + pone, lg);    
       strip.setPixelColor(i + ptwo, dg);
       strip.setPixelColor(i + ptwo * pnum, g);  
       strip.setPixelColor(i + ptwo + ptwo, dg);
       strip.setPixelColor(i + ptwo + ptwo + ptwo, dg);
       strip.setPixelColor(i + ptre, g);
       strip.setPixelColor(i + ptre * pnum, g);
       strip.setPixelColor(i + ptre + ptre, g);
       strip.setPixelColor(i + ptre + ptre + ptre, dg);
       strip.setPixelColor(i + pfor, l);
       strip.setPixelColor(i + pfor * pnum, dg);
       strip.setPixelColor(i + pfor + pfor, lg);
       strip.setPixelColor(i + pfor + pfor + pfor, lg);
       strip.setPixelColor(i + pfiv, dg);
       strip.setPixelColor(i + pfiv * pnum, dg);
       strip.setPixelColor(i + pfiv + pfiv, g);
       strip.setPixelColor(i + pfiv + pfiv + pfiv, g);
       strip.setPixelColor(i + pfiv, lg);
       strip.setPixelColor(i + pfiv * pnum, l);
       
       stripz.setPixelColor(i + pone, lg);
       stripz.setPixelColor(i + pone + pone, g);
       stripz.setPixelColor(i + pone + pone + pone, lg);
       stripz.setPixelColor(i + ptwo, dg);
       stripz.setPixelColor(i + ptwo * pnum, g);  
       stripz.setPixelColor(i + ptwo + ptwo, dg);
       stripz.setPixelColor(i + ptwo + ptwo + ptwo, dg);
       stripz.setPixelColor(i + ptre, g);
       stripz.setPixelColor(i + ptre * pnum, g);
       stripz.setPixelColor(i + ptre + ptre, g);
       stripz.setPixelColor(i + ptre + ptre + ptre, dg);
       stripz.setPixelColor(i + pfor, l);
       stripz.setPixelColor(i + pfor * pnum, dg);
       stripz.setPixelColor(i + pfor + pfor, lg);
       stripz.setPixelColor(i + pfor + pfor + pfor, lg);
       stripz.setPixelColor(i + pfiv, dg);
       stripz.setPixelColor(i + pfiv * pnum, dg);
       stripz.setPixelColor(i + pfiv + pfiv, g);
       stripz.setPixelColor(i + pfiv + pfiv + pfiv, g);
       stripz.setPixelColor(i + pfiv, lg);
       stripz.setPixelColor(i + pfiv * pnum, l);
     }
     delay(80);
}


uint8_t sweg = 1;
void statusLight()
{
  int blue = strip.Color(0, 0, 255);
  int green = strip.Color(0, 255, 00);
  int red = strip.Color(255, 0, 0);
  int yellow = strip.Color(255, 255, 0);
  int white = strip.Color(255, 255, 255);

  if (first_loop == true)
  {
    if (!alliance)
    {
      fillStrip(255, red);
      fillStripZ(255, red);
      delay(300);
    }
    else
    {
      fillStrip(255, blue);
      fillStrip(255, blue);
      delay(300);
    }
    first_loop = false;
  }

  if (sweg == 0)
    sweg = 1;
  else
    sweg = 0;

  //ball loaded
  if (ballLoaded)
  {
    for (uint8_t i = 0; i < strip.numPixels() / 3; i ++)
    {
      if ((i + sweg) % 2 == 0)
      {
        strip.setPixelColor(i, green);
        stripz.setPixelColor(i, green);
      }
      else
      {
        strip.setPixelColor(i, white);
        stripz.setPixelColor(i, white);
      }
    }
  }
  else
  {
    //intake indicator 1 Roller Active
    for (uint8_t i = 0; i < strip.numPixels() / 6; i ++)
    {
      if (intakeActive)
      {
        if ((i + sweg) % 2 == 0)
        {
          strip.setPixelColor(i, blue);
          stripz.setPixelColor(i, blue);
        }
        else
        {
          strip.setPixelColor(i, white);
          stripz.setPixelColor(i, white);
        }
      }
      else
      {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        stripz.setPixelColor(i, strip.Color(0, 0, 0));
      }
    }
    //intake indicator 2 Roller Mode
    for (uint8_t i = strip.numPixels() / 6; i < strip.numPixels() / 3; i ++)
    {
      if (intakeMan)
      {
        strip.setPixelColor(i, red);
        stripz.setPixelColor(i, red);
      }
      if (intakeSafe)
      {
        strip.setPixelColor(i, yellow);
        stripz.setPixelColor(i, yellow);
      }
      if (intakeAuto)
      {
        strip.setPixelColor(i, green);
        stripz.setPixelColor(i, green);
      }
    }
  }

  //drive indicator gear
  for (uint8_t i = strip.numPixels() / 3; i < 2 * (strip.numPixels() / 3); i ++)
  {
    if (gear)
    {
      strip.setPixelColor(i, red);
      stripz.setPixelColor(i, red);
    }
    else
    {
      strip.setPixelColor(i, white);
      stripz.setPixelColor(i, white);
    }
  }

  //shooter indicator 1 shooter mode
  for (uint8_t i = strip.numPixels() / 3; i < 5 * (strip.numPixels() / 6); i ++)
  {
    if (shooterMan)
    {
      strip.setPixelColor(i, red);
      stripz.setPixelColor(i, red);
    }
    if (shooterSafe)
    {
      strip.setPixelColor(i, yellow);
      stripz.setPixelColor(i, yellow);
    }
    if (shooterAuto)
    {
      strip.setPixelColor(i, green);
      stripz.setPixelColor(i, green);
    }
  }
  //shooter indicator 2 shooter ready
  for (uint8_t i = (5 * (strip.numPixels() / 6)); i < strip.numPixels(); i ++)
  {
    if (shooterReady)
    {
      strip.setPixelColor(i, green);
      stripz.setPixelColor(i, green);
    }
    else
    {
      strip.setPixelColor(i, strip.Color(0,0,0));
      stripz.setPixelColor(i, strip.Color(0,0,0));
    }
  }
}

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
  int green = strip.Color(0, 255, 0);
  int blue = strip.Color(0, 0, 255);
  int red = strip.Color(255, 0, 0);
  int white = strip.Color(255, 255, 255);

  fillStripZ(0, 0);
  fillStrip(0, 0);

  if (first_loop)
  {
    rate1 = 0;
    rate2 = 0;
    pos1 = 0;
    pos2 = strip.numPixels();
    first_loop = false;
  }
  else
  {
    if (pos2 > strip.numPixels() / 2)
      rate2 --;
    else
      rate2 ++;
    if (pos1 > strip.numPixels() / 2)
      rate1 --;
    else
      rate1 ++;

    if (pos1 == 0)
      delay(100);

    pos1 += rate1;
    pos2 += rate2;
    delay(50);

    for (uint8_t i = pos1 - 8; i < pos1 + 8; i ++)
    {
      strip.setPixelColor(i, green);
      stripz.setPixelColor(i, green);
    }

    for (uint8_t i = pos2 - 8; i < pos2 + 8; i ++)
    {
      if (!alliance)
      {
        strip.setPixelColor(i, red);
        stripz.setPixelColor(i, red);
      }
      else
      {
        strip.setPixelColor(i, blue);
        stripz.setPixelColor(i, blue);
      }
    }

    for (uint8_t i = pos2 + 8; i < pos1 - 8; i ++)
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

void disabled()
{
  int green = strip.Color(0,255,0);
  int black = strip.Color(0,0,0);
  int white = strip.Color(255,255,255);
  
  clearStrip();
  
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
      delay(10);
    
    pos1 += rate1;
    pos2 += rate2;
    delay(10);
    
    for(int i = pos1 - 12; i < pos1 + 12; i ++)
    {
      strip.setPixelColor(i, green); 
      stripz.setPixelColor(i, green);
    }
   
    for(int i = pos2 - 12; i < pos2 + 12; i ++)
    {
      if(!alliance)
      {
        strip.setPixelColor(i, white);
        stripz.setPixelColor(i, white); 
      }
    }
   
    for(int i = pos2 + 12; i < pos1 - 12; i ++)
    {
      strip.setPixelColor(i, black);
      stripz.setPixelColor(i, black);
    }
  }  
  delay(90);
}

void pixelate()
{
  int  px, pxs, pxss, pxz, pxsz, pxssz, colour;
  int g, w, b, c;
  g = strip.Color(0, 150, 0); //darkgreen
  b = strip.Color(0, 255, 0); //green
  w = strip.Color(255, 255, 255); //white
  c = strip.Color(0, 0, 0); //black

  int gz, wz, bz, cz;
  gz = stripz.Color(0, 150, 0);
  bz = stripz.Color(0, 255, 0);
  wz = stripz.Color(255, 255,255);
  cz = stripz.Color(0, 0, 0);

  px = (int)random(0, strip.numPixels());
  pxs = (int)random(0, strip.numPixels());
  pxss = (int)random(0, strip.numPixels());
  colour = (int)random(0,4) ;
  if (colour == 0)
  {
    strip.setPixelColor(px, g);
    strip.setPixelColor(pxs, g);
    strip.setPixelColor(pxss, g);

    stripz.setPixelColor(px, gz);
    stripz.setPixelColor(pxs, gz);
    stripz.setPixelColor(pxss, gz);
  }
  else if (colour ==1)
  {
    strip.setPixelColor(px, b);
    strip.setPixelColor(pxs, b);
    strip.setPixelColor(pxss, b);

    stripz.setPixelColor(px, bz);
    stripz.setPixelColor(pxs, bz);
    stripz.setPixelColor(pxss, bz);
  }
  else if (colour == 2)
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


void clearStrip()
{
  for (int i = 0; i < strip.numPixels(); i ++)
  {
    strip.setPixelColor(i, 0);
    //strip.show();
    //delay(1);
  }
  for (int i = 0; i < stripz.numPixels(); i ++)
  {
    //stripz.setPixelColor(i, 0);
    //stripz.show();
    delay(1);
  }
}

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


