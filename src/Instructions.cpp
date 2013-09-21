#include "Instructions.h"

Instructions::Instructions() {
	font.loadFont("fonts/CreteRound.ttf", 130);
	fontSmall.loadFont("fonts/CreteRound.ttf", 50);
	fontInstructions.loadFont("fonts/CreteRound.ttf", 20);

	fontSmall.setSpaceSize(.5);
	fontInstructions.setSpaceSize(.5);

	modes.push_back("HOME");
	modes.push_back("MODE");
	modes.push_back("COLOR");

	currentMode = 0;
	bParticle = true;

	colorPicker = drawColorPicker(600, 600);
	pix.setFromPixels(colorPicker.getPixels(), colorPicker.getWidth(), colorPicker.getHeight(), OF_IMAGE_COLOR);
	color = ofColor::red;

	position = ofPoint(0, 0, 100);
}

void Instructions::update() {
}

void Instructions::draw() {
	string selectedMode = modes[currentMode];
	string label = "";
	ofRectangle bounds;

	ofPushMatrix();
	ofTranslate((ofGetWidth() - 600) / 2, (ofGetHeight() - 600) / 2);

	if (selectedMode == "HOME") {
		label = "Wisp";
		bounds = font.getStringBoundingBox(label, 0, 0);
		font.drawString(label, (600 - bounds.width) / 2, (600 + bounds.height / 2) / 2);
	} else if (selectedMode == "MODE") {
		label = "Mode";
		bounds = fontSmall.getStringBoundingBox(label, 0, 0);
		fontSmall.drawString(label, (600 - bounds.width) / 2, 80);

		label = "Swipe Up or Down to set mode.";
		bounds = fontInstructions.getStringBoundingBox(label, 0, 0);
		fontInstructions.drawString(label, (600 - bounds.width) / 2, 120);

		label = "Thumbs up to accept.";
		bounds = fontInstructions.getStringBoundingBox(label, 0, 0);
		fontInstructions.drawString(label, (600 - bounds.width) / 2, 170);

		label = (bParticle) ? "Particle" : "Ring";
		bounds = font.getStringBoundingBox(label, 0, 0);
		font.drawString(label, (600 - bounds.width) / 2, (600 + bounds.height / 2) / 2);

		if (bAccepted) {
			label = "Instruction accepted.";
			bounds = fontInstructions.getStringBoundingBox(label, 0, 0);
			fontInstructions.drawString(label, (600 - bounds.width) / 2, 500);
		}
	} else if (selectedMode == "COLOR") {
		colorPicker.draw(0, 0);

		label = "Color";
		bounds = fontSmall.getStringBoundingBox(label, 0, 0);
		fontSmall.drawString(label, (600 - bounds.width) / 2, 80);

		if ((position.x >= 0 && position.x < 600) 
			&& (position.y >= 0 && position.y < 600)
			&& (position.z >= 0 && position.z <= 100)) {

			ofCircle(ofPoint(position.x, position.y), position.z);
			color = pix.getColor(position.x, position.y);

			ofFill();
			ofSetColor(0);
			ofCircle(ofPoint(((600 - bounds.width) / 2) - 50, 60), 20);
			ofSetColor(color);
			ofCircle(ofPoint(((600 - bounds.width) / 2) - 51, 61), 18);
			ofSetColor(255);

			ofSendMessage("CHANGE-COLOR");
		}

		bAccepted = (position.z > 15) ? true : false;

		if (bAccepted) {
			label = "Instruction accepted.";
			bounds = fontInstructions.getStringBoundingBox(label, 0, 0);
			fontInstructions.drawString(label, (600 - bounds.width) / 2, 500);
			ofSendMessage("CHANGE-COLOR-SELECTED");
		}
	}

	ofPopMatrix();
}

void Instructions::setMessage(string message) {
	if (message == "GESTURE_SWIPELEFT") {
		currentMode --;
		if (currentMode < 0) currentMode = modes.size() - 1;
		bAccepted = false; // RESET ACCEPTED INDICATOR
	} else if (message == "GESTURE_SWIPERIGHT") {
		currentMode ++;
		if (currentMode > modes.size() - 1) currentMode = 0;
		bAccepted = false; // RESET ACCEPTED INDICATOR
	} else if (message == "GESTURE_SWIPEUP") {
		if (currentMode == 1) { // PARTICLE OR RING SELECT
			bParticle = true;
		}
		bAccepted = false; // RESET ACCEPTED INDICATOR
	} else if (message == "GESTURE_SWIPEDOWN") {
		if (currentMode == 1) { // PARTICLE OR RING SELECT
			bParticle = false;
		}
		bAccepted = false; // RESET ACCEPTED INDICATOR
	} else if (message == "GESTURE_THUMBUP") {
		if (currentMode == 1) { // PARTICLE OR RING SELECT
			ofSendMessage("CHANGE_MODE-" + (bParticle) ? "PARTICLE" : "RING");
			bAccepted = true;
		}
	} else if (message == "GESTURE_THUMBDOWN") {
		bAccepted = false; // RESET ACCEPTED INDICATOR
	}
}

void Instructions::setPosition(ofPoint pos) {
	position = ofPoint(pos.x - ((ofGetWidth() - 600) / 2), pos.y - ((ofGetHeight() - 600) / 2), pos.z);
}

ofColor Instructions::getColor() {
	return color;
}