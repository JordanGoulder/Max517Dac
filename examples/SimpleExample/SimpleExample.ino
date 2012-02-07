/*
 * SimpleExample.ino
 *
 * This example demonstrates the usage of the MAX517 8-bit DAC library.
 *
 * Copyright (c) 2012 Jordan Goulder. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
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

// Create a new DAC object
Max517Dac   dac;

void setup()
{
   // Initialize the serial interface
   Serial.begin(9600); 

   // Reset the DAC output
   dac.resetOutput();
}

void loop()
{
   // Read the value of the ADC
   adcInputValue = analogRead(adcInputPin);            

   // Convert the analog reading to an 8-bit value
   adcInputValue >>= 2;

   // Print out the values
   Serial.print("DAC Output: ");
   Serial.print(dacOutputValue);
   Serial.print(", ADC Input: " );                       
   Serial.println(adcInputValue);

   // Update the DAC with a new value
   dac.setOutput(++dacOutputValue);
 
   // Wait a little bit for things to settle out
   delay(100);                     
}
