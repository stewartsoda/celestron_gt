#include "Arduino.h"
#include "stdint.h"

#include "defines.h"
#include "celestron.h"
#include "utils.h"
#include "globals.h"

/**
 * @brief Generates a checksum for a given message.
 *
 * This function calculates a checksum for the provided message string.
 *
 * @param message A pointer to the NULL-terminated message string for which the checksum is to be generated.
 * @return The 1-byte calculated checksum as an int.
 */
int calculateChecksum(celestronMessage_t *message) {
  // 1. Add characters message->length, message->source, message->dest, message->command, and message->data[] (if any)
  // 2. Take two's complement
  // 3. Return the resulting value
  uint8_t checksum = 0;
  checksum += message->length;
  checksum += message->source;
  checksum += message->dest;
  checksum += message->command;
  for (int i = 0; i < message->length - 3; i++) {
    checksum += message->data[i];
  }
  checksum = ~checksum + 1;
  return checksum;
}

/**
 * @brief Checks if the checksum of the given message matches the provided checksum.
 *
 * This function calculates the checksum of the provided message and compares it
 * with the given checksum. If they match, the function returns 0 indicating success.
 * If they do not match, the function returns -1 indicating failure.
 *
 * @param message A pointer to the celestronMessage_t whose checksum needs to be verified.
 * @return uint8_t Returns 0 if the checksum matches, otherwise returns -1.
 */
int checkChecksum(celestronMessage_t *message) {
  // calculate checksum on that substring
  // compare to message[end]
  // if matches, return 0 (success)
  // if it doesn't match, return -1 (failure)
  if(calculateChecksum(message) == message->checksum) {
    return 0;
  }
  else {
    return 1;
  }
}

int updateChecksum(celestronMessage_t *message) {
  message->checksum = calculateChecksum(message);
  return 0;
}

/**
 * @brief Handles a message received from a Celestron device.
 *
 * This function processes a message received from a Celestron device and performs
 * the appropriate action based on the command contained within the message.
 * The message is modified in place to become the response message.
 * The function returns 1 if the command is invalid, otherwise it returns 0.
 *
 * @param message A pointer to the message received.
 *
 * @return uint8_t Returns 1 if the command is invalid, otherwise returns 0.
 *
 * The following commands are supported:
 * - MC_GET_POSITION: Send back the current position (24-bit signed fraction of full rotation).
 * - MC_GOTO_FAST: Move to the desired position with rate=9 (16 or 24-bit signed fraction of full rotation).
 * - MC_SET_POSITION: Set the current position (24-bit signed fraction of full rotation).
 * - MC_SET_POS_GUIDERATE: Set the positive guiderate (24 or 16 bits).
 * - MC_SET_NEG_GUIDERATE: Set the negative guiderate (24 or 16 bits).
 * - MC_LEVEL_START: Start the leveling process.
 * - MC_PEC_RECORD_START: Start the PEC recording process.
 * - MC_PEC_PLAYBACK: Start or stop PEC playback (8 bits: 0x01 = start, 0x00 = stop).
 * - MC_SET_POS_BACKLASH: Set positive backlash (8 bits: positive backlash value from 0-99).
 * - MC_SET_NEG_BACKLASH: Set negative backlash (8 bits: negative backlash value from 0-99).
 * - MC_IS_LEVEL_DONE: Check if the leveling process is done (0x00 if not done, 0xff if done).
 * - MC_IS_SLEW_DONE: Check if the slew process is done (0x00 if not done, 0xff if done).
 * - MC_IS_PEC_RECORD_DONE: Determine if PEC record is complete (0x00 if not done, 0xff if done).
 * - MC_PEC_RECORD_STOP: Stop the PEC recording process.
 * - MC_GOTO_SLOW: Move to desired position with slow, variable rate (16 or 24-bit signed fraction of full rotation).
 * - MC_AT_INDEX: Determine if axis is at an index marker (0x00 if not at index, 0xff if at index).
 * - MC_SEEK_INDEX: Move to the nearest index marker.
 * - MC_MOVE_POS: Move in the positive (up/right) direction at a fixed rate (8 bits: rate from 0-9, 0 = stop).
 * - MC_MOVE_NEG: Move in the negative (down/left) direction at a fixed rate (8 bits: rate from 0-9, 0 = stop).
 * - MC_ENABLE_CORDWRAP: Enable cordwrap.
 * - MC_DISABLE_CORDWRAP: Disable cordwrap.
 * - MC_IS_CORDWRAP_ENABLED: Check if cordwrap is enabled (0x00 if not enabled, 0xff if enabled).
 * - MC_GET_CORDWRAP_POSITION: Get the cordwrap position (24-bit position).
 * - MC_GET_POS_BACKLASH: Get the positive backlash (8-bit positive backlash value).
 * - MC_GET_NEG_BACKLASH: Get the negative backlash (8-bit negative backlash value).
 * - MC_SET_AUTOGUIDE_RATE: Set the autoguide rate (8 bits: 100 * rate / 256 = percentage of sidereal rate).
 * - MC_GET_AUTOGUIDE_RATE: Get the autoguide rate (100 * rate / 256 = percentage of sidereal rate).
 * - MC_GET_APPROACH: Get the approach direction (0x00 for positive, 0x01 for negative).
 * - MC_SET_APPROACH: Set the approach direction (8 bits: 0x00 for positive, 0x01 for negative).
 * - MC_GET_VERSION: Get the firmware version (16 bits 0x1234 where the version is 12.34).
 */
int handleCelestronMessage(celestronMessage_t *message) {
  CelestronDevice sourceDevice = message->source;
  CelestronDevice destinationDevice = message->dest;
  message->source = destinationDevice;
  message->dest = sourceDevice;

  int result = 1;
  switch (message->command) {
    case MC_GET_POSITION:
    //Send back the current position
    //Returns 24 bit position, signed fraction of full rotation
      switch (message->dest) {
        case ALT_MOTOR_BOARD:
          // send back the current position of the alt motor
          
          break;
        case AZI_MOTOR_BOARD:
          // send back the current position of the azi motor
          break;
        default:
          // invalid destination
          break;
      }
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
    //Returns 16 bits e.g. 0x1234 where the version is 12.34
    message->length = 0x05;
    message->data[0] = 0x12;
    message->data[1] = 0x34;
    result = 0;
    break;
    default:
    //Invalid command
    result = 1;
    break;
    message->checksum = calculateChecksum(message);

    return result;
  }  








  return result;
}

