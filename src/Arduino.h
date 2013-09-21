#pragma once

#include "ofMain.h"

class Arduino {
	public:
		Arduino();
		void update();
		void sendInstruction(string message);
		void setColor(ofColor _color);
	private:
		ofSerial serial;
		ofColor color;
};
