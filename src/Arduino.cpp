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
	string val = ofxGetSerialString(serial, '\n');

	if (val.size() > 0) {
		cout << val << endl;
	}
}

void Arduino::sendInstruction(string message) {
	send(message);
}

void Arduino::setColor(ofColor _color) {
	color = _color;
	send("COLOR");
}

void Arduino::send(string msg) {
	msg += "\n";
	serial.writeBytes((unsigned char *)msg.c_str(), msg.length());
}
