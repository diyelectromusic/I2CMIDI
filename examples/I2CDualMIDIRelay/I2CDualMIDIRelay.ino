/*
// Simple DIY Electronic Music Projects
//    diyelectromusic.wordpress.com
//
//  Serial to Dual I2CMIDI
//  https://diyelectromusic.wordpress.com/2022/01/10/arduino-i2c-midi-interface-part-4/
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

// Instantiate two instances of the I2C MIDI interface
#define I2CMIDIADDR1 0x40   // Relay to the I2CMIDIReceive Demo
#define I2CMIDIADDR2 0x50   // Relay to an I2CMIDI to Serial Relay Demo (with address changed)

// Both instances are CONTROLLERS, but will be looking to talk to PERIPHERALS with different addresses.
I2CMIDI_CREATE_INSTANCE(I2CMIDICONTROLLER, I2CMIDIADDR1, I2CMIDI1);  // CONTROLLER OUT
I2CMIDI_CREATE_INSTANCE(I2CMIDICONTROLLER, I2CMIDIADDR2, I2CMIDI2);  // CONTROLLER OUT

// Initialise the serial MIDI
MIDI_CREATE_DEFAULT_INSTANCE();

#define MIDI_LED LED_BUILTIN

void setup() {
  pinMode(MIDI_LED, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
  I2CMIDI1.begin(MIDI_CHANNEL_OMNI);
  I2CMIDI1.turnThruOff();
  I2CMIDI2.begin(MIDI_CHANNEL_OMNI);
  I2CMIDI2.turnThruOff();
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
  if (MIDI.read())
  {
    ledOn();
    midi::MidiType cmd = MIDI.getType();
    midi::DataByte d1 = MIDI.getData1();
    midi::DataByte d2 = MIDI.getData2();
    midi::Channel ch = MIDI.getChannel();

    switch (cmd) {
      case midi::NoteOn:
      case midi::NoteOff:
        if (d1 >= 60 && d1 <= 72) {
          // This is the range of notes for the I2CReceive Demo
          I2CMIDI1.send(cmd, d1, d2, ch);
        } else {
          // Semd to the other MIDI port
          I2CMIDI2.send(cmd, d1, d2, ch);
        }
        break;
      default:
        // Ignore anything else
        break;
    }
  }
  ledOff();
}
