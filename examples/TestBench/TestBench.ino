/*
 * TestBench.ino
 *
 * This program tests the usage of the MAX517 8-bit DAC library.
 * The diagram below shows the pinout of the 8 pin DIP version of the 
 * device and its connections to the Arduino Uno.
 *
 *         -------.   .-------
 *        |        \_/        |
 *  A0 <--| OUT0         REF0 |<-- 5V
 *        |                   |
 * GND -->| GND   MAX517  VDD |<-- 5V
 *        |                   |
 *  A5 -->| SCL           AD0 |<-- GND
 *        |                   |
 *  A4 -->| SDA           AD1 |<-- GND
 *        |                   |
 *         ------------------- 
 *
 * Copyright (c) 2012 Jordan Goulder. All rights reserved.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Wire.h>
#include <Max517Dac.h>

const int   adcInputPin       = A0;    // Pin used to read analog input
int         adcInputValue     = 0;     // Value read form ADC
int         dacOutputValue    = 0;     // Value written to DAC
int         passCount         = 0;
int         failCount         = 0;

typedef enum testState_t
{
   TEST_STATE_INIT,
   TEST_STATE_POWER_ON_RESET,
   TEST_STATE_SET_OUTPUT,
   TEST_STATE_RESET_OUTPUT,
   TEST_STATE_POWER_DOWN,
   TEST_STATE_DONE
} testState_t;

testState_t testState = TEST_STATE_INIT;

// Create a new DAC object
Max517Dac   dac;

void setup()
{
   // Initialize the serial interface
   Serial.begin(115200);
}

void loop()
{
   switch (testState)
   {
      case TEST_STATE_INIT:
         testStateInit();
         testState = TEST_STATE_POWER_ON_RESET;
         break;

      case TEST_STATE_POWER_ON_RESET:
         testStatePowerOnReset();
         testState = TEST_STATE_SET_OUTPUT;
         break;

      case TEST_STATE_SET_OUTPUT:
         testStateSetOutput();
         testState = TEST_STATE_RESET_OUTPUT;
         break;

      case TEST_STATE_RESET_OUTPUT:
         testStateResetOutput();
         testState = TEST_STATE_POWER_DOWN;
         break;

      case TEST_STATE_POWER_DOWN:
         testStatePowerDown();
         testState = TEST_STATE_DONE;
         break;

      case TEST_STATE_DONE:
      default:
         testStateDone();
         testState = TEST_STATE_INIT;
         break;
   }
   waitForKey();
}

void waitForKey()
{
   Serial.println("Press any key...");
   while(Serial.available() == 0);
   while(Serial.available() > 0)
   {
      Serial.read();
   }
}

int sampleAdc()
{
   int sample = 0;
   sample += analogRead(adcInputPin);            
   sample >>= 2;
   return (sample);
}

void testStateInit()
{
   passCount = 0;
   failCount = 0;

   Serial.println("\n\nMAX517 Test Bench");
}

void testStatePowerOnReset()
{
   Serial.println("\nTesting power-on reset value.");

   adcInputValue = sampleAdc();

   Serial.print("Power-On Reset Value = ");
   Serial.println(adcInputValue, DEC);
   if (adcInputValue == 0)
   {
      passCount++;
      Serial.println("PASS");
   }
   else
   {
      failCount++;
      Serial.println("FAIL");
   }
}

void testStateSetOutput()
{
   Serial.println("\nTesting setOutput()");
   Serial.println("Setting DAC Output to 128");
   if (dac.setOutput(128))
   {
      delay(100);
      Serial.print("Reading ADC Input: ");
      adcInputValue = sampleAdc();
      Serial.println(adcInputValue, DEC);
    
      if (adcInputValue == 128)
      {
         passCount++;
         Serial.println("PASS");
      }
      else
      {
         failCount++;
         Serial.print("FAIL");
         Serial.println(" Input/Output Mismatch");
      }
   }
   else
   {
      failCount++;
      Serial.print("FAIL");
      Serial.println(" I2C Write Failed");
   }
}

void testStateResetOutput()
{
   Serial.println("\nTesting resetOutput()");
   Serial.println("Resetting DAC Output");
   if (dac.resetOutput())
   {
      delay(100);
      Serial.print("Reading ADC Input: ");
      adcInputValue = sampleAdc();
      Serial.println(adcInputValue, DEC);
    
      if (adcInputValue == 0)
      {
         passCount++;
         Serial.println("PASS");
      }
      else
      {
         failCount++;
         Serial.print("FAIL");
         Serial.println(" Input/Output Mismatch");
      }
   }
   else
   {
      failCount++;
      Serial.print("FAIL");
      Serial.println(" I2C Write Failed");
   }
}

void testStatePowerDown()
{
   Serial.println("\nTesting powerDown()");

   Serial.println("Setting DAC to full output.");
   dac.setOutput(255);

   Serial.print("Reading ADC Input: ");
   adcInputValue = sampleAdc();
   Serial.println(adcInputValue, DEC);

   Serial.println("Powering down DAC");
   dac.powerDown();

   Serial.println("Reading ADC Input: ");
   delay(1000);
   adcInputValue = sampleAdc();
   Serial.println(adcInputValue, DEC);

   Serial.println("Setting DAC to 128 but staying in power down mode.");
   dac.setOutput(128, true);

   Serial.println("Reading ADC Input: ");
   delay(100);
   adcInputValue = sampleAdc();
   Serial.println(adcInputValue, DEC);

   Serial.println("Powering up DAC");
   dac.powerUp();
   Serial.println("Reading ADC Input: ");
   delay(100);
   adcInputValue = sampleAdc();
   Serial.println(adcInputValue, DEC);
}

void testStateDone()
{
   Serial.println("");
   Serial.print(passCount, DEC);
   Serial.print(" of ");
   Serial.print(passCount + failCount);
   Serial.println(" tests passed.");

   Serial.println("\nReady to test again?");
}
