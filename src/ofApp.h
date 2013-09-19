#pragma once

#include "ofMain.h"
#include "Tracker.h"

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void keyReleased(int key);
		void messageReceived(ofMessage &message);
	private:
		Tracker *tracker;
};
