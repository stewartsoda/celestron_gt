#include "Arduino.h"
#include "stdint.h"

#include <Adafruit_DotStar.h>
#include <Encoder.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <L298NX2.h>
#include <TemperatureZero.h>
#include <Timer.h>
#include <string.h>

#include "defines.h"
#include "celestron.h"
#include "utils.h"

#define DEBUG true


////////////////////////////////////////
// Global Variables
////////////////////////////////////////

/* LEDs */
Adafruit_DotStar led(DOTSTAR_NUM, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLK, DOTSTAR_BRG);
uint8_t brightness = 0;
bool goingup=true;

/* Encoders */
Encoder azienc(PIN_AZI_LED_A, PIN_AZI_LED_B); // X ticks for 90 degrees of elevation
Encoder altenc(PIN_ALT_LED_A, PIN_ALT_LED_B); // X ticks for 360 degrees of rotation

/* Motors */
L298NX2 myMotors(PIN_ALT_MOTOR_EN, PIN_ALT_MOTOR_1, PIN_ALT_MOTOR_2, PIN_AZI_MOTOR_EN, PIN_AZI_MOTOR_1, PIN_AZI_MOTOR_2);

/* Display */
U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ PIN_OLED_CS, /* dc=*/ PIN_OLED_DC, /* reset=*/ PIN_OLED_RST);

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
  Serial.setTimeout(25);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Starting setup....");
  Serial1.setTimeout(25); //FIXME: make this appropriate for the baud rate and HC messages
  Serial1.begin(115200);  //FIXME: make this appropriate for the HC baud rate

  // Set up LED
  led.begin();
  led.setBrightness(BRIGHTNESS_MIN);
  led.setPixelColor(0,0x000000FF);
  led.show();

  // Set up encoders
  altenc.write(0);
  azienc.write(0);

  // Set up temperature sensor
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
  // if first char is 0x3b
  // then if there are at least 6 bytes
  // then if the checksum is correct
  // then it is a valid Celestron message
  // else wait for another byte
  // else wait for another byte, or too many bytes
  // else it's a debug message

  char* message = (char*)malloc(10);

  if (Serial.read() == 0x3b) {
    // We're expecting this to be a Celestron message
    delay(5); // this should be enough time for a 10-byte message at 9600 bps
    char msg_length = Serial.peek() + 1; // the command length (not including the checksum), plus the length itself 
    if (Serial.readBytes(message, msg_length) == msg_length) {
      char checksum = Serial.read();
      // we have a complete message
      if (checkChecksum(message, checksum) == 0) {
        Serial.print("Valid message: ");
      }
      else {
        Serial.print("Invalid checksum: ");
      }
    }
    else {
      Serial.print("Incomplete message: ");
    }
    for (uint8_t i = 0; i < strlen(message); i++) {
      Serial.print(message[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    decodeCelestronMessage(message);
  }
  else {
    // We're expecting this to be a debug message
    while (Serial.available()) {
      char inChar = (char)Serial.read();
      Serial.print("I received: ");
      Serial.println(inChar);
    }
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
