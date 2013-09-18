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
 * MODE    6
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <LPD8806.h>

const int BAUD = 9600;

const int nLEDs = 32; // Number of LEDs in strand
const int dataPin  = 2; // Output pins to the LPD8806
const int clockPin = 3; // Output pins to the LPD8806
const int buttonPin = 6;// the number of the input pin

const int frameDuration = 25; // ms to display each frame of animation
const int counterLimit = 10240; // we don't need to track the number of loop executions up to infinity, once it hits this number it starts back at zero.
int counter = 0; // keeps track of the number of times we've executed loop
int maxParticles = 5;
int maxLeadingCorona = (nLEDs/2)-1;
int maxTrailingCorona = (nLEDs/2)-1;

LPD8806 strip = LPD8806(32, dataPin, clockPin); // Init the strip

int particleParamSetCount = 5;		// number of sets of particle parameters
int currentParticleParamSet = 1;	// which of those particle parameter sets are currently active
int ringParamSetCount = 5;			// number of sets of ring parameters
int currentRingParamSet = 1;		// which of those ring parameter sets are currently active

int PARTICLE = 1;
int RING = 2;

int mode = RING;
// int mode = PARTICLE;

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

int particleParams1[] = {
    1, 0, 0, 2, 66, 66, 66, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    1, 0, 1, 2, 66, 66, 66, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 0, 4, 127, 0, 0, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 4, 0, 0, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};
						
int particleParams2[] = {
    1, 0, 0, 4, 43, 43, 43, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    1, 0, 1, 4, 0, 0, 127, 127, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 66, 66, 66, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 66, 66, 66, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};

int particleParams3[] = {
    1, 0, 0, 2, 0, 0, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 66, 66, 66, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 0, 4, 127, 0, 0, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 4, 0, 0, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};

int particleParams4[] = {
    1, 0, 0, 2, 43, 43, 43, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    1, 11, 0, 2, 0, 66, 66, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    1, 22, 0, 2, 0, 0, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 4, 0, 0, 127, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};

int particleParams5[] = {
    1, 0, 0, 2, 66, 66, 66, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    1, 16, 0, 2, 66, 66, 66, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 0, 4, 127, 0, 0, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 4, 0, 0, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};

int activeParticleParams[80];

/* arrays store xx parameters defining a ring pattern. Params are:
 * style - 0: pulse
 * rate
 * red val (127 max)
 * green val
 * blue val
 * brightness
 * 6 empty slots, for future expansion 
*/
int ringParams1[] = {1, 32, 43, 43, 43, 127, 0, 0, 0, 0, 0, 0};
int ringParams2[] = {1, 32, 0, 66, 66, 127, 0, 0, 0, 0, 0, 0};
int ringParams3[] = {0, 64, 0, 66, 66, 127, 0, 0, 0, 0, 0, 0};
int ringParams4[] = {1, 32, 0, 0, 127, 127, 0, 0, 0, 0, 0, 0};
int ringParams5[] = {1, 64, 0, 0, 127, 127, 0, 0, 0, 0, 0, 0};

int activeRingParams[12];

int buttonReading; // the current reading from the input pin
int previousReading = LOW; // the previous reading from the input pin

// the follow variables are longs because the time, measured in miliseconds, will quickly become a bigger number than can be stored in an int.
long time = 0; // the last time the output pin was toggled
long debounce = 100; // the debounce time, increase if the output flickers

String inputString = "";
boolean stringComplete = false;

void setup() {
    Serial.begin(BAUD);
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();
    Mirf.setRADDR((byte *)"CLIENT"); // Receiving Address
    Mirf.payload = sizeof(unsigned long);
    Mirf.config();
    Serial.println("CLIENT READY");
    
    pinMode(buttonPin,INPUT_PULLUP);
    
    strip.begin();
    strip.show();
    initPattern();
    // fill the active params arrays with their default values
    memcpy(activeParticleParams, particleParams1, 128);
    memcpy(activeRingParams, ringParams1, 128);
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
    
    int switchstate;
    buttonReading = digitalRead(buttonPin);
    // If the switch changed, due to bounce or pressing...
    if (buttonReading != previousReading) time = millis(); // reset the debouncing timer
    
    if ((millis() - time) > debounce) {
        switchstate = buttonReading; // whatever the switch is at, its been there for a while so lets act on it!
        if (switchstate == LOW) nextParamSet(); // Now invert the output on the pin13 LED
    }
	
    previousReading = buttonReading; // Save the last reading so we keep a running tally
    
    (mode == PARTICLE) ? renderParticles() : renderRings();

    strip.show();
    clearPixels();
    delay(frameDuration);
}

