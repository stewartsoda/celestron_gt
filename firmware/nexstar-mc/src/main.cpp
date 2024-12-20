#include "Arduino.h"
#include <Adafruit_DotStar.h>



/* Globals */
Adafruit_DotStar led(DOTSTAR_NUM, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLK, DOTSTAR_BRG);
uint8_t brightness = 0;
bool goingup=true;


void setup()
{
  led.begin();
  led.setBrightness(0);
  led.setPixelColor(0,0x000000FF);
  led.show();

}

void loop()
{
  if(brightness == 255) {
    goingup = false;
    delay(500);
  }
  if(brightness == 0) {
    goingup = true;
    delay(1000);
  }
  if(goingup) {
    brightness++;
    delay(5);
  }
  else {
    brightness--;
    delay(7);
  }
  led.setBrightness(brightness);
  led.show();

}