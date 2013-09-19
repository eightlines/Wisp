#include "ofApp.h"

void ofApp::setup() {
	ofHideCursor();
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetSmoothLighting(true);
	ofSetFrameRate(120);
	ofBackground(0);

	font.loadFont("fonts/CreteRound.ttf", 50);

	tracker = new Tracker();
	ring = new Ring(200);
}

void ofApp::update() {
	tracker->update();
	ring->update();
}

void ofApp::draw() {
	tracker->getDepth().draw(0, 0, 320, 240);

	ofRectangle b = font.getStringBoundingBox("LightWriter", 0, 0);
	font.drawString("LightWriter", (ofGetWidth() - b.width) / 2, (ofGetHeight() + b.height / 2) / 2);

	ofSetColor(0);
	drawDonut(ofPoint(ofGetWidth() / 2, ofGetHeight() / 2), ofGetWidth(), 339);
	ofSetColor(255);

	ring->getResult().draw(0, 0);
}

void ofApp::keyReleased(int key) {
	switch (key) {
		case 'f': ofToggleFullscreen(); break;
	}
}

void ofApp::messageReceived(ofMessage &message) {
	cout << message.message << endl;
}
