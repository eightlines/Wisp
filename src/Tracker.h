#pragma once

#include "ofMain.h"
#include "util_pipeline.h"
#include "ofxTween.h"

class Tracker : public UtilPipeline {
	public:
		Tracker();
		void update();
		ofTexture getDepth();
	private:
		ofTexture tex;
		unsigned char *labelMap;
		ofxTween timeout;
		ofxEasingLinear easing;
};
