/************************************************************************
 * 
 * HoneywellEncoder.cpp
 * This is an Arduino library for handling Honeywell HOA0902-style rotary encoders.
 * 
 * Author: Cameron Stewart
 * Date: 2025-03-20
 * http://github.com/stewartsoda/
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */


/************************************************************************
 * 
 * Future improvements:
 * TODO: Add support for interrupts on tach and dir pins
 * TODO: Add enum/type for direction, handle unknown direction
 * TODO: Integrate with motor control to handle direction and encoder mask/gap finding
 * 
 * */


/************************************************************************
 * 
 * Method of operation:
 * 
 * The HOA0902 is a rotary encoder that provides two output signals:
 * 1. A tachometer signal (tach) that pulses as the encoder rotates.
 *    The tach signal is active low and has a frequency proportional to the speed of rotation.
 *    The tach signal pulses low twice for each step of the encoder.
 *    The tach signal low-pulse duration is 3-20 microseconds.  
 * 2. A direction signal (dir) that indicates the direction of rotation.
 * 
 * The applicable waveform is as follows, and is different than standard quadrature encoders,
 * though it can be related to the a and b quadrature signals:
 *  
 *          from a to b direction  pause-no-mask  from b to a direction
 *  mask:        1       2       3                3       2       1       
 *              ___     ___     ___              ___     ___     ___
 *  a:      ___/   *___/   *___/   *____________/   L___/   L___/   L___
 *               ___     ___     ___            ___     ___     ___     
 *  b:      \___/   \___/   \___/   \__________/   *___/   *___/   *___/
 *  states:   
 *                                                   ___________________
 *  dir:    ________________________________________/
 *          ___   _   _   _   _   _   __________   _   _   _   _   _   _
 *  tach:      \_/ \_/ \_/ \_/ \_/ \_/          \_/ \_/ \_/ \_/ \_/ \_/
 *  pos:    0  1   2   3   4   5   6            7   6   5   4   3   2
 * 
 * 
 * Previous movement was in A direction, pause with mask clear, move to B direction
 *  mask:   0g1  1  1g2  2   2g3   2  2g1  1  1g0
 *              ___     ___        ___     ___   
 *  a:      ___/   \___/   \______/   \___/   \___
 *               ___     ___      ___     ___         
 *  b:      ____/   \___/   \____/   \___/   \____
 *  states:   
 *                                     ___________   dir change always comes on an edge of a
 *  dir:    __________________________/              in theory can wiggle to find gap
 *          ___   _   _   _   ____   _   _   _   _
 *  tach:      \_/ \_/ \_/ \_/    \_/ \_/ \_/ \_/
 *  +/-     =  +   +   +   +     X+X  -   -   -
 *  pos:    0  1   2   3   4      5   4   3   2     **** +2 DISCREPANCY HERE ****
 *  corr:   0  1   2   3   4      3   2   1   0
 * 
 * Previous movement was in B direction, pause with mask clear, move to A direction
 *  mask:  1g0 0   0g-1 -1 -1g-2 -1  -1g0 0  0g1
 *              ___     ___      ___     ___     
 *  a:      ___/   \___/   \____/   \___/   \_____
 *             ___     ___        ___     ___         
 *  b:      __/   \___/   \______/   \___/   \____
 *  states:   
 *          ________________________
 *  dir:                            \_____________
 *          ___   _   _   _   __   _   _   _   ___
 *  tach:      \_/ \_/ \_/ \_/  \_/ \_/ \_/ \_/ 
 *  pos:    2  1   0   -1  -2   -3  -2  -1  0       **** -2 DISCREPANCY HERE ****
 *  corr:   0  1   2   3   4    3   2   1   0
 * 
 * Previous movement was in A direction, pause with mask occluded, move to B direction
 *  mask:   0g1  1  1g2  2  2g3  3   3g2 2  2g1  1   1g0
 *              ___     ___     _____     ___     ___
 *  a:      ___/   \___/   \___/     \___/   \___/   \___
 *               ___     ___     ___     ___     ___     
 *  b:      ____/   \___/   \___/   \___/   \___/   \____
 *  states:   
 *                                    ___________________
 *  dir:    _________________________/
 *          ___   _   _   _   _   ___   _   _   _   _   _
 *  tach:      \_/ \_/ \_/ \_/ \_/   \_/ \_/ \_/ \_/ \_/
 *  pos:    0  1   2   3   4   5     4   3   2   1   0
 * 
 * Previous movement was in B direction, pause with mask occluded, move to A direction
 *  mask:   1g0  0 0g-1 -1 -1g-2 -2 -2g-1 -1 -1g0 0  0g1
 *               ___     ___     ___     ___     ___     
 *  a:      ____/   \___/   \___/   \___/   \___/   \____
 *              ___     ___     _____     ___     ___
 *  b:      ___/   \___/   \___/     \___/   \___/   \___
 *  states:   
 *          ________________________
 *  dir:                            \____________________
 *          ____   _   _   _   _   _   _   _   _   _   __
 *  tach:       \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/
 *  pos:    0   -1  -2  -3  -4  -5  -4  -3  -2  -1  0
 * 
 * Scenario: mask 1, go A+1, pause in gap 2g3, go B-1, pause in mask, go a+1, pause in gap, go b-1
 *  mask:      1   1g2  2  2g3*  2   2g1 1  *  1g2  2  2g3 *  2   2g1  1
 *          ______     ___        ___     ____     ___        ___     ______
 *  a:            \___/   \______/   \___/    \___/   \______/   \___/
 *          _______     ___      ___     ______     ___      ___     _______
 *  b:             \___/   \____/   \___/      \___/   \____/   \___/
 *                |        |         |________|       |          |__________
 *  dir:    _________________________/        \__________________/
 *          ______   _   _   ____   _   _   __   _   _   ____   _   _   ____
 *  tach:         \_/ \_/ \_/    \_/ \_/ \_/  \_/ \_/ \_/    \_/ \_/ \_/
 *  pos:    0     1   2   3      4   3   2    3   4   5      6   5   4   
 * 
 *  correct: 0    1   2   3      4   1   0    1   2   3      4   1   0
 * 
 * Have to keep track of the current count, the last direction
 *  Last direction  | New direction | Last count | NEW COUNT
 *  Low             | Low           | n          | n+1 
 *  Low             | High          | n          | n-1
 * 
 * 
 * a and b go low when lit
 * 
 * States are as follows:
 *            _   _                           _   _ 
 * a: _   /           \   _   _   _   _   /           \   _
 *                _   _                   _   _
 * b: _   _   /           \   _   _   /           \   _   _
 * 
 * s: 0   ra  RB  1   FA  fb  0   0   rb  RA  1   FB  fa  0
 * 
 * State |  a  |  b  | newdir | tach |  Note
 * 0     |  0  |  0  | olddir |  1   |  mask clear of a and b (both LEDs lit)
 * ra    |  _/ |  0  | olddir | \_/  |  mask clear of b, begins to occlude a
 * RB    |  1  | _/  | olddir |  1   |  mask occludes a, begins to occlude b
 * 1     |  1  |  1  | olddir |  1   |  mask occludes a and b
 * FA    |  \_ |  1  |   0    | \_/  |  mask occludes b, begins to clear a          a       b           a       b
 * fb    |  0  |  \_ | olddir |  1   |  mask clear of a, begins to clear b        XXXXXXXXXXXXXX  ->       XXXXXXXXXXXXXX
 * rb    |  0  |  _/ | olddir |  1   |  mask clear of a, begins to occlude b
 * RA    |  _/ |  1  | olddir | \_/  |  mask occludes b, begins to occlude a
 * FB    |  1  |  \_ | olddir |  1   |  mask occludes a, begins to clear b
 * fa    |  \_ |  0  |   1    | \_/  |  mask clear of b, begins to clear a          a       b           a       b
 *                                                                             XXXXXXXXXXXX     -> XXXX
 * 
 * State transition table:
 * State |   Possible new states
 * 0     | 0, ra, rb
 * ra*   | (10), RB, fa
 * RB    | 1
 * 1     | 1, FA, FB
 * FA*   | (01), fb, RA
 * fb    | 0
 * rb    | (01), fb, RA
 * RA*   | 1
 * FB    | (10), RB, fa
 * fa*   | 0

 * Unknowns: There is an apparrent one-pulse delay in the direction signal. It is unclear
 * if this is symmetrical across direction changes. From the functional circuit diagram
 * it appears that direction is latched when there is a change on a, which is also when
 * the tach pulse is generated.
 * 
 * Have to establish an initial count and whether it's a count on a gap or a count on a mask
 * Tach is essentially an edge detector on A
 * Go clockwise until we get four tach pulses, then stop. Check dir.
 *  We know we just passed an A edge, so we just moved through RA, ra, FA, or fa.
 *  If dir = 1, we know it can't be FA, so we are in ra, RA, or fa.
 * Go counterclockwise until we get a tach pulse, then stop.
 * 
 */