void nextParamSet() {
    counter = 0;
    if (mode == PARTICLE) {
        if (currentParticleParamSet+1 <= particleParamSetCount) {
            currentParticleParamSet++;
        } else {
            currentParticleParamSet = 1;
            mode = RING;
            return;
        }
    
        switch (currentParticleParamSet) {
            case 1: memcpy(activeParticleParams, particleParams1, 128); break;
            case 2: memcpy(activeParticleParams, particleParams2, 128); break;
            case 3: memcpy(activeParticleParams, particleParams3, 128); break;
            case 4: memcpy(activeParticleParams, particleParams4, 128); break;
            case 5: memcpy(activeParticleParams, particleParams5, 128); break;
        }
    } else if ( mode == RING ) {
        if ( currentRingParamSet+1 <= ringParamSetCount ) {
            currentRingParamSet++;
        } else {
            currentRingParamSet = 1;
            mode = PARTICLE;
            return;
        }
        
        switch (currentRingParamSet) {
            case 1: memcpy(activeRingParams, ringParams1, 128); break;
            case 2: memcpy(activeRingParams, ringParams2, 128); break;
            case 3: memcpy(activeRingParams, ringParams3, 128); break;
            case 4: memcpy(activeRingParams, ringParams4, 128); break;
            case 5: memcpy(activeRingParams, ringParams5, 128); break;
        }
    }	
}

void initPattern() {
    counter = 0;
    for (int i = 0; i < maxParticles; i++) {
        int pActive = activeParticleParams[i * 16];
        if (pActive == 1) {
            // limit the corona sizes
            int pCoronaLeading = activeParticleParams[i * 16 + 8];
            pCoronaLeading = min(pCoronaLeading, maxLeadingCorona);
            activeParticleParams[i * 16 + 8] = pCoronaLeading;

            int pCoronaTrailing = activeParticleParams[i * 16 + 9];
            pCoronaTrailing = min(pCoronaTrailing, maxTrailingCorona);
            activeParticleParams[i * 16 + 9] = pCoronaTrailing;
        }
    }
}

int calculateBrightness(int colorVal, int brightnessPercentage) {
    float colorValue = (float)colorVal;
    colorValue = colorValue / 100 * brightnessPercentage;
    return (int)colorValue;
}

void renderParticles() {
    for (int i = 0; i < maxParticles; i++) {
        int pActive = activeParticleParams[i*16];		
        if (pActive == 1) {
		int pActive = activeParticleParams[i * 16];
		if ( pActive == 1 ) {
                    int pPos = activeParticleParams[i * 16 + 1];		
                    int pDirection = activeParticleParams[i * 16 + 2];
                    int pSpeed = activeParticleParams[i * 16 + 3];	
                    int pRVal = activeParticleParams[i * 16 + 4];
                    int pGVal = activeParticleParams[i * 16 + 5];
                    int pBVal = activeParticleParams[i * 16 + 6];
                    int pBrightness = activeParticleParams[i * 16 + 7];
                    int pCoronaLeading = activeParticleParams[i * 16 + 8];
                    int pCoronaTrailing = activeParticleParams[i * 16 + 9];

                    // if based on the particle's speed it should move on this frame
                    if (counter % pSpeed == 0) { // if this particle rotates clockwise...
                        if (pDirection == 1) { // if this particle has reached the end of the strip...
                            if ((pPos + 1) == nLEDs) {
                                pPos = 0;
                            } else {
                                pPos = pPos + 1;
                            }
                        } else { // else, if this particle rotates counter clockwise
                            if ((pPos - 1) < 0) {
                                pPos = nLEDs - 1;
                            } else {
                                pPos--;
                            }
                        }

                        activeParticleParams[i*16+1] = pPos;
                    }

                    // note the Color | Color argument blends the existing color (if any) of the pixel with the newly calculated color - so particle overlaps look right
                    strip.setPixelColor(pPos, strip.Color(pRVal, pGVal, pBVal) | strip.getPixelColor(pPos));
			
                    /* PLACEHOLDER CODE - replace with proper corona code */
                    if ( pCoronaLeading > 0 ) {
                        // clockwise rotation
                        if (pDirection == 1) {
                            if (pPos-1 < 0) {
                                pPos = nLEDs - 1;
                            } else {
                                pPos = pPos - 1;
                            }

                            strip.setPixelColor(pPos, strip.Color(calculateBrightness(pRVal, 10), calculateBrightness(pGVal, 10), calculateBrightness(pBVal, 10)) | strip.getPixelColor(pPos));				

                            if (pPos - 1 < 0) {
                                pPos = nLEDs - 1;
                            } else {
                                pPos = pPos - 1;
                            }
                    
                            strip.setPixelColor(pPos, strip.Color(calculateBrightness(pRVal, 3), calculateBrightness(pGVal, 3), calculateBrightness(pBVal, 3)) | strip.getPixelColor(pPos));				
                        } else {
                            if (pPos + 1 == nLEDs) {
                                pPos = 0;
                            } else {
                                pPos = pPos + 1;
                            }

                            strip.setPixelColor(pPos, strip.Color(calculateBrightness(pRVal, 10), calculateBrightness(pGVal, 10), calculateBrightness(pBVal, 10)) | strip.getPixelColor(pPos));				

                        if (pPos + 1 == nLEDs) {
                            pPos = 0;
                        } else {
                            pPos = pPos+1;
                        }
                
                        strip.setPixelColor(pPos, strip.Color(calculateBrightness(pRVal, 3), calculateBrightness(pGVal, 3), calculateBrightness(pBVal, 3)) | strip.getPixelColor(pPos));
                    }
                }
            }
        }
    }
}

