#include "ofApp.h"

void ofApp::setup() {
	tracker = new Tracker();
}

void ofApp::update() {
	tracker->update();
}

void ofApp::draw() {
	tracker->getDepth().draw(0, 0, 320, 240);
}

void ofApp::keyReleased(int key) {

}

void ofApp::messageReceived(ofMessage &message) {
	cout << message.message << endl;
}
