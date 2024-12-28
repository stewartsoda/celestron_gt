#ifndef _utils_h_
#define _utils_h_


//Function prototypes
int calculateChecksum(celestronMessage_t *message);

int checkChecksum(celestronMessage_t *message);
int updateChecksum(celestronMessage_t *message);

int handleCelestronMessage(celestronMessage_t *message);

#endif