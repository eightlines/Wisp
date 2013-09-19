#include "ofApp.h"

void ofApp::setup() {
	ofHideCursor();
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofSetSmoothLighting(true);
	ofSetFrameRate(120);
	ofBackground(0);

	tracker = new Tracker();
	arduino = new Arduino();
	ring = new Ring(200);
	instructions = new Instructions();

	bShowDepth = true;
}

void ofApp::update() {
	tracker->update();
	arduino->update();
	ring->update();
	instructions->update();
}

void ofApp::draw() {
	instructions->draw();

	ofSetColor(0);
	drawDonut(ofPoint(ofGetWidth() / 2, ofGetHeight() / 2), ofGetWidth(), 339);
	ofSetColor(255);

	ring->getResult().draw(0, 0);
	if (bShowDepth) {
		ofDrawBitmapString(gesture, 10, ofGetHeight() - (tracker->getDepth().getHeight() / 2) - 10);
		tracker->getDepth().draw(0, ofGetHeight() - tracker->getDepth().getHeight() / 2, 320 / 2, 240 / 2);
	}
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
	gesture = message.message;
	if (gesture != "") instructions->setMessage(gesture);
}
