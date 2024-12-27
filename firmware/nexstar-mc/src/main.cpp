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
/**
 * @brief Generates a checksum for a given message.
 *
 * This function calculates a checksum for the provided message string.
 *
 * @param message A pointer to the message string for which the checksum is to be generated.
 * @return The calculated checksum as a char.
 */
char generateChecksum(char *message) {
  // 1. Add characters from message[0] to message[end]
  // 2. Take two's complement
  // 3. Return the resulting value
  // Note- there is no error checking, the input is assumed to have at sufficient characters
  uint8_t checksum = 0;
  for(int i=0; i<sizeof(message); i++) {
    checksum += message[i];
  }
  checksum = ~checksum + 1;
  return checksum;
}

int checkChecksum(char *message, char checksum) {
  // calculate checksum on that substring
  // compare to message[end]
  // if matches, return 0 (success)
  // if it doesn't match, return -1 (failure)
  //char substring[strlen(message)-2];
  //strncpy(substring, message+1, strlen(message)-2);
  if(generateChecksum(message) == checksum) {
    return 0;
  }
  else {
    return -1;
  }
}

int handleCommand(char source, char dest, char command, char* data) {
    int result = 0;
  switch (command) {
    case MC_GET_POSITION:
    //Send back the current position
    //Returns 24 bit position, signed fraction of full rotation
    break;
    case MC_GOTO_FAST:
    //Move to the desired position with rate=9
    //Data is 16 or 24 bit desired position, signed fraction of a full rotation
    //Returns ack
    break;
    case MC_SET_POSITION:
    //Set the current position
    //Data is 24 bit position, signed fraction of a full rotation
    //Returns ack
    break;
    case MC_SET_POS_GUIDERATE:
    //Set the positive guiderate
    //Data is 24 or 16 bits:
    // 24 bits: rate
    // 16 bits: 0xffff = sidereal, 0xfffe = solar, 0xfffd = lunar
    //Returns ack
    break;
    case MC_SET_NEG_GUIDERATE:
    //Set the negative guiderate
    //Data is 24 or 16 bits:
    // 24 bits: rate
    // 16 bits: 0xffff = sidereal, 0xfffe = solar, 0xfffd = lunar
    //Returns ack
    break;
    case MC_LEVEL_START:
    //Start the leveling process
    //Returns ack
    break;
    case MC_PEC_RECORD_START:
    //Start the PEC recording process
    //Returns ack
    break;
    case MC_PEC_PLAYBACK:
    //Start or stop PEC playback
    //Data is 8 bits: 0x01 = start, 0x00 = stop
    //Returns ack
    break;
    case MC_SET_POS_BACKLASH:
    //Set positive backlash
    //Data is 8 bits: positive backlash value from 0-99
    //Returns ack
    break;
    case MC_SET_NEG_BACKLASH:
    //Set negative backlash
    //Data is 8 bits: negative backlash value from 0-99
    //Returns ack
    break;
    case MC_IS_LEVEL_DONE:
    //Check if the leveling process is done
    //Returns 0x00 if not done, 0xff if done
    break;
    case MC_IS_SLEW_DONE:
    //check if the slew process is done
    //Returns 0x00 if not done, 0xff if done
    break;
    case MC_IS_PEC_RECORD_DONE:
    //Determine if PEC record is complete 
    //Returns 0x00 if not done, 0xff if done
    break;
    case MC_PEC_RECORD_STOP:
    //Stop the PEC recording process
    //Returns nothing
    break;
    case MC_GOTO_SLOW:
    //Move to desired position with slow, variable rate
    //Data is 16 or 24 bit desired position, signed fraction of a full rotation
    //Returns ack
    break;
    case MC_AT_INDEX:
    //Determine if axis is at an index marker
    //returns 0x00 if not at index, 0xff if at index
    break;
    case MC_SEEK_INDEX:
    //Move to the nearest index marker
    //Returns nothing
    break;
    case MC_MOVE_POS:
    //Move in the positive (up/right) direction at a fixed rate
    //Data is 8 bits: rate from 0-9, 0 = stop
    break;
    case MC_MOVE_NEG:
    //Move in the negative (down/left) direction at a fixed rate
    //Data is 8 bits: rate from 0-9, 0 = stop
    break;
    case MC_ENABLE_CORDWRAP:
    //Enable cordwrap
    //Returns nothing
    break;
    case MC_DISABLE_CORDWRAP:
    //Disable cordwrap
    //Returns nothing
    break;
    case MC_IS_CORDWRAP_ENABLED:
    //Check if cordwrap is enabled
    //Returns 0x00 if not enabled, 0xff if enabled
    break;
    case MC_GET_CORDWRAP_POSITION:
    //Get the cordwrap position
    //Returns 24 bit position
    break;
    case MC_GET_POS_BACKLASH:
    //Get the positive backlash
    //Returns the 8 bit positive backlash value
    break;
    case MC_GET_NEG_BACKLASH:
    //Get the negative backlash
    //Returns the 8 bit negative backlash value
    break;
    case MC_SET_AUTOGUIDE_RATE:
    //Set the autoguide rate
    //Data is 8 bits: 100 * rate / 256 = percentage of sidereal rate
    //Returns ack
    case MC_GET_AUTOGUIDE_RATE:
    //Get the autoguide rate
    //Returns the autoguide rate, 100 * rate / 256 = percentage of sidereal rate
    break;
    case MC_GET_APPROACH:
    //Get the approach direction
    //Returns 0x00 for positive, 0x01 for negative
    break;
    case MC_SET_APPROACH:
    //Set the approach direction
    //Data is 8 bits: 0x00 for positive, 0x01 for negative
    //Returns ack
    case MC_GET_VERSION:
    //Get the firmware version
    //Returns 16 bits 0x1234 where the version is 12.34
    break;
    default:
    //Invalid command
    result = 1;
    break;
  }  
  return result;
}

void sendCommand(char source, char dest, char command, char *data) {
  //send the command to the destination
  //data is optional
  return;
}

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

int decodeCelestronMessage(char *message) {
  char len = message[0];
  char source = message[1];
  char dest = message[2];
  char command = message[3];
  char data[3];
  int result = 1;

  if (len > 3) {
    for (int i = 0; i < len-4; i++) {
      data[i] = message[i+4];
    }
    result = handleCommand(source, dest, command, data);
  }
  else {
    result = handleCommand(source, dest, command, NULL);
  }
  return result;
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

/* 
  Need the following ISRs:
  1. Update encoder position
  2. Update desired position
  3. Receive serial input
  4. Print the screen

  Need the following functions

*/

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
