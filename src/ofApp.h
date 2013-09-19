#pragma once

#include "ofMain.h"
#include "Utils.h"
#include "Arduino.h"
#include "Ring.h"
#include "Tracker.h"

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void keyReleased(int key);
		void windowResized(ofResizeEventArgs &resize);
		void messageReceived(ofMessage &message);
	private:
		Tracker *tracker;
		Arduino *arduino;
		Ring *ring;
		ofTrueTypeFont font;
		bool bShowDepth;
};
