#include <Adafruit_NeoPixel.h>
#include "Wire.h"

#define PIN 420 //I don't know this Pin, please change
#define PINZ 1738 //I don't know this Pin, please change

#define STATE_PIN 100 //I don't know this Pin, please change
#define MULTIPLEX_PIN 200 //I don't know this Pin, please Change

Adafruit_NeoPixel strip = Adafruit_NeoPixel(31, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripz = Adafruit_NeoPixel(31, PINZ, NEO_GRB + NEO_KHZ800);

bool on = true;
//bool stay_white = false;

#include "utilities.h"

void setup()
{
  pinMode(PIN, OUTPUT);
  pinMode(PINZ, OUTPUT);

  strip.begin();
  strip.show();

  stripz.begin();
  stripz.show();
  //Serial.begin(9600);
}

bool first_loop = false;

bool gear;
bool shooterReady;
bool ballLoaded;
bool shooterWarm;
bool alliance;

bool brownout;
bool lostComms;
bool teleop;
bool auton;
bool climbing;

/*
We need to have a way to see these things (in order of preference):
1. Do we have a ball?
2. Is the shooter warming up?
3. Is the shooter ready to shoot?
4. Which gear we are in
5. Low battery
6. Lost Comms
*/
String robot_state;
String old_state = "disabled";

void loop()
{  
  uint8_t state_read = analogRead(STATE_PIN) >> 15;
  uint8_t multi_read = analogRead(MULTIPLEX_PIN) >> 31;
  
  auton = (state_read & 1);
  teleop = (state_read >> 1) & 1;
  brownout = (state_read >> 2) & 1;
  lostComms = (state_read >> 3) & 1;
  
  ballLoaded = (state_read & 1);
  shooterReady = (state_read >> 1) & 1;
  shooterWarm = (state_read >> 2) & 1;
  gear = (state_read >> 3) & 1;
  alliance = (state_read >> 4) & 1;
  
  if(teleop)
    robot_state = "teleop";
  else if(auton)
    robot_state = "auton";
  else if(lostComms)
    robot_state = "lost comms";
  else if(brownout)
    robot_state = "brownout";/*
  else if(climbing)
    robot_state = "climbing";*/
  else
    robot_state = "disabled";  
  
  if(old_state != robot_state)
  {
    clearStrip();
    first_loop = true;
  }
  
  if(teleop)
  {
    statusLight();
  }
  else if(auton)
  {
    //robot is performing scripted autonomous
    sweep();
  }
  else if(lostComms)
  {
    //loss of Communications
    fillStrip(strip.Color(255,0,0),255);
    fillStripZ(strip.Color(255,0,0),255);
  }
  else if(brownout)
  {
    //power short
    fillStrip(strip.Color(131,79,0),255);
    fillStripZ(strip.Color(131,79,0),255);
  }/*
  else if(climbing)
  {
    //robot is climbing
    climbCRAZE();
  }*/
  else
  {
    //disabled
    lavalamp();
  }
 
  old_state = robot_state;
  
  strip.show();
  stripz.show();
}



void climbCRAZE()
{
  clearStrip();
   
   uint16_t i, g, l, dg, lg, t, pone, ptwo, ptre, pfor, pfiv, pnum, doops, colour;
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

void statusLight()
{
  int black = strip.Color(0,0,0);
  int purple = strip.Color(255,0,255);
  int white = strip.Color(255,255,255);
  int red = strip.Color(255,0,0);
  int green = strip.Color(0,255,0);
  int yellow = strip.Color(255,255,0);
  
  for(uint8_t i = 0; i < strip.numPixels() / 3; i ++)
  {
    if(ballLoaded)
    {
      strip.setPixelColor(i, purple);
      stripz.setPixelColor(i, purple);
    } 
    else
    {
      strip.setPixelColor(i, black);
      stripz.setPixelColor(i, black);
    }
  }
  for(uint8_t i = strip.numPixels() / 3; i < strip.numPixels() * (2 / 3); i ++)
  {
    if(gear)
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
  for(uint8_t i = strip.numPixels() * (2/3); i < strip.numPixels(); i ++)
  {
    if(!shooterReady && !shooterWarm)
    {
      strip.setPixelColor(i, black);
      stripz.setPixelColor(i, black);
    }
    else if(shooterWarm)
    {
      strip.setPixelColor(i, yellow);
      stripz.setPixelColor(i, yellow); 
    }
    else if(shooterReady)
    {
      strip.setPixelColor(i, green);
      stripz.setPixelColor(i, green);
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
  uint32_t green = strip.Color(0, 255, 0);
  uint32_t blue = strip.Color(0, 0, 255);
  uint32_t red = strip.Color(255, 0, 0);
  uint32_t white = strip.Color(255, 255, 255);

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

void pixelate()
{
  uint32_t  px, pxs, pxss, pxz, pxsz, pxssz, colour;
  uint32_t g, w, b, c;
  g = strip.Color(0, 150, 0); //darkgreen
  b = strip.Color(0, 255, 0); //green
  w = strip.Color(255, 255, 255); //white
  c = strip.Color(0, 0, 0); //black

  uint32_t gz, wz, bz, cz;
  gz = stripz.Color(0, 255, 0);
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



