/*
 * Max517Dac.cpp
 *
 * Copyright (c) 2012 Jordan Goulder. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <Wire.h>
#include "Max517Dac.h"

static const int NO_OUTPUT       = -1;
static const int CMD_RESET       = 0x10;
static const int CMD_SET_OUTPUT  = 0x00;
static const int CMD_POWER_DOWN  = 0x08;
static const int CMD_POWER_UP    = 0x00;

Max517Dac::Max517Dac(int address)
{
   mAddress = address;
   Wire.begin();
}

void
Max517Dac::resetOutput(bool powerDown) const
{
   int command = CMD_RESET;

   if (powerDown)
   {
      command |= CMD_POWER_DOWN;
   }
  
   mSendCommand(command, NO_OUTPUT);
}

void
Max517Dac::setOutput(int output, bool powerDown) const
{
   int command = CMD_SET_OUTPUT;
  
   if (powerDown)
   {
      command |= CMD_POWER_DOWN;
   }
  
   mSendCommand(command, output);
}

void
Max517Dac::powerDown(void) const
{  
   mSendCommand(CMD_POWER_DOWN, NO_OUTPUT);
}

void
Max517Dac::powerUp(void) const
{  
   mSendCommand(CMD_POWER_UP, NO_OUTPUT);
}

void
Max517Dac::mSendCommand(int command, int output) const
{
   Wire.beginTransmission(mAddress);
   Wire.write(byte(command));

   if (NO_OUTPUT != output)
   {
      Wire.write(byte(output));
   }

   Wire.endTransmission();
}
