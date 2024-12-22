#include "Arduino.h"
#include <Adafruit_DotStar.h>
#include <Encoder.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <L298NX2.h>
#include <TemperatureZero.h>
#include <Timer.h>

////////////////////////////////////////
// Pin Definitions
////////////////////////////////////////

/* Encoder pins */
#define PIN_AZI_LED_A 13
#define PIN_AZI_LED_B 12
#define PIN_ALT_LED_A 11
#define PIN_ALT_LED_B 10

/* Motor pins */
#define PIN_AZI_MOTOR_1 14    //also A0
#define PIN_AZI_MOTOR_2 15    //also A1
#define PIN_AZI_MOTOR_EN 16  //also A2
#define PIN_ALT_MOTOR_1 17    //also A3
#define PIN_ALT_MOTOR_2 18    //also A4
#define PIN_ALT_MOTOR_EN 19  //also A5

/* Serial pins - Serial1 */
#define PIN_SERIAL_TX 1
#define PIN_SERIAL_RX 0

/* Display pins */
// on the board, pins are defined as follows:
// 1 - GND
// 2 - VCC
// 3 - SCL
// 4 - SDA
// 5 - RST
// 6 - DC
// 7 - CS
#define PIN_OLED_SCL  24    //also SCK
#define PIN_OLED_SDA  25    //also MOSI
#define PIN_OLED_RST  9
#define PIN_OLED_DC   7
#define PIN_OLED_CS   2     //also A6

////////////////////////////////////////
// Global Variables
////////////////////////////////////////

/* LEDs */
Adafruit_DotStar led(DOTSTAR_NUM, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLK, DOTSTAR_BRG);
uint8_t brightness = 0;
#define BRIGHTNESS_MAX 16
#define BRIGHTNESS_MIN 0
#define LED_DELAY 10
bool goingup=true;

/* Encoders */
Encoder azienc(PIN_AZI_LED_A, PIN_AZI_LED_B); // X ticks for 90 degrees of elevation
Encoder altenc(PIN_ALT_LED_A, PIN_ALT_LED_B); // X ticks for 360 degrees of rotation

/* Motors */
L298NX2 myMotors(PIN_ALT_MOTOR_EN, PIN_ALT_MOTOR_1, PIN_ALT_MOTOR_2, PIN_AZI_MOTOR_EN, PIN_AZI_MOTOR_1, PIN_AZI_MOTOR_2);

/* Display */
U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ PIN_OLED_CS, /* dc=*/ PIN_OLED_DC, /* reset=*/ PIN_OLED_RST);
#define SCREEN_FONT u8g2_font_timB14_tf

/* PID */

/* Misc. */
TemperatureZero temp = TemperatureZero();

////////////////////////////////////////
// Functions
////////////////////////////////////////

void printScreen() {
  float temp_val = temp.readInternalTemperature();
  u8g2.clearBuffer();
  u8g2.setCursor(0,14);
  u8g2.print("Scope");
  u8g2.setCursor(0,28);
  u8g2.print("Alt: ");
  u8g2.print(altenc.read());
  u8g2.setCursor(0,42);
  u8g2.print("Azi: ");
  u8g2.print(azienc.read());
  u8g2.setCursor(0,60);
  u8g2.print("Temp: ");
  u8g2.print(temp_val);
  u8g2.sendBuffer();
}

void setup()
{
  // Set up screen
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  u8g2.setFont(SCREEN_FONT);
  u8g2.drawStr(0,18,"Scope");
  u8g2.drawStr(0,36,"Starting up...");
  u8g2.sendBuffer();

// Set up serial devices
  Serial.begin(115200);
  Serial.println("Starting setup....");
  Serial1.begin(115200);

  // Set up LED
  led.begin();
  led.setBrightness(BRIGHTNESS_MIN);
  led.setPixelColor(0,0x000000FF);
  led.show();

  // Set up encoders
  altenc.write(0);
  azienc.write(0);

  temp.init();

  // Set up motors


  // Set up timers
  Timer.repeat(printScreen, 50);

  delay(1000);
  u8g2.clearBuffer();
  u8g2.drawStr(0,18,"Scope ready!");
  u8g2.sendBuffer();
  delay(1000);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    Serial.print("I received: ");
    Serial.println(inChar);
  }
}

void serial1Event() {
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    Serial.print("I received: ");
    Serial.println(inChar);
  }
}

void loop()
{
  if(brightness == BRIGHTNESS_MAX) {
    goingup = false;
  }
  if(brightness == BRIGHTNESS_MIN) {
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
  }
  if(goingup) {
    brightness++;
  }
  else {
    brightness--;
  }
  led.setBrightness(brightness);
  led.show();

  if (Serial.available() > 0) {
    serialEvent();
  }

  if (Serial1.available() > 0) {
    serial1Event();
  }

  Timer.run();
}
