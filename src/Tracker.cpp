#include "Tracker.h"

Tracker::Tracker() {
	EnableGesture();
	Init();

	pxcU32 w = 320;
	pxcU32 h = 240;

	if (QueryImageSize(PXCImage::IMAGE_TYPE_DEPTH, w, h)) {
		tex.allocate(w, h, GL_LUMINANCE);
		labelMap = new unsigned char[w * h];
	}
}

void Tracker::update() {
	if (!AcquireFrame(false)) return;
	if (!labelMap) return;

	PXCImage *image;
	if (QueryGesture()->QueryBlobImage(PXCGesture::Blob::LABEL_SCENE, 0, &image) >= PXC_STATUS_NO_ERROR) {
		PXCImage::ImageData data;
		if (image->AcquireAccess(PXCImage::ACCESS_READ, &data) >= PXC_STATUS_NO_ERROR) {
			tex.loadData(data.planes[0], tex.getWidth(), tex.getHeight(), GL_LUMINANCE);
			image->ReleaseAccess(&data);
		}
	}

	PXCGesture::Gesture *gesture = new PXCGesture::Gesture();
	if (QueryGesture()->QueryGestureData(0, PXCGesture::GeoNode::LABEL_BODY_HAND_PRIMARY, 0, gesture) >= PXC_STATUS_NO_ERROR) {
		string name = "";
		switch (gesture->label) {
			case PXCGesture::Gesture::LABEL_HAND_CIRCLE: name = "CIRCLE"; break;
			case PXCGesture::Gesture::LABEL_HAND_WAVE: name = "WAVE"; break;
			case PXCGesture::Gesture::LABEL_POSE_BIG5: name = "HIGH5"; break;
			case PXCGesture::Gesture::LABEL_POSE_THUMB_UP: name = "THUMBUP"; break;
			case PXCGesture::Gesture::LABEL_POSE_THUMB_DOWN: name = "THUMBDOWN"; break;
			case PXCGesture::Gesture::LABEL_NAV_SWIPE_DOWN: name = "SWIPEDOWN"; break;
			case PXCGesture::Gesture::LABEL_NAV_SWIPE_LEFT: name = "SWIPELEFT"; break;
			case PXCGesture::Gesture::LABEL_NAV_SWIPE_RIGHT: name = "SWIPERIGHT"; break;
			case PXCGesture::Gesture::LABEL_NAV_SWIPE_UP: name = "SWIPEUP"; break;
		}

		if (name != "") {
			if (!timeout.isRunning()) {
				timeout.setParameters(easing, ofxTween::easeIn, 0, 1, 1000, 0); // ONLY SEND ONCE PER SECOND
				ofSendMessage("GESTURE_" + name);
			} 
		}
	}

	if (timeout.isRunning()) timeout.update();

	ReleaseFrame();
}

ofTexture Tracker::getDepth() {
	return tex;
}
