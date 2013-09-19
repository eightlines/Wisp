#pragma once

#include "ofMain.h"

class Arduino {
	public:
		Arduino();
		void update();
	private:
		ofSerial serial;
};
