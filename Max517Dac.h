/*
   Max517Dac.h

   MAX517 8-bit DAC library for Arduino

   Copyright (c) 2012 Jordan Goulder. All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MAX517DAC_H
#define MAX517DAC_H

#include <Arduino.h>

class Max517Dac
{
   public:
      static const int DEFAULT_ADDRESS = 0x2C;

      Max517Dac(int address = DEFAULT_ADDRESS);
    
      void resetOutput(bool powerDown = false) const;
      void setOutput(int value, bool powerDown = false) const;
      void powerDown(void) const;
      void powerUp(void) const;

   private:
      int mAddress;
      void mSendCommand(int command, int output) const;
};

#endif
