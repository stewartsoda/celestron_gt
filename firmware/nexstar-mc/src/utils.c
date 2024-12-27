#include "Arduino.h"
#include "stdint.h"

#include "defines.h"
#include "celestron.h"
#include "utils.h"

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