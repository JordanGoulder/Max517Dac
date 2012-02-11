/*
 * Max517Dac.h
 *
 * MAX517 8-bit DAC library definition.
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

/**
 * @file Max517Dac.h
 *
 * MAX517 8-bit DAC library definition
 *
 * @author Jordan Goulder
 */

#ifndef MAX517DAC_H
#define MAX517DAC_H

#include <stdint.h>

/**
 * MAX517 8-bit DAC Device Class.
 * This class provides a simple interface to the MAX517 8-bit DAC.
 */
class Max517Dac
{
   public:

      /**
       * Default I2C address of the DAC.
       *
       * @note This assumes that AD0 and AD1 of the DAC are tied to ground.
       */
      static const uint8_t DEFAULT_ADDRESS = 0x2C;

      /**
       * Create a new MAX517 device.
       *
       * @param deviceAddress The I2C address of the device.
       */
      Max517Dac(uint8_t deviceAddress = DEFAULT_ADDRESS);
      
      /**
       * Reset the DAC output back to 0.
       *
       * @param powerDownMode Put the device in power down mode after the
       * commnad is exectued.
       *
       * @return I2C write status.<br><br>
       * true: I2C write successful.<br>
       * false: I2C write failed.<br>
       *
       * @note The output of the device is left floating while in the
       * power down mode.
       */
      bool resetOutput(bool powerDownMode = false) const;

      /**
       * Change the value of the DAC.
       *
       * @param value Value used to set the DAC.
       * @param powerDownMode Put the device in power down mode after the
       * commnad is exectued.
       *
       * @return I2C write status.<br><br>
       * true: I2C write successful.<br>
       * false: I2C write failed.<br>
       *
       * @note The output of the device is left floating while in the
       * power down mode.
       */
      bool setOutput(uint8_t value, bool powerDownMode = false) const;

      /**
       * Power down the device.
       *
       * @return I2C write status.<br><br>
       * true: I2C write successful.<br>
       * false: I2C write failed.<br>
       *
       * @note The output of the device is left floating while in the
       * power down mode.
       */
      bool powerDown() const;

      /**
       * Power up the device.
       *
       * @return I2C write status.<br><br>
       * true: I2C write successful.<br>
       * false: I2C write failed.<br>
       *
       * @note The output is restored to the last output value written
       * to the device.
       */
      bool powerUp() const;

   private:

      /**
       * I2C address of the device
       */
      uint8_t mDeviceAddress;

      /**
       * Performs the actual I2C write to the device
       * 
       * @param command Command used to control the device.
       * @param output  Value to send as the output byte of the message.
       * @param sendOuput Set this to true to indicate that the output should be sent.
       *
       * @return I2C write status.<br><br>
       * true: I2C write successful.<br>
       * false: I2C write failed.<br>
       */
      bool mSendCommand(uint8_t  command,
                        uint8_t  output      = 0,
                        bool     sendOutput  = false) const;
};

#endif
