#pragma once

#include "ofMain.h"
#include "Utils.h"

class Arduino {
	public:
		Arduino();
		void update();
		void sendInstruction(string message);
		void setColor(ofColor _color);
	private:
		void send(string msg);
		ofSerial serial;
		ofColor color;
};
