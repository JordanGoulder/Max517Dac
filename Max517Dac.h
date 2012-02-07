/*
 * Max517Dac.h
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

/**
 * @file Max517Dac.h
 *
 * MAX517 8-bit DAC library definition
 *
 * @author Jordan Goulder
 */

#ifndef MAX517DAC_H
#define MAX517DAC_H

#include <Arduino.h>

/**
 * MAX 517 8-bit DAC Device Class.
 * This class provides a simple interface to the MAX 517 8-bit DAC.
 */
class Max517Dac
{
   public:
      /**
       * Default I2C address of the DAC.
       * @note This assumes that A0 and A1 of the DAC are tied to ground.
       */
      static const int DEFAULT_ADDRESS = 0x2C;

      /**
       * Create a new MAX517 device.
       *
       * @param address The I2C address of the device.
       */
      Max517Dac(int address = DEFAULT_ADDRESS);
      
      /**
       * Reset the DAC output back to 0.
       *
       * @param powerDown Put the device in power down mode after the
       * commnad is exectued.
       *
       * @note The output of the device is left floating while in the
       * power down mode.
       */
      void resetOutput(bool powerDown = false) const;

      /**
       * Change the value of the DAC.
       *
       * @param value Value used to set the DAC.
       * @param powerDown Put the device in power down mode after the
       * commnad is exectued.
       *
       * @note The output of the device is left floating while in the
       * power down mode.
       */
      void setOutput(int value, bool powerDown = false) const;

      /**
       * Power down the device.
       *
       * @note The output of the device is left floating while in the
       * power down mode.
       */
      void powerDown(void) const;

      /**
       * Power up the device.
       *
       * @note The output is restored to the last output value written
       * to the device.
       */
      void powerUp(void) const;

   private:
      int mAddress;
      void mSendCommand(int command, int output) const;
};

#endif
