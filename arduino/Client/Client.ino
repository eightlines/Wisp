/**
 * Client - Wireless communication to drive LED on remote device
 *
 * nRF24L01+ pin assignments
 * GND     GND
 * VCC     3v3
 * MISO    12
 * MOSI    11
 * SCK     13
 * CE      8
 * CSN     7
 *
 * LPD8806 pin assignments
 * DATA    2
 * CLOCK   3
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <LPD8806.h>

const int BAUD = 9600;

const int nLEDs = 32; // Number of LEDs in strand
const int dataPin  = 2;
const int clockPin = 3;
const int frameDuration = 25; // ms to display each frame of animation
int counter = 0; // keeps track of the number of times we've executed loop
const int counterLimit = 10240; // we don't need to track the number of loop executions up to infinity, once it hits this number it starts back at zero.
int maxParticles = 5;
int maxLeadingCorona = (nLEDs/2)-1;
int maxTrailingCorona = (nLEDs/2)-1;
LPD8806 strip = LPD8806(32, dataPin, clockPin);

/* 
 * array stores 16 parameters per particle. In order these represent:
 * particle active (0 or 1)
 * particle starting position (0 to 31, assuming a 32 LED strip)
 * rotation direction (1 = clockwise, 0 = ccw)
 * particle speed (Lower is faster. 1 means the particle moves on every frame update. 4 would be a movement every fourth frame.)
 * red val (127 max)
 * green val
 * blue val
 * brightness 
 * leading corona length
 * trailing corona length 
 * 6 empty slots, for future expansion
*/

int particleParams[] = {
    1, 0, 0, 2, 66, 66, 66, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    1, 0, 1, 2, 66, 66, 66, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    1, 0, 0, 4, 127, 0, 0, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    1, 0, 1, 4, 0, 0, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};

String inputString = "";
String mode = "";

boolean stringComplete = false;

void setup() {
    Serial.begin(BAUD);
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"CLIENT"); // Receiving Address
    Mirf.payload = sizeof(unsigned long);
    Mirf.config();
    Serial.println("CLIENT READY");
    
    strip.begin();
    strip.show();
    initPattern();
}

void loop() {
    byte data[Mirf.payload];

    if (Mirf.dataReady()) {
        do {
            Serial.print("RECD INSTRUCTION ");
            Mirf.getData(data);
            for (int i = 0; i < sizeof(data); i++) {
                char inChar = data[i];
                inputString += inChar;
            }
        } while (!Mirf.rxFifoEmpty());

        inputString = "";
    }
    
    counter++;
    if (counter > counterLimit) counter = 0;
    renderParticles();
    strip.show();
    clearPixels();
    delay(frameDuration);
}

void initPattern() {
    counter = 0;
    for (int i=0; i<maxParticles; i++) {
        int pActive = particleParams[i*16];		
        if (pActive == 1) {
            // limit the corona sizes
            int pCoronaLeading = particleParams[i*16+8];
            pCoronaLeading = min(pCoronaLeading, maxLeadingCorona);
            particleParams[i*16+8] = pCoronaLeading;

            int pCoronaTrailing = particleParams[i*16+9];
            pCoronaTrailing = min(pCoronaTrailing, maxTrailingCorona);
            particleParams[i*16+9] = pCoronaTrailing;
        }
    }
}

int calculateBrightness(int colorVal, int brightnessPercentage) {
    float colorValue = (float)colorVal;
    colorValue = colorValue / 100 * brightnessPercentage;
    return (int)colorValue;
}

void renderParticles() {
    for (int i=0; i<maxParticles; i++) {
        int pActive = particleParams[i*16];		
        if (pActive == 1) {
            int pPos = particleParams[i*16+1];		
            int pDirection = particleParams[i*16+2];
            int pSpeed = particleParams[i*16+3];	
            int pRVal = particleParams[i*16+4];
            int pGVal = particleParams[i*16+5];
            int pBVal = particleParams[i*16+6];
            int pBrightness = particleParams[i*16+7];
            int pCoronaLeading = particleParams[i*16+8];
            int pCoronaTrailing = particleParams[i*16+9];

            // if based on the particle's speed it should move on this frame
            if (counter % pSpeed == 0) { // if this particle rotates clockwise...
                if (pDirection == 1) { // if this particle has reached the end of the strip...
                    if ((pPos + 1) == (nLEDs)) {
                        pPos = 0;
                    } else {
                        pPos = pPos+1;
                    }
                } else { // else, if this particle rotates counter clockwise
                    if (pPos-1 < 0) {
                        pPos = nLEDs-1;
                    } else {
                        pPos--;
                    }
                }
                particleParams[i*16+1] = pPos;
            }
            // note the Color | Color argument blends the existing color (if any) of the pixel with the newly calculated color - so particle overlaps look right
            strip.setPixelColor(pPos, strip.Color(pRVal, pGVal, pBVal) | strip.getPixelColor(pPos));
			
            /* PLACEHOLDER CODE - replace with proper corona code */
            if (pPos-1 < 0) {
                pPos = nLEDs-1;
            } else {
                pPos = pPos-1;
            }
            strip.setPixelColor(pPos, strip.Color(calculateBrightness(pRVal, 15), calculateBrightness(pGVal, 15), calculateBrightness(pBVal, 15)) | strip.getPixelColor(pPos));

            if (pPos-1 < 0) {
                pPos = nLEDs-1;
            } else {
                pPos = pPos-1;
            }
            strip.setPixelColor(pPos, strip.Color(calculateBrightness(pRVal, 5), calculateBrightness(pGVal, 5), calculateBrightness(pBVal, 5)) | strip.getPixelColor(pPos));
        }
    }
}

// turn all pixels off
void clearPixels() {
    for (int i = 0; i < nLEDs; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
}

int stringToInt(String string) {
    char charString[string.length() + 1];
    string.toCharArray(charString, string.length() + 1);
    return atoi(charString);
}

