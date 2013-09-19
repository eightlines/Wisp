#pragma once

#include "ofMain.h"
#include "ofxTween.h"

enum panelType {
	PANEL_HOME,
	PANEL_MODE
};

class Instructions {
	public:
		Instructions();
		void update();
		void draw();
		void setMessage(string message);
	private:
		ofTrueTypeFont font;
		ofTrueTypeFont fontSmall;
		ofxTween tween;
		ofxEasingLinear easing;
		vector<string> modes;
		int currentMode;
};
