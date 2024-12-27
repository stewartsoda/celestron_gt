# Firmware Notes

## Design Goals
For this project, the firmware needs to do the following:
1. Receive serial input from Serial and Serial1 (interrupt)
2. Decode input from Serial and Serial1 into Celestron messages and debug commands
3. Craft response Celestron messages
4. Respond to Celestron commands on Serial and Serial1
5. Update current encoder positions (interrupt)
6. Issue motor commands for slewing/moving
7. Update desired position for tracking (timer/interrupt)
