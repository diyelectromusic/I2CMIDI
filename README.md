# Arduino I2C MIDI Transport
I2C/IIC MIDI Transport for use with the Arduino MIDI Library

This library implements a direct IIC/I2C MIDI transport layer for the [FortySevenEffects Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library).

*Warning: This is a very draft implementation.  Use at your own risk!*

## Installation

This library depends on the [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library).

## Usage
### Basic / Default

MIDI I2C Controller Transmitter

```cpp
// MIDI Transmit as I2C Controller
#include <MIDI.h>
#include <I2CMIDI.h>

I2CMIDI_CREATE_INSTANCE(I2CMIDICONTROLLER, 0x40, MIDI);

void setup()
{
   MIDI.begin();
...

void loop()
{
   MIDI.SendNoteOn(60, 127, 1);
...
```
MIDI I2C Peripheral Receiver

```cpp
// MIDI Receive as I2C Peripheral
#include <MIDI.h>
#include <I2CMIDI.h>

I2CMIDI_CREATE_INSTANCE(I2CMIDIPERIPHERAL, 0x40, MIDI);

void setup()
{
   MIDI.begin(1);
...

void loop()
{
   MIDI.read();
   ...
```
These will create a instance named `MIDI` using an I2C exchange to a device at I2C address 0x40, using MIDI channel 1.

There should only be one CONTROLLER on the bus, but in principle there can be several PERIPHERALs.  Either can send or receive MIDI data but note that it is more efficient for a CONTROLLER to do the sending and PERIPHERALs to receive.


### Provided Example

The included examples demonstrate a CONTROLLER transmitter (MIDI OUT) and a PERIPHERAL receiver (MIDI IN).  This requires two Arduinos with an I2C link between them.  This requires the connections: A4 to A4; A5 to A5; and GND to GND.  A simple loudspeaker can be connected to pin 8 of the PERIPHERAL receiver to hear the tones being played by the CONTROLLER.  Either way the results will be printed out to the serial monitor.

To experiment with the PERIPHERAL as the transmitter (MIDI OUT) and the CONTROLLER as the receiver (MIDI IN), just change the parameters passed in to I2CMIDI_CREATE_INSTANCE.

For more details, refer to the following blog post: [Arduino I2C MIDI Interface - Part 2](https://diyelectromusic.wordpress.com/)

Kevin
@diyelectromusic
