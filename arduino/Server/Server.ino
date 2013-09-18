/**
 * Server - Wireless communication to drive LED on remote device
 * nRF24L01+ pin assignments
 * GND     GND
 * VCC     3v3
 * MISO    12
 * MOSI    11
 * SCK     13
 * CE      8
 * CSN     7
 */
 
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

const int BAUD = 9600;

String inputString = "";
boolean stringComplete = false;

void setup() {
    Serial.begin(BAUD);
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"SERVER"); // Receiving Address
    Mirf.payload = sizeof(unsigned long);
    Mirf.config();
    Serial.println("SERVER READY");
}

void loop() {
    if (stringComplete) {
        Serial.print("SEND: ");
        Serial.println(inputString);
        
        char charBuff[inputString.length()];
        inputString.toCharArray(charBuff, inputString.length());

        Mirf.setTADDR((byte *)"CLIENT");
        Mirf.send((byte *)charBuff);
        while (Mirf.isSending()) {}
        
        inputString = "";
        stringComplete = false;
    }
}

void serialEvent() {
    while (Serial.available()) {
        char inChar = Serial.read();
        inputString += inChar;
        if (inChar == '\n') {
            stringComplete = true;
        }
    }
}
