/************************************************************************
 * 
 * HoneywellEncoder.h
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

#ifndef _HoneywellEncoder_h_
#define _HoneywellEncoder_h_

#include "Arduino.h"

class HoneywellEncoder {
public:
    HoneywellEncoder(int tachPin, int dirPin);
    void begin();
    void setPosition(long newPosition);
    long getPosition();
    void tachTick();
    void dirChange();
private:
    int _tachPin;
    int _dirPin;
    volatile long _position;
    int _dirState;
};

#endif