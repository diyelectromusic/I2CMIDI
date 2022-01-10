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


### Provided Example 1

The included examples demonstrate a CONTROLLER transmitter (MIDI OUT) and a PERIPHERAL receiver (MIDI IN).  This requires two Arduinos with an I2C link between them.  This requires the connections: A4 to A4; A5 to A5; and GND to GND.  A simple loudspeaker can be connected to pin 8 of the PERIPHERAL receiver to hear the tones being played by the CONTROLLER.  Either way the results will be printed out to the serial monitor.

To experiment with the PERIPHERAL as the transmitter (MIDI OUT) and the CONTROLLER as the receiver (MIDI IN), just change the parameters passed in to I2CMIDI_CREATE_INSTANCE.

For more details, refer to the following blog post: [Arduino I2C MIDI Interface - Part 2](https://diyelectromusic.wordpress.com/2022/01/08/arduino-i2c-midi-interface-part-2/)

### Provided Example 2

A second example provides code to show how an Arduino can act as a one-way Serial MIDI to I2C MIDI relay.  The direction is configurable.

For details on this one, refer to the blog post: [Arduino I2C MIDI Interface - Part 3](https://diyelectromusic.wordpress.com/2022/01/09/arduino-i2c-midi-interface-part-3/)

### Provided Example 3

This requires three Arduinos and shows how a single I2C MIDI controller can talk to two I2C MIDI peripherals.  The provided code runs on an Arduino acting as a serial to I2C MIDI relay. One Arduino should be running the I2CMIDIReceive example and have a loudspeaker connected.  The remaining Arduino should be running the I2CSerialMIDIRelay example, but configured for I2C2SERIAL mode using I2C address 0x50.

Further details for this example can be found in the blog post: [Arduino I2C MIDI Interface - Part 4](https://diyelectromusic.wordpress.com/2022/01/10/arduino-i2c-midi-interface-part-4/)

Kevin
@diyelectromusic
