#include <Wire.h>
#include "Max517Dac.h"

const int NO_OUTPUT       = -1;
const int CMD_RESET       = 0x10;
const int CMD_SET_OUTPUT  = 0x00;
const int CMD_POWER_DOWN  = 0x08;
const int CMD_POWER_UP    = 0x00;

Max517Dac::Max517Dac(int address)
{
   mAddress = address;
   Wire.begin();
}

void
Max517Dac::resetOutput(bool powerDown)
{
   int command = CMD_RESET;

   if (powerDown)
   {
      command |= CMD_POWER_DOWN;
   }
  
   mSendCommand(command, NO_OUTPUT);
}

void
Max517Dac::setOutput(int output, bool powerDown)
{
   int command = CMD_SET_OUTPUT;
  
   if (powerDown)
   {
      command |= CMD_POWER_DOWN;
   }
  
   mSendCommand(command, output);
}

void
Max517Dac::powerDown(void)
{  
   mSendCommand(CMD_POWER_DOWN, NO_OUTPUT);
}

void
Max517Dac::powerUp(void)
{  
   mSendCommand(CMD_POWER_UP, NO_OUTPUT);
}

void
Max517Dac::mSendCommand(int command, int output)
{
   Wire.beginTransmission(mAddress);
   Wire.write(byte(command));

   if (NO_OUTPUT != output)
   {
      Wire.write(byte(output));
   }

   Wire.endTransmission();
}
