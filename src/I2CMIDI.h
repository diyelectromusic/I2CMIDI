/*!
 *  @file       I2CMIDI.h
 *  @license    MIT - Copyright (c) 2022 Kevin (@diyelectromusic)
 *  @author     Kevin, Francois Best
 *  @date       01/03/22
 *
 *  Based on:
 *  @file       serialMIDI.h
 *  Project     Arduino MIDI Library
 *  @brief      MIDI Library for the Arduino - Platform
 *  @license    MIT - Copyright (c) 2015 Francois Best
 *  @author     lathoub, Francois Best
 *  @date       22/03/20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#include <MIDI.h>
#include <Wire.h>
using namespace MIDI_NAMESPACE;

#include "I2CMIDI_Namespace.h"

BEGIN_I2CMIDI_NAMESPACE

#define I2CMIDICONTROLLER 1
#define I2CMIDIPERIPHERAL 0

// Use the "fast mode" bus speed
#define I2CCLOCK 400000

// Static variables and function to act in the callback for
// receiving I2C data from the Wire library.
//
// Note: If there are several instances of I2CMIDI then
//       things will start to get weird...
//
#define I2C_BUFFER_SIZE 32

// Circular buffer for a peripheral to write data to
// until it is requested by the controller.
static int mWAvailable=0;
static int mWWIdx=0;
static int mWRIdx=0;
static int mWBuf[I2C_BUFFER_SIZE];

// Circular buffer for a peripheral to receive data
// when sent from a controller until ready to be read.
static int mRAvailable=0;
static int mRWIdx=0;
static int mRRIdx=0;
static int mRBuf[I2C_BUFFER_SIZE];

// Called by peripheral when data received by the controller.
static void receiveEvent(int howMany) {
    while (Wire.available()) {
        mRAvailable++;
        mRBuf[mRWIdx++] = Wire.read();
        mRWIdx %= I2C_BUFFER_SIZE;
    }
}

// Called by peripheral when a datq request is received by the controller.
static void requestEvent() {
    // Write out data from the write buffer
    if (mWAvailable > 0) {
        int val = mWBuf[mWRIdx++];
        mWRIdx %= I2C_BUFFER_SIZE;
        mWAvailable--;
        Wire.write(val);
    }
}

class I2CMIDI
{

public:
	I2CMIDI(bool mode, int addr)
	{
        mI2CAddr = addr;
        mMode =  mode;
	};

public:
    static const bool thruActivated = false;
    
    void begin()
	{
        if (mMode == I2CMIDICONTROLLER) {
            Wire.begin();
            Wire.setClock(I2CCLOCK);
        } else {
            Wire.begin(mI2CAddr);
            Wire.setClock(I2CCLOCK);
            // Register our callbacks for when the controller sends or requests data
            Wire.onReceive(receiveEvent);
            Wire.onRequest(requestEvent);
        }
    }

	bool beginTransmission(MidiType)
	{
        if (mMode == I2CMIDICONTROLLER) {
            Wire.beginTransmission(mI2CAddr);
        }
		return true;
	};

	void write(byte value)
	{
        if (mMode == I2CMIDICONTROLLER) {
            Wire.write(value);
        } else {
            // Cannot write from a peripheral directly, so need
            // to write out to our buffer and let the controller request the data.
            // If the controller doesn't request data quick enough, then
            // we will lose data somewhere... but we can't do anything about that!
            mWAvailable++;
            mWBuf[mWWIdx++] = value;
            mWWIdx %= I2C_BUFFER_SIZE;
        }
	};

	void endTransmission()
	{
        if (mMode == I2CMIDICONTROLLER) {
            Wire.endTransmission();
        }
	};

	byte read()
	{
        if (mMode == I2CMIDICONTROLLER) {
            return mReceivedValue;
            mRAvailable = 0;
        } else {
            // Read data out of the receive buffer
            int val = mRBuf[mRRIdx++];
            mRRIdx %= I2C_BUFFER_SIZE;
            mRAvailable--;
            if (mRAvailable < 0) mRAvailable = 0;
            return val;
        }
	};

	unsigned available()
	{
        if (mMode == I2CMIDICONTROLLER) {
            // If data is available, then lets grab a value and let caller know
            if (Wire.available()) {
                mReceivedValue = Wire.read();
                mRAvailable++;
            } else {
                // If no data available then request more data from the peripheral.
                // Seems to work best when request three bytes at a time.
                // Possibly because MIDI messages are usually either 2 or 3 bytes
                // long (at least the simple ones are).
                Wire.requestFrom(mI2CAddr, 3);
                mRAvailable = 0;
            }
        } else {
            return mRAvailable;
        }
	};

private:
    int mI2CAddr;
    int mMode;
    int mReceivedValue;
};

/*! \brief Create an instance of the library attached to an I2C port.
 Example: MIDI_CREATE_INSTANCE(I2CMIDICONROLLER, I2CADDR, i2cmidi);
 Then call i2cmidi.begin(), i2cmidi.read() etc..
 */
#define I2CMIDI_CREATE_INSTANCE(I2CMaster, I2CAddr, Name)  \
    I2CMIDI_NAMESPACE::I2CMIDI I2C##Name(I2CMaster, I2CAddr);\
    MIDI_NAMESPACE::MidiInterface<I2CMIDI_NAMESPACE::I2CMIDI> Name((I2CMIDI_NAMESPACE::I2CMIDI&)I2C##Name);

END_I2CMIDI_NAMESPACE
