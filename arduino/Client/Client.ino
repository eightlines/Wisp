#include <Streaming.h>
#include <SPI.h>
#include <LPD8806.h>
#include <ble.h>

enum State{
    STATE_OFF,
    STATE_ON,
    STATE_BLINK,
    STATE_FADE,
    STATE_PARTICLE
} state;

#define LPD_CLOCK             5
#define LPD_DATA              4
#define NUM_LEDS              32
#define FRAME_DURATION        25 // ms
#define COUNTER_LIMIT         10240 // reset limit
#define PARTICLES_MAX         5

long previousMillis         = 0;
int counter                 = 0; // count loop executions
int rgb[]                   = {0, 0, 0};
int rate                    = 32;
int pos                     = 0;
int coronaL                 = 0;
int coronaT                 = 0;
int dir                     = 1;
int style                   = 0;
LPD8806 leds                = LPD8806(NUM_LEDS, LPD_DATA, LPD_CLOCK);

void setup() {
    Serial.begin(9600);
    Serial << "WISP Ready" << endl;

    BLE_setup();

    leds.begin();
    ledClear();
    leds.show();
}

void loop() {
    BLE_loop();
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > FRAME_DURATION) {
        previousMillis = millis();
        
        counter++;
	if (counter > COUNTER_LIMIT) counter = 0;

        setState(style);
        switch (state) {
            case STATE_OFF:      ledClear();    break;
            case STATE_ON:       ledOn();       break;
            case STATE_BLINK:    ledBlink();    break;
            case STATE_FADE:     ledFade();     break;
            case STATE_PARTICLE: ledParticle(); break;
        }

        leds.show();
        ledClear();
    }
}

void ledClear() {
    for(int i = 0; i < leds.numPixels(); i++)
        leds.setPixelColor(i, 0, 0, 0);
}

void ledOn() {
    for (int i = 0; i < leds.numPixels(); i++) 
        leds.setPixelColor(i, rgb[0], rgb[1], rgb[2]);
}

void ledBlink() {
    if (counter % rate == 0)
        for (int i = 0; i < leds.numPixels(); i++) 
            leds.setPixelColor(i, rgb[0], rgb[1], rgb[2]);
}

void ledFade() {
    int rFade = (rgb[0] > 0) ? calculateBrightness(rgb[0], counter) : 0;
    int gFade = (rgb[1] > 0) ? calculateBrightness(rgb[1], counter) : 0;
    int bFade = (rgb[2] > 0) ? calculateBrightness(rgb[2], counter) : 0;

    for (int i = 0; i < rate; i++)
        if ((counter + i) % rate == 0 || (counter - i) % rate == 0)
            for (int i = 0; i < leds.numPixels(); i++) 
                leds.setPixelColor(i, rFade, gFade, bFade);
}

void ledParticle() {
    // [0] PARTICLE STARTING POSITION (0-31)
    // [1] ROTATION DIRECTION (0 CW, 1 CCW)
    // [2] PARTICLE SPEED (LOW-FAST > HIGH-SLOW, 1 EVERY FRAME, 4 EVERY 4TH FRAME)
    // [3] LEADING CORONA LENGTH
    // [4] TRAILING CORONA LENGTH

    if (counter % rate == 0) {
        if (dir == 1) { // CCW PARTICLE MOVEMENT
            pos = ((pos + 1) == NUM_LEDS) ? 0 : pos ++;
        } else { // CW PARTICLE MOVEMENT
            pos = ((pos - 1 < 0)) ? pos = NUM_LEDS - 1 : pos --;
        }
        
        leds.setPixelColor(pos, leds.Color(rgb[0], rgb[1], rgb[2]) | leds.getPixelColor(pos));
        int bri = 0;
        
        if (coronaL > 0) {
            if (dir == 1) { // CW ROTATION
                for (int i = 0; i < 2; i++) {
                    pos = ((pos - 1) < 0) ? NUM_LEDS - 1 : pos --;
                    bri = (i == 0) ? 10 : 3;
                    leds.setPixelColor(pos, leds.Color(calculateBrightness(rgb[0], bri), calculateBrightness(rgb[1], bri), calculateBrightness(rgb[2], bri)) | leds.getPixelColor(pos));
                }
            } else { // CCW ROTATION
                for (int i = 0; i < 2; i++) {
                    pos = ((pos + 1) == NUM_LEDS) ? 0 : pos ++;
                    bri = (i == 0) ? 10 : 3;
                    leds.setPixelColor(pos, leds.Color(calculateBrightness(rgb[0], bri), calculateBrightness(rgb[1], bri), calculateBrightness(rgb[2], bri)) | leds.getPixelColor(pos));
                }
            }
        }
    }
}

int calculateBrightness(int colorVal, int brightnessPercentage) {
    if (brightnessPercentage < 1) return 0;

    float colorValue = (float)colorVal;
    colorValue = colorValue / 100 * brightnessPercentage;
    return (int)colorValue;
}

void setState(int style) {
    switch (style) {
        case 1: state = STATE_ON; break;
        case 2: state = STATE_BLINK; break;
        case 3: state = STATE_FADE; break;
        case 4: state = STATE_PARTICLE; break;
        default: state = STATE_OFF;
    }
}

