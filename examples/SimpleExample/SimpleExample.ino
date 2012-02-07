#include <Wire.h>
#include <Max517Dac.h>

const int   adcInputPin       = A0;
int         adcInputValue     = 0;
int         dacOutputValue    = 0;

Max517Dac   dac;

void setup()
{
   Serial.begin(9600); 
}

void loop()
{
   adcInputValue = analogRead(adcInputPin);            
   adcInputValue >>= 2;

   Serial.print("DAC Output: ");
   Serial.print(dacOutputValue);
   Serial.print(", ADC Input: " );                       
   Serial.println(adcInputValue);

   dac.setOutput(dacOutputValue++);
  
   delay(100);                     
}
