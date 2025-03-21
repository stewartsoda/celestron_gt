#ifndef _pins_h_
#define _pins_h_

////////////////////////////////////////
// Pin Definitions
////////////////////////////////////////

/* Encoder pins */
#define PIN_AZI_DIR 13
#define PIN_AZI_STEP 12
#define PIN_ALT_DIR 11
#define PIN_ALT_STEP 10

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

#endif // PINS_H