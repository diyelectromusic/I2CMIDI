/*
// Simple DIY Electronic Music Projects
//    diyelectromusic.wordpress.com
//
//  I2CMIDI Receive
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
I2CMIDI_CREATE_INSTANCE(I2CMIDIPERIPHERAL, I2CMIDIADDR, I2CMIDI);   // PERIPHERAL IN
//I2CMIDI_CREATE_INSTANCE(I2CMIDICONTROLLER, I2CMIDIADDR, I2CMIDI);   // CONTROLLER IN

#define MIDI_CHANNEL 1
#define MIDI_LED LED_BUILTIN

// Attach a speaker to pin 8.
// See: https://www.arduino.cc/en/Tutorial/BuiltInExamples/toneMelody
#define TONE_PIN 8

#define MIDI_NOTE_START 60
#define MIDI_NOTE_END   72
#define NUM_NOTES       (MIDI_NOTE_END-MIDI_NOTE_START+1)
int notes[NUM_NOTES] = {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523};
int playing;

void midiNoteOn(byte channel, byte pitch, byte velocity) {
    Serial.print("NoteOn: ");
    Serial.print(channel);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.println(velocity);
    if (velocity == 0) {
        midiNoteOff(channel, pitch, velocity);
        return;
    }

    if ((pitch < MIDI_NOTE_START) || (pitch > MIDI_NOTE_END)) {
        // Note out of range
        return;
    }
    playing = pitch;
    tone (TONE_PIN, notes[pitch-MIDI_NOTE_START]);
    digitalWrite(MIDI_LED, HIGH);
}

void midiNoteOff(byte channel, byte pitch, byte velocity) {
    Serial.print("NoteOff: ");
    Serial.print(channel);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.println(velocity);
    if (pitch == playing) {
        noTone(TONE_PIN);
        digitalWrite(MIDI_LED, LOW);
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(MIDI_LED, OUTPUT);
    I2CMIDI.setHandleNoteOn(midiNoteOn);
    I2CMIDI.setHandleNoteOff(midiNoteOff);
    I2CMIDI.begin(MIDI_CHANNEL);
    Serial.println("Listening for MIDI data...");
}

void loop() {
    I2CMIDI.read();
}
