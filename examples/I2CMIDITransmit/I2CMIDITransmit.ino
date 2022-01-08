/*
// Simple DIY Electronic Music Projects
//    diyelectromusic.wordpress.com
//
//  I2CMIDI Transmit
//  https://diyelectromusic.wordpress.com/
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

// Note that whilst either side can act as a CONTROLLER or PERIPHERAL (as long
// as there is just one CONTROLLER), MIDI works much better when the CONTROLLER
// is doing the majority of the sending.
//
// So having CONTROLLER = OUT = Transmit; PERIPHERAL = IN = Receive
// gives the best performance if that is manageable.
//
#define I2CMIDIADDR 0x40
I2CMIDI_CREATE_INSTANCE(I2CMIDICONTROLLER, I2CMIDIADDR, MIDI);  // CONTROLLER OUT
//I2CMIDI_CREATE_INSTANCE(I2CMIDIPERIPHERAL, I2CMIDIADDR, MIDI);  // PERIPHERAL OUT

#define MIDI_CHANNEL 1
#define MIDI_LED LED_BUILTIN

void setup() {
    Serial.begin(9600);
    pinMode(MIDI_LED, OUTPUT);
    MIDI.begin(MIDI_CHANNEL_OFF);
}

void loop() {
    Serial.print("Sending MIDI data on channel ");
    Serial.println(MIDI_CHANNEL);
    for (int i=0; i<13; i++) {
        digitalWrite(MIDI_LED, HIGH);
        MIDI.sendNoteOn(60+i, 127, MIDI_CHANNEL);
        delay(100);
        digitalWrite(MIDI_LED, LOW);
        MIDI.sendNoteOff(60+i, 0, MIDI_CHANNEL);
        delay(100);
    }
    delay(500);
    for (int i=12; i>=0; i--) {
        digitalWrite(MIDI_LED, HIGH);
        MIDI.sendNoteOn(60+i, 127, MIDI_CHANNEL);
        delay(30);
        digitalWrite(MIDI_LED, LOW);
        MIDI.sendNoteOff(60+i, 0, MIDI_CHANNEL);
    }
    delay(500);
}
