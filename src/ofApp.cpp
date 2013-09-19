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
	arduino = new Arduino();
	ring = new Ring(200);

	bShowDepth = true;
}

void ofApp::update() {
	tracker->update();
	arduino->update();
	ring->update();
}

void ofApp::draw() {
	ofRectangle b = font.getStringBoundingBox("LightWriter", 0, 0);
	font.drawString("LightWriter", (ofGetWidth() - b.width) / 2, (ofGetHeight() + b.height / 2) / 2);

	ofSetColor(0);
	drawDonut(ofPoint(ofGetWidth() / 2, ofGetHeight() / 2), ofGetWidth(), 339);
	ofSetColor(255);

	ring->getResult().draw(0, 0);
	if (bShowDepth) tracker->getDepth().draw(0, ofGetHeight() - tracker->getDepth().getHeight() / 2, 320 / 2, 240 / 2);
}

void ofApp::keyReleased(int key) {
	switch (key) {
		case 'f': ofToggleFullscreen(); break;
		case 'h': bShowDepth = !bShowDepth; break;
	}
}

void ofApp::windowResized(ofResizeEventArgs &resize) {
	ring->setSize(resize);
}

void ofApp::messageReceived(ofMessage &message) {
	cout << message.message << endl;
}
