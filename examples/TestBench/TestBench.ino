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

#include <stdlib.h>
#include <Wire.h>
#include <Max517Dac.h>

const int      adcInputPin    = A0;    // Pin used to read analog input
const uint8_t  adcError       = 1;     // Acceptable ADC read error when comparing
                                       // to the output DAC
int         adcInputValue     = 0;     // Value read form ADC
int         dacOutputValue    = 0;     // Value written to DAC
int         passCount         = 0;     // Count of the number tests that passed
int         failCount         = 0;     // Count of the number of tests that failed

// Test States
typedef enum testState_t
{
   TEST_STATE_INIT,
   TEST_STATE_POWER_ON_RESET,
   TEST_STATE_SET_OUTPUT,
   TEST_STATE_RESET_OUTPUT,
   TEST_STATE_POWER_DOWN,
   TEST_STATE_DONE
} testState_t;

// Start out in the INIT state
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
   // Call the correct test based on the state
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

   // Wait for the user to press a key before continuing
   waitForKey();
}

// Wait until the user presses a key
void waitForKey()
{
   // Let the user know we are waiting for a key
   Serial.println("Press any key...");
   // Wait for input
   while(Serial.available() == 0);
   // Read all the input out of the buffer
   while(Serial.available() > 0)
   {
      Serial.read();
   }
}

// Compare two values accounting for error
bool compareWithError(uint8_t a, uint8_t b, uint8_t allowableError)
{
   return (abs(a - b) <= allowableError);
}

// Sample the ADC device and convert it to an 8-bit value
int sampleAdc()
{
   int sample = 0;
   // Read the sample
   sample += analogRead(adcInputPin);            
   // Covert to 8-bits
   sample >>= 2;
   return (sample);
}

// Init things before starting tests
void testStateInit()
{
   // Clear out pass/fail counts
   passCount = 0;
   failCount = 0;

   Serial.println("\n\nMAX517 DAC Library Test Bench");
}

// Test the Power-On Reset value
// This should be zero
void testStatePowerOnReset()
{
   Serial.println("\nTesting power-on reset value.");

   adcInputValue = sampleAdc();

   Serial.print("Power-On Reset Value = ");
   Serial.println(adcInputValue, DEC);
   if (compareWithError(0, adcInputValue,  adcError))
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

// Test the setOutput function
//
// The ADC should read the same value as was set to the output
void testStateSetOutput()
{
   uint8_t dacOutput;

   Serial.println("\nTesting setOutput()");
   dacOutput = 128;
   Serial.print("Setting DAC Output to ");
   Serial.println(dacOutput, DEC);
   if (dac.setOutput(dacOutput))
   {
      delay(100);
      Serial.print("Reading ADC Input: ");
      adcInputValue = sampleAdc();
      Serial.println(adcInputValue, DEC);
    
      if (compareWithError(dacOutput, adcInputValue, adcError))
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

// Test the resetOutput fucntion
//
// The DAC output should go to zero and the ADC should read zero
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
    
      if (compareWithError(0, adcInputValue, adcError))
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

// Test the powerDown/powerUp functionality
//
// The output should float during power down and anything that sets
// the DAC should not affect the output until power up
void testStatePowerDown()
{
   uint8_t dacOutput;

   Serial.println("\nTesting powerDown()");

   Serial.println("Setting DAC to full output.");
   dacOutput = 255;
   if (!dac.setOutput(dacOutput))
   {
      Serial.print("FAIL");
      Serial.println(" I2C Write Failed");
      failCount++;
      return;
   }

   delay(100);
   Serial.print("Reading ADC Input: ");
   adcInputValue = sampleAdc();
   Serial.println(adcInputValue, DEC);
   if (!compareWithError(dacOutput, adcInputValue, adcError))
   {
      Serial.print("FAIL");
      Serial.println(" Input/Output mismatch");
      failCount++;
      return;
   }

   Serial.println("Powering down DAC");
   if(!dac.powerDown())
   {
      Serial.print("FAIL");
      Serial.println(" I2C Write Failed");
      failCount++;
      return;
   }

   delay(500);
   Serial.print("Reading ADC Input: ");
   adcInputValue = sampleAdc();
   Serial.println(adcInputValue, DEC);
   if (compareWithError(dacOutput, adcInputValue, adcError))
   {
      Serial.print("FAIL");
      Serial.println(" Input/Output mismatch");
      failCount++;
      return;
   }

   Serial.println("Resetting DAC output, but staying in power down mode.");
   if(!dac.resetOutput(true))
   {
      Serial.print("FAIL");
      Serial.println(" I2C Write Failed");
      failCount++;
      return;
   }

   delay(100);
   Serial.print("Reading ADC Input: ");
   adcInputValue = sampleAdc();
   Serial.println(adcInputValue, DEC);
   if (compareWithError(0, adcInputValue, adcError))
   {
      Serial.print("FAIL");
      Serial.println(" Input/Output mismatch");
      failCount++;
      return;
   }

   dacOutput = 128;
   Serial.print("Setting DAC to ");
   Serial.print(dacOutput);
   Serial.println(", but staying in power down mode.");
   if(!dac.setOutput(dacOutput, true))
   {
      Serial.print("FAIL");
      Serial.println(" I2C Write Failed");
      failCount++;
      return;
   }

   delay(100);
   Serial.print("Reading ADC Input: ");
   adcInputValue = sampleAdc();
   Serial.println(adcInputValue, DEC);
   if (compareWithError(dacOutput, adcInputValue, adcError))
   {
      Serial.print("FAIL");
      Serial.println(" Input/Output mismatch");
      failCount++;
      return;
   }

   Serial.println("Powering up DAC");
   if(!dac.powerUp())
   {
      Serial.print("FAIL");
      Serial.println(" I2C Write Failed");
      failCount++;
      return;
   }

   delay(100);
   Serial.print("Reading ADC Input: ");
   adcInputValue = sampleAdc();
   Serial.println(adcInputValue, DEC);
   if (!compareWithError(dacOutput, adcInputValue, adcError))
   {
      Serial.print("FAIL");
      Serial.println(" Input/Output mismatch");
      failCount++;
      return;
   }

   Serial.println("PASS");
   passCount++;
}

// We are done with tests
//
// Report the number of pass/fails
void testStateDone()
{
   Serial.println("");
   Serial.print(passCount, DEC);
   Serial.print(" of ");
   Serial.print(passCount + failCount);
   Serial.println(" tests passed.");

   Serial.println("\nReady to test again?");
}
