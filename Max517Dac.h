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