#include "Arduino.h"
#include "HoneywellEncoder.h"

/**
 * @brief Constructs a new HoneywellEncoder object.
 * 
 * This constructor initializes a HoneywellEncoder object with the specified tachometer and direction pins.
 * It also sets the initial position to 0 and the direction state to LOW.
 * 
 * @param tachPin The pin number connected to the tachometer output of the encoder.
 * @param dirPin The pin number connected to the direction output of the encoder.
 */
HoneywellEncoder::HoneywellEncoder(int tachPin, int dirPin) {
    this->_tachPin = tachPin;
    this->_dirPin = dirPin;
    this->_position = 0;
    this->_dirState = LOW;
}

/**
 * @brief Initializes the Honeywell Encoder by setting up the tachometer and direction pins.
 * 
 * This function configures the tachometer pin (_tachPin) and the direction pin (_dirPin) as input pins.
 * It should be called during the setup phase to prepare the encoder for operation.
 */
void HoneywellEncoder::begin() {
    pinMode(_tachPin, INPUT);
    pinMode(_dirPin, INPUT);    
}

/**
 * @brief Sets the Honeywell Encoder to the specified position.
 * 
 * This function sets the current position of the encoder to the specified value.
 * It is important to call this function to set the encoder's position to some known value before using it.
 * 
 * @param newPosition The new position to set for the encoder.
 */
void HoneywellEncoder::setPosition(long newPosition) {
    noInterrupts();
    _position = newPosition;
    interrupts();
}

/**
 * @brief Returns the current position of the Honeywell Encoder.
 * 
 * This function retrieves the current position of the encoder.
 * 
 * @return long The current position of the encoder.
 */
long HoneywellEncoder::getPosition() {
    noInterrupts();
    long currentPosition = _position;
    interrupts();
    return currentPosition;
}

/**
 * @brief Updates the position of the Honeywell Encoder based on the tachometer signal.
 * 
 * This function updates the position of the encoder based on the current direction of motion.
 * It should be called when the tachometer signal pulses from high to low.
 */
void HoneywellEncoder::tachTick() {
    if (_dirState == HIGH) {
        _position++;
    } else {
        _position--;
    }
}

/**
 * @brief Reads the current state of the direction pin and updates the direction state.
 * 
 * This function reads the current state of the direction pin and updates the internal direction state variable.
 * It should be called when the direction signal changes.
 */
void HoneywellEncoder::dirChange() {
    _dirState = digitalRead(_dirPin);
}