void renderRings() {
    int rStyle = activeRingParams[0];		
    int rRate = activeRingParams[1];	
    int rRVal = activeRingParams[2];
    int rGVal = activeRingParams[3];
    int rBVal = activeRingParams[4];
    // TODO do something with the brightness
    // int rBrightness = activeRingParams[5];

    // this is a sharp pulse style ring
    if (rStyle == 0) {
        if (counter % rRate == 0) {
            for ( int i=0; i<nLEDs; i++ ) {
                strip.setPixelColor(i, strip.Color(rRVal, rGVal, rBVal));
            }
        }
    }
	
    // temp variables used for the ring fade
    int rRFadeVal = 0;
    int rGFadeVal = 0;
    int rBFadeVal = 0;
    // this is a style of ring with a fade into and out of the brightest pulse
    if (rStyle == 1) {
        if (counter % rRate == 0) {
            // colour all the LEDs in the strip
            for (int i = 0; i < nLEDs; i++) {
                strip.setPixelColor(i, strip.Color(rRVal, rGVal, rBVal));
            }
        } else if ( (counter+1) % rRate == 0 || (counter-1) % rRate == 0 ) {
            if (rRVal > 0) {
                rRFadeVal = calculateBrightness(rRVal, 10);
            }
            
            if (rGVal > 0) {
                rGFadeVal = calculateBrightness(rGVal, 10);
            }

            if (rBVal > 0) {
                rBFadeVal = calculateBrightness(rBVal, 10);
            }

            for (int i = 0; i < nLEDs; i++) {
                strip.setPixelColor(i, strip.Color(rRFadeVal, rGFadeVal, rBFadeVal));
            }
        } else if ((counter+2) % rRate == 0 || (counter-2) % rRate == 0) {
            if (rRVal > 0) {
                rRFadeVal = calculateBrightness(rRVal, 5);
            }

            if (rGVal > 0) {
                rGFadeVal = calculateBrightness(rGVal, 5);
            }

            if (rBVal > 0) {
                rBFadeVal = calculateBrightness(rBVal, 5);
            }
            
            for ( int i=0; i<nLEDs; i++ ) {
                strip.setPixelColor(i, strip.Color(rRFadeVal, rGFadeVal, rBFadeVal));
            }
        } else if ( (counter+3) % rRate == 0 || (counter-3) % rRate == 0 ) {
            if (rRVal > 0) {
                rRFadeVal = calculateBrightness(rRVal, 3);
            }
            
            if (rGVal > 0) {
                rGFadeVal = calculateBrightness(rGVal, 3);
            }

            if (rBVal > 0) {
                rBFadeVal = calculateBrightness(rBVal, 3);
            }
		
            for (int i = 0; i < nLEDs; i++) {
                strip.setPixelColor(i, strip.Color(rRFadeVal, rGFadeVal, rBFadeVal));
            }
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

