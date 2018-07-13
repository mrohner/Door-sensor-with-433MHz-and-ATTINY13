# Door-sensor-with-433MHz-and-ATTINY13
Door and Window sensor sends open and close code to home automation system

## Function
This is a little device with a reed switch one one side of the door/window and a magnet one the other side. When the door/window
opens a code will be sent through the 433MHz transciever. When the door/window closes another code will be sent. There is also
an optional tampering switch possible.
The sensors work together with https://github.com/mrohner/433MHz-MQTT-Bridge-with-Wemos-D1 in order to relay the door/window status to
MQTT and eventually to Domoticz.

## Design
I used an ATTINY13A and attached a reed switch as well as a 433MHz transciever. All runs off a 3V CR2032 battery. Of course the ATTINY
is asleep most of the time. I attach an Eagle Cad file FYI.

You will need this library: https://github.com/NicoHood/PinChangeInterrupt
