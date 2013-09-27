#include <LPD8806.h>
#include "LEDParams.h"

#define LPD_CLOCK             5
#define LPD_DATA              4

#define NUM_LEDS              32
#define FRAME_DURATION        25 // ms to display each frame of animation
#define COUNTER_LIMIT         10240 // we don't need to track the number of loop executions up to infinity, once it hits this number it starts back at zero.

int counter =                 0; // keeps track of the number of times we've executed loop
int maxParticles =            5;
int maxLeadingCorona =        (NUM_LEDS / 2) - 1;
int maxTrailingCorona =       (NUM_LEDS / 2) - 1;

int particleParamSetCount =   5;		// number of sets of particle parameters
int currentParticleParamSet = 1;	// which of those particle parameter sets are currently active
int ringParamSetCount =       5;			// number of sets of ring parameters
int currentRingParamSet =     1;		// which of those ring parameter sets are currently active

int PARTICLE =                1;
int RING =                    2;

int mode = RING;
//int mode = PARTICLE;

LPD8806 strip = LPD8806(NUM_LEDS, LPD_DATA, LPD_CLOCK);

void LPD8806_setup() {
    strip.begin();
    clearPixels();
    strip.show();
    
    Serial.println("LPD8806 Initialized");
    
    initPattern();
    // fill the active params arrays with their default values
    copyArray(particleParams1, activeParticleParams);
    copyArray(ringParams1, activeRingParams);
} 

void LPD8806_loop() {
    (mode == PARTICLE) ? renderParticles() : renderRings();

    strip.show();
    clearPixels();
    delay(FRAME_DURATION);
}

void clearPixels() {
    for(int i = 0; i < strip.numPixels(); i++) strip.setPixelColor(i, 0);
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
                            if ((pPos + 1) == NUM_LEDS) {
                                pPos = 0;
                            } else {
                                pPos = pPos + 1;
                            }
                        } else { // else, if this particle rotates counter clockwise
                            if ((pPos - 1) < 0) {
                                pPos = NUM_LEDS - 1;
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
                                pPos = NUM_LEDS - 1;
                            } else {
                                pPos = pPos - 1;
                            }

                            strip.setPixelColor(pPos, strip.Color(calculateBrightness(pRVal, 10), calculateBrightness(pGVal, 10), calculateBrightness(pBVal, 10)) | strip.getPixelColor(pPos));				

                            if (pPos - 1 < 0) {
                                pPos = NUM_LEDS - 1;
                            } else {
                                pPos = pPos - 1;
                            }
                    
                            strip.setPixelColor(pPos, strip.Color(calculateBrightness(pRVal, 3), calculateBrightness(pGVal, 3), calculateBrightness(pBVal, 3)) | strip.getPixelColor(pPos));				
                        } else {
                            if (pPos + 1 == NUM_LEDS) {
                                pPos = 0;
                            } else {
                                pPos = pPos + 1;
                            }

                            strip.setPixelColor(pPos, strip.Color(calculateBrightness(pRVal, 10), calculateBrightness(pGVal, 10), calculateBrightness(pBVal, 10)) | strip.getPixelColor(pPos));				

                        if (pPos + 1 == NUM_LEDS) {
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
    Serial.println("Render Ring");
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
            for ( int i=0; i<NUM_LEDS; i++ ) {
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
            for (int i = 0; i < NUM_LEDS; i++) {
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

            for (int i = 0; i < NUM_LEDS; i++) {
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
            
            for ( int i=0; i<NUM_LEDS; i++ ) {
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
		
            for (int i = 0; i < NUM_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(rRFadeVal, rGFadeVal, rBFadeVal));
            }
        }
    }	
}

int calculateBrightness(int colorVal, int brightnessPercentage) {
    float colorValue = (float)colorVal;
    colorValue = colorValue / 100 * brightnessPercentage;
    return (int)colorValue;
}

void copyArray(int src[], int dest[]) {
    for (int i = 0; i < 80; i++) dest[i] = src[i];
}



