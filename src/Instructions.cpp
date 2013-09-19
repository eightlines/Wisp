#include "Instructions.h"

Instructions::Instructions() {
	font.loadFont("fonts/CreteRound.ttf", 150);
	fontSmall.loadFont("fonts/CreteRound.ttf", 50);

	modes.push_back("HOME");
	modes.push_back("MODE");
	currentMode = 0;
}

void Instructions::update() {
}

void Instructions::draw() {
	string selectedMode = modes[currentMode];
	string label = "";
	ofRectangle bounds;

	if (selectedMode == "HOME") {
		label = "Wisp";
		bounds = font.getStringBoundingBox(label, 0, 0);
		font.drawString(label, (ofGetWidth() - bounds.width) / 2, (ofGetHeight() + bounds.height / 2) / 2);
	} else if (selectedMode == "MODE") {
		label = "Mode (Swipe up or down to set Ring or Particle)";
		bounds = fontSmall.getStringBoundingBox(label, 0, 0);
		fontSmall.drawString(label, (ofGetWidth() - bounds.width) / 2, (ofGetHeight() + bounds.height / 2) / 2);
	}
}

void Instructions::setMessage(string message) {
	if (message == "GESTURE_SWIPELEFT") {
		currentMode --;
		if (currentMode < 0) currentMode = modes.size() - 1;
	} else if (message == "GESTURE_SWIPERIGHT") {
		currentMode ++;
		if (currentMode > modes.size() - 1) currentMode = 0;
	}
}
