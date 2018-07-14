# Door-sensor-with-433MHz-and-ATTINY13
Door and Window sensor sends open and close code to home automation system

## Function
This is a little device with a reed switch one one side of the door/window and a magnet on the other side. When the door/window
opens a code will be sent through the 433MHz transciever (I use a Wireless Transmitter Module H34B). When the door/window closes another code will be sent. There is also an optional tampering switch possible.
The sensors work together with https://github.com/mrohner/433MHz-MQTT-Bridge-with-Wemos-D1 in order to relay the door/window status to
MQTT and eventually to Domoticz.

## Design
I used an ATTINY13A and attached a reed switch as well as a 433MHz transciever. All runs off a 3V CR2032 battery. Of course the ATTINY
is asleep most of the time. I attach an Eagle Cad file FYI. The protocol used is understandable by most receivers, see also
https://github.com/sui77/rc-switch and https://github.com/sweetpi/attiny-433mhz-sender
```
/* Format for protocol definitions:
 * {pulselength, Sync bit, "0" bit, "1" bit}
 * 
 * pulselength: pulse length in microseconds, e.g. 350
 * Sync bit: {1, 31} means 1 high pulse and 31 low pulses
 *     (perceived as a 31*pulselength long pulse, total length of sync bit is
 *     32*pulselength microseconds), i.e:
 *      _
 *     | |_______________________________ (don't count the vertical bars)
 * "0" bit: waveform for a data bit of value "0", {1, 3} means 1 high pulse
 *     and 3 low pulses, total length (1+3)*pulselength, i.e:
 *      _
 *     | |___
 * "1" bit: waveform for a data bit of value "1", e.g. {3,1}:
 *      ___
 *     |   |_
 *
 * These are combined to form Tri-State bits when sending or receiving codes.
 */
```
## Library used
You will need this library: https://github.com/NicoHood/PinChangeInterrupt
