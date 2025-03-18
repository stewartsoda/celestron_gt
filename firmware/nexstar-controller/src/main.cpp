#include <Arduino.h>

/*
  We're replacing the Celestron HC and it's interface with the MC board.
  Step 1: Implement basic MC controls.
  Step 2: Implement basic USB-Serial PC interface.
  Step 3: Implement alignment method.
  Step 4: Implement some kind of planetarium control software interface (ASCOM?).
    1. Goto a position (ra_int, dec_int) - Slew to a given Ra/Dec position
    2. Sync a position (ra_int, dec_int) - Track a given Ra/Dec position
    3. Is sync supported?
    4. Abort slew
    5. Is abort slew supported?
    6. Is the telescope initialized? (not implemented)
*/

/*
  Board <-> MC needed suppported commands:
  For each axis:
    1. Get current position
    2. Set current position


*/

/*
  Communications protocol = 9600bps 8N1
*/

/*
  PC <-> Board needed supported commands:
    Get Version
    GoTo position (RA,DEC) = "r34AB0500,12CE0500" = "Go to this position", position is encoded as a fraction of a circle.
    Sync position (RA,DEC) = s34AB0500,12CE0500" = "you are pointing at this position"
    Get position = PC sends "e" and board responds "34AB0500,12CE0500", first four bytes are RA and last four bytes are DEC in fraction of a circle.
      So if the RA is 34AB0500, then the RA is 34AB0500/0xFFFFFFFF * 24 hours = 883,623,168/4,294,967,295 * 24 hours = 4.9376292240195044, or 4h 56m 15.48s
      So if the DEC is 12CE0500, then the DEC is 12CE0500/0xFFFFFFFF * 360 degrees etc.
    Abort
*/


// put function declarations here:
int myFunction(int, int);

// MC commands


void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}