#ifndef _utils_h_
#define _utils_h_


//Function prototypes
int checkChecksum(char *message, char checksum);
char generateChecksum(char *message);

int handleCommand(char source, char dest, char command, char* data);
void sendCommand(char source, char dest, char command, char *data);

#endif