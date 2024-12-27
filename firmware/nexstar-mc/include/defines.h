//////////////////////////////////////////////////////////////////////////////////////////////
// Defines used everywhere
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DEFINES_H_
#define _DEFINES_H_

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
// LED Definitions
////////////////////////////////////////
#define BRIGHTNESS_MAX 16
#define BRIGHTNESS_MIN 0
#define LED_DELAY 10

////////////////////////////////////////
// Screen Definitions
////////////////////////////////////////
#define SCREEN_FONT u8g2_font_timB14_tf
#define SCREEN_LINE_1 0,15
#define SCREEN_LINE_2 0,31
#define SCREEN_LINE_3 0,47
#define SCREEN_LINE_4 0,63

////////////////////////////////////////
// PID Definitions
////////////////////////////////////////



#endif