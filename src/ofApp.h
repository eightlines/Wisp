#pragma once

#include "ofMain.h"
#include "Utils.h"
#include "Arduino.h"
#include "Ring.h"
#include "Tracker.h"
#include "Instructions.h"

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void keyReleased(int key);
		void windowResized(ofResizeEventArgs &resize);
		void messageReceived(ofMessage &message);
	private:
		void geoNodePositonChange(ofPoint &pos);
		void colorChange(ofColor &color);

		Tracker *tracker;
		Arduino *arduino;
		Ring *ring;
		Instructions *instructions;

		bool bShowDepth;

		string gesture;
};
