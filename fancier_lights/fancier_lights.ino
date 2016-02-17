#include <Adafruit_NeoPixel.h>
#include "Wire.h"

#define PIN 5
#define PINZ 2

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


