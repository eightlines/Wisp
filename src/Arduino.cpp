#include "Arduino.h"

Arduino::Arduino() {
	vector<ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	for (int i = 0; i < deviceList.size(); i++) {
		cout << deviceList[i].getDevicePath() << endl;
	}

	if (serial.setup("COM2", 9600)) {
		cout << "ARDUINO CONNECTED" << endl;
	}
}

void Arduino::update() {
}

void Arduino::sendInstruction(string message) {
}

void Arduino::setColor(ofColor _color) {
	color = _color;
}