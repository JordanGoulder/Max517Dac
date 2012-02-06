#ifndef MAX517DAC_H
#define MAX517DAC_H

#include <Arduino.h>

const int DEFAULT_ADDRESS = 0x2C;

class Max517Dac
{
   public:
      Max517Dac(int address = DEFAULT_ADDRESS);
    
      void resetOutput(bool powerDown = false);
      void setOutput(int value, bool powerDown = false);
      void powerDown(void);
      void powerUp(void);

   private:
      int mAddress;
      void mSendCommand(int command, int output);
};

#endif
