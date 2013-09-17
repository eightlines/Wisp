The LightWriter experiment is part of the http://gameofdrones.ca/ project.

Installation
============

LightWriter is divided into several components. 
- The openFrameworks 0.8 application is a PerC utility to control RF communications.
- LightWriterServer is the Arduino application which broadcasts the oF commands.
- LightWriterClient receives RF commands and manages the LEDs.

Requirements
------------

- openFrameworks 0.8
- Arduino 1.0+
- Arduino SPI (http://arduino.cc/en/Reference/SPI)
- Arduino MIRF (http://playground.arduino.cc/InterfacingWithHardware/Nrf24L01)
- Adafruit LPD8806 (https://github.com/adafruit/LPD8806))
