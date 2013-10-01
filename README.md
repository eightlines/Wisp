The WISP Project is an LED Rig mounted to a QuadRotor for the purposes of light writing. 
WISP is the first of the http://gameofdrones.ca/ projects.

Installation
============

WISP is divided into several components:
- src/ is the openFrameworks 0.8 application is a PerC utility to control LED functions through hand gestures.
- Arduino/Server.ino is an Arduino application which broadcasts the oF commands through an RF module. (Deprecated for BLE)
- Arduino/Client.ino receives BLE commands and manages the LEDs.
- Fritzing/WispRF.fzz details the pin layouts of the RF module. (Deprecated for BLE)
- OSX/ contains the BLE GUI for the LED rig.

Requirements
------------

- openFrameworks 0.8
- Mac OS X 10.8 (XCode 5)
- Arduino 1.0+
- Arduino SPI (http://arduino.cc/en/Reference/SPI)
- Adafruit LPD8806 (https://github.com/adafruit/LPD8806))
- RedBearLabs BLE Shield (https://github.com/RedBearLab/Release)

Monitoring
----------
To monitor multiple serial connections on a Mac, you can screen the tty port.
- ls /dev/tty.*
- screen /dev/tty.usbmodemfd1141 9600
- To quit, Ctrl-A, Ctrl-\

OS X
----
The Mac OS X BLE controller requires the RedBearLabs BLE Framework.
