# Design Notes
These are some notes relating to the design of this project.
## Inputs and Outputs
Here are some of the considerations of inputs and outputs.
### Physical Interface
The project board taps into the HC (Hand Controller) connector on the mount, which interfaces with the MC board. This is a unified communications bus inside the mount known as the Aux Bus. The HC connector is a 6P6C cable and has the following pins. See [this thread](https://www.cloudynights.com/topic/524901-nexstar-slt-controller-cabling/) for details.
1. GND (white)
2. TXD (black) (serial data from mount to HC)
3. GND (blue)
4. RXD (green) (serial data from HC to mount)
5. GND (yellow)
6. VDD (red) (+12V supply from mount)
The project board will have two 6P6C jacks. One of them will be connected to the HC connector on the mount. The HC may remain connected while the project board is connected by plugging into the other.
The VDD line of the Aux Bus will provide power to the project board.
