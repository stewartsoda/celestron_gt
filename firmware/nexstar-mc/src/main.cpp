#include "Arduino.h"
#include "stdint.h"

#include "defines.h"
#include "celestron.h"
#include "utils.h"
#include "globals.h"
#include "libs.h"
#include "pins.h"

//#include "encod.h"

#define DEBUG true


////////////////////////////////////////
// Global Variables
////////////////////////////////////////

/* LEDs */
Adafruit_DotStar led(DOTSTAR_NUM, PIN_DOTSTAR_DATA, PIN_DOTSTAR_CLK, DOTSTAR_BRG);
uint8_t brightness = 0;
bool goingup=true;

/* Encoders */
HoneywellEncoder altenc(PIN_ALT_STEP, PIN_ALT_DIR);
HoneywellEncoder azienc(PIN_AZI_STEP, PIN_AZI_DIR);

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
  //u8g2.print(altenc.read());
  u8g2.setCursor(0,42);
  u8g2.print("Azi: ");
  //u8g2.print(azienc.read());
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
  altenc.begin();
  azienc.begin();
  
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

void sendReplyCelestronMessage(celestronMessage_t *msg) {
  // Send the message back to the source
  Serial1.write(msg->preamble);
  Serial1.write(msg->length);
  Serial1.write(msg->source);
  Serial1.write(msg->dest);
  Serial1.write(msg->command);
  for (int i = 0; i < msg->length - 3; i++) {
    Serial1.write(msg->data[i]);
  }
  Serial1.write(msg->checksum);
}

void serialEvent() {
  // if first char is 0x3b
  // then if there are at least 6 bytes
  // then if the checksum is correct
  // then it is a valid Celestron message
  // else wait for another byte
  // else wait for another byte, or too many bytes
  // else it's a debug message

  //don't strip the first byte, if it's not a Celestron message we may need it
  if (Serial.peek() == 0x3b) {
    celestronMessage_t msg;
    // We're expecting this to be a Celestron message
    delay(5); // this should be enough time for a 10-byte message at 9600 bps
    msg.preamble = Serial.read();
    msg.length = Serial.read();
    msg.source = (CelestronDevice_t)Serial.read();
    msg.dest = (CelestronDevice_t)Serial.read();
    msg.command = Serial.read();
    if (msg.length > 3) {
      for (int i = 0; i < msg.length - 3; i++) {
        msg.data[i] = Serial.read();
      }
    }
    msg.checksum = Serial.read();
    handleCelestronMessage(&msg);
    sendReplyCelestronMessage(&msg);
    Serial.println();
  }
  else {
    // We're expecting this to be a debug message
    String input;
    celestronMessage_t msg;
    while (Serial.available()) {
      //debug message that is like a celestronMessage (get FW version):
      //0x3b 0x03 0xff 0x10 0xfe 0xf0
      input = Serial.readString();
      Serial.print("I received: ");
      Serial.println(input);
  

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
