/*
 * Max517Dac.cpp
 *
 * MAX517 8-bit DAC library implimentation.
 *
 * Copyright (c) 2012 Jordan Goulder. All rights reserved.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not see <http://www.gnu.org/licenses/>.
 */
 
#include <Wire.h>
#include "Max517Dac.h"

const uint8_t CMD_RESET       = 0x10;
const uint8_t CMD_SET_OUTPUT  = 0x00;
const uint8_t CMD_POWER_DOWN  = 0x08;
const uint8_t CMD_POWER_UP    = 0x00;

Max517Dac::Max517Dac(uint8_t address)
{
   mAddress = address;
   Wire.begin();
}

void
Max517Dac::resetOutput(bool powerDownMode) const
{
   uint8_t command = CMD_RESET;

   if (powerDownMode)
   {
      command |= CMD_POWER_DOWN;
   }
  
   mSendCommand(command);
}

void
Max517Dac::setOutput(uint8_t output, bool powerDownMode) const
{
   uint8_t command = CMD_SET_OUTPUT;
  
   if (powerDownMode)
   {
      command |= CMD_POWER_DOWN;
   }
  
   mSendCommand(command, output, true);
}

void
Max517Dac::powerDown() const
{  
   mSendCommand(CMD_POWER_DOWN);
}

void
Max517Dac::powerUp() const
{  
   mSendCommand(CMD_POWER_UP);
}

void
Max517Dac::mSendCommand(uint8_t command, uint8_t output, bool sendOutput) const
{
   Wire.beginTransmission(mAddress);
   Wire.write(command);

   if (sendOutput)
   {
      Wire.write(output);
   }

   Wire.endTransmission();
}
