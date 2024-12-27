/////////////////////////////////////////////////////////////////////////
// File:        celestron.h
// Contents:    Definitions for Celestron commands and devices
// Author:      Cameron Stewart
// Date:        2024-12-23
// Copyright:   Cameron Stewart
/////////////////////////////////////////////////////////////////////////

#ifndef _celestron_h_
#define _celestron_h_


/* Devices */
enum CelestronDevice {
    MAIN_BOARD = 0x01,
    GT_HAND_CONTROLLER = 0x04,
    PLUS_HAND_CONTROLLER = 0x0d,
    AZI_MOTOR_BOARD = 0x10,
    ALT_MOTOR_BOARD = 0x11,
    GPS_INTERFACE = 0xb0,
    REALTIMECLOCK = 0xb2
};
typedef enum CelestronDevice CelestronDevice_t;

/* AUX Bus commands */
enum CelestronMotorCommand {
    MC_GET_POSITION = 0x01,
    MC_GOTO_FAST = 0x02,
    MC_SET_POSITION = 0x04,
    MC_SET_POS_GUIDERATE = 0x06,
    MC_SET_NEG_GUIDERATE = 0x07,
    MC_LEVEL_START = 0x0b,
    MC_PEC_RECORD_START = 0x0c,
    MC_PEC_PLAYBACK = 0x0d,
    MC_SET_POS_BACKLASH = 0x10,
    MC_SET_NEG_BACKLASH = 0x11,
    MC_IS_LEVEL_DONE = 0x12,
    MC_IS_SLEW_DONE = 0x13,
    MC_UNKNOWN = 0x14,
    MC_IS_PEC_RECORD_DONE = 0x15,
    MC_PEC_RECORD_STOP = 0x16,
    MC_GOTO_SLOW = 0x17,
    MC_AT_INDEX = 0x18,
    MC_SEEK_INDEX = 0x19,
    MC_MOVE_POS = 0x24,
    MC_MOVE_NEG = 0x25,
    MC_MOVE_PULSE = 0x26,
    MC_IS_MOVE_PULSE_DONE = 0x27,
    MC_ENABLE_CORDWRAP = 0x38,
    MC_DISABLE_CORDWRAP = 0x39,
    MC_SET_CORDWRAP_POS = 0x3a,
    MC_IS_CORDWRAP_ENABLED = 0x3b,
    MC_GET_CORDWRAP_POSITION = 0x3c,
    MC_GET_POS_BACKLASH = 0x40,
    MC_GET_NEG_BACKLASH = 0x41,
    MC_SET_AUTOGUIDE_RATE = 0x46,
    MC_GET_AUTOGUIDE_RATE = 0x47,
    MC_PROGRAM_ENTER = 0x81,
    MC_PROGRAM_INIT = 0x82,
    MC_PROGRAM_DATA = 0x83,
    MC_PROGRAM_END = 0x84,
    MC_GET_APPROACH = 0xfc,
    MC_SET_APPROACH = 0xfd,
    MC_GET_VERSION = 0xfe
};
typedef enum CelestronMotorCommand CelestronMotorCommand_t;

/* Original Nexstar GT HC commands */


/* Command Structure */
struct celestronMessage
{
    char preamble;
    char length;
    CelestronDevice_t source;
    CelestronDevice_t dest;
    char command;
    char data[3];
    char checksum;
};
typedef struct celestronMessage celestronMessage_t;

#endif