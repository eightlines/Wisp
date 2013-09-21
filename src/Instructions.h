#pragma once

#include "ofMain.h"
#include "ofxTween.h"
#include "Utils.h"

class Instructions {
	public:
		Instructions();
		void update();
		void draw();
		void setMessage(string message);
		void setPosition(ofPoint pos);
		ofColor getColor();

		ofEvent<ofColor> COLOR_CHANGE;
	private:
		ofTrueTypeFont font;
		ofTrueTypeFont fontSmall;
		ofTrueTypeFont fontInstructions;
		ofxTween tween;
		ofxEasingLinear easing;
		vector<string> modes;
		int currentMode;
		bool bParticle;
		bool bAccepted;
		ofImage colorPicker;
		ofColor color;
		ofPoint position;
		ofPixels pix;
};
