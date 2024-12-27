#ifndef _utils_h_
#define _utils_h_


//Function prototypes
uint8_t generateChecksum(char *message);
uint8_t checkChecksum(char *message, char checksum);
uint8_t decodeCelestronMessage(char *message);
uint8_t handleCommand(char source, char dest, char command, char* data);
uint8_t sendCommand(char source, char dest, char command, char *data);

#endif