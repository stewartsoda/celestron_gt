#include "Arduino.h"
#include <Adafruit_DotStar.h>
#include <Encoder.h>

////////////////////////////////////////
// Pin Definitions
////////////////////////////////////////

/* Encoder pins */
#define PIN_AZI_LED_A 13
#define PIN_AZI_LED_B 12
#define PIN_ALT_LED_A 11
#define PIN_ALT_LED_B 10

/* Motor pins */
#define PIN_AZI_MOTOR_A 14
#define PIN_AZI_MOTOR_B 15
#define PIN_AZI_MOTOR_PWM 16
#define PIN_ALT_MOTOR_A 17
#define PIN_ALT_MOTOR_B 18
#define PIN_ALT_MOTOR_PWM 19

/* Serial pins */
#define PIN_SERIAL_TX 5
#define PIN_SERIAL_RX 1

/* Display pins */


////////////////////////////////////////
// Global Variables
////////////////////////////////////////

/* LEDs */
Adafruit_DotStar led(DOTSTAR_NUM, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLK, DOTSTAR_BRG);
uint8_t brightness = 0;
bool goingup=true;

/* Encoders */
Encoder azienc(PIN_AZI_LED_A, PIN_AZI_LED_B);
Encoder altenc(PIN_ALT_LED_A, PIN_ALT_LED_B);

/* Motors */

/* Display */

/* PID */

/* Serial */
// Use D5 on the ItsyBitsy M4 for TX as it is level shifted to 5V
#define PIN_SERIAL_TX 5


////////////////////////////////////////
// Functions
////////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting setup....");

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
    switch (led.getPixelColor(0)) {
      case 0x000000FF:  //blue
        led.setPixelColor(0,0x00FF00FF);  //magenta
        break;
      case 0x00FF00FF:  //magenta
        led.setPixelColor(0,0x00FF0000);  //red
        break;
      case 0x00FF0000:  //red
        led.setPixelColor(0,0x00FFFF00);  //yellow
        break;
      case 0x00FFFF00:  //yellow
        led.setPixelColor(0,0x0000FF00);  //green
        break;
      case 0x0000FF00:  //green
        led.setPixelColor(0,0x0000FFFF);  //cyan
        break;
      case 0x0000FFFF:  //cyan
        led.setPixelColor(0,0x000000FF);  //blue
        break;
    }
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