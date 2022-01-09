/*
// Simple DIY Electronic Music Projects
//    diyelectromusic.wordpress.com
//
//  I2CMIDI to Serial MIDI Relay
//  https://diyelectromusic.wordpress.com/2022/01/09/arduino-i2c-midi-interface-part-3/
//
      MIT License
      
      Copyright (c) 2022 diyelectromusic (Kevin)
      
      Permission is hereby granted, free of charge, to any person obtaining a copy of
      this software and associated documentation files (the "Software"), to deal in
      the Software without restriction, including without limitation the rights to
      use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
      the Software, and to permit persons to whom the Software is furnished to do so,
      subject to the following conditions:
      
      The above copyright notice and this permission notice shall be included in all
      copies or substantial portions of the Software.
      
      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
      FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
      COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHERIN
      AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
      WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*
  Using principles from the following:
     Arduino MIDI Library: https://github.com/FortySevenEffects/arduino_midi_library
     Master Reader/Slave Sender: https://www.arduino.cc/en/Tutorial/LibraryExamples/MasterReader
*/
#include <MIDI.h>
#include <I2CMIDI.h>

// Choose the direction for this relay...
#define SERIAL2I2C 1  // Serial MIDI IN to I2C MIDI OUT
//#define I2C2SERIAL 2  // I2C MIDI IN to Serial MIDI OUT

#define I2CMIDIADDR 0x40
#ifdef SERIAL2I2C
// This is the I2C CONTROLLER with MIDI OUT
I2CMIDI_CREATE_INSTANCE(I2CMIDICONTROLLER, I2CMIDIADDR, I2CMIDI);  // CONTROLLER OUT
#else
// This is the I2C PERIPHERAL with MIDI IN
I2CMIDI_CREATE_INSTANCE(I2CMIDIPERIPHERAL, I2CMIDIADDR, I2CMIDI);  // PERIPHERAL OUT
#endif

// Initialise the serial MIDI
MIDI_CREATE_DEFAULT_INSTANCE();

#define MIDI_LED LED_BUILTIN

void setup() {
  pinMode(MIDI_LED, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
  I2CMIDI.begin(MIDI_CHANNEL_OMNI);
  I2CMIDI.turnThruOff();
}

int ledCount;
void ledOn() {
  digitalWrite(MIDI_LED, HIGH);
  ledCount = 1000;
}

void ledOff() {
  if (ledCount > 0) {
      ledCount--;
  } else if (ledCount == 0) {
    digitalWrite(MIDI_LED, LOW);
    ledCount = -1;
  }
}

void loop() {
#ifdef SERIAL2I2C
  if (MIDI.read())
  {
    ledOn();
    I2CMIDI.send(MIDI.getType(),
                 MIDI.getData1(),
                 MIDI.getData2(),
                 MIDI.getChannel());
  }
#else
  if (I2CMIDI.read())
  {
    ledOn();
    MIDI.send(I2CMIDI.getType(),
              I2CMIDI.getData1(),
              I2CMIDI.getData2(),
              I2CMIDI.getChannel());
  }
#endif
  ledOff();
}
