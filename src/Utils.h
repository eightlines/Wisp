#pragma once

#include "ofMain.h"

static string ofxTrimStringRight(string str)
{
    size_t endpos = str.find_last_not_of(" \t\r\n");
    return (string::npos != endpos) ? str.substr( 0, endpos+1) : str;
}

static string ofxTrimStringLeft(string str)
{
    size_t startpos = str.find_first_not_of(" \t\r\n");
    return (string::npos != startpos) ? str.substr(startpos) : str;
}

static string ofxTrimString(string str)
{
    return ofxTrimStringLeft(ofxTrimStringRight(str));;
}

static string ofxGetSerialString(ofSerial &serial, char until)
{
    static string str;
    stringstream ss;
    char ch;
    int ttl=1000;
    while ((ch=serial.readByte())>0 && ttl-->0 && ch!=until) {
        ss << ch;
    }
    str+=ss.str();
    if (ch==until) {
        string tmp=str;
        str="";
        return ofxTrimString(tmp);
    } else {
        return "";
    }
}

static ofImage createMaskImage(ofImage image, ofImage alphaMap) {
	ofImage result;

	if (image.width == alphaMap.width &&
		image.height == alphaMap.height) {
		
		result.allocate(image.width, image.height, OF_IMAGE_COLOR);
		
		unsigned char *pMask = alphaMap.getPixels();
		unsigned char *pImage = image.getPixels();
		unsigned char *pResult = new unsigned char[image.width * image.height * 3];

		for (int y = 0; y < image.height; y++) {
			for (int x = 0; x < image.width; x++) {
				int i = (y * image.width * 3) + (x * 3);
				pResult[i] = pImage[i] * (pMask[i]/255.0f);
				pResult[i + 1] = pImage[i + 1] * (pMask[i]/255.0f);
				pResult[i + 2] = pImage[i + 2] * (pMask[i]/255.0f);
			}
		}

		result.setFromPixels(pResult, image.width, image.height, OF_IMAGE_COLOR);
		delete pResult;
	}

	return result;
}

static ofImage convertFboToImage(ofFbo source) {
	ofImage result;
	result.allocate(source.getWidth(), source.getHeight(), OF_IMAGE_COLOR);

	ofPixels pix;
	source.readToPixels(pix);

	result.setFromPixels(pix);
	result.update();
	return result;
}

static void drawDonut(ofPoint center, float outerRadius, float innerRadius, float degrees = 360) {
	GLUquadricObj *quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);
	gluQuadricTexture(quad, GLU_SMOOTH);

	glPushMatrix(); {
		ofTranslate(center);
		gluPartialDisk(quad, innerRadius, outerRadius, 64, 1, 0, degrees);
	} glPopMatrix();
}

static void drawGradRect(ofRectangle rect, ofFloatColor from, ofFloatColor to) {
	glBegin(GL_QUAD_STRIP);  
	
		glColor3f(from.r, from.g, from.b);
		glVertex3f(rect.x, rect.y, 0);
		glVertex3f(rect.x + rect.width, rect.y, 0);  

		glColor3f(to.r, to.g, to.b);
		
		glVertex3f(rect.x, rect.y + rect.height, 0);
		glVertex3f(rect.x + rect.width, rect.y + rect.height, 0);

	glEnd();  
}

static ofImage drawColorPicker(int w, int h) {
	float cx = w/2;
	float cy = h/2;
	ofImage img;

	img.allocate(w, h, OF_IMAGE_COLOR);

	for (float y = 0; y < h; y++) {
		for (float x = 0; x < w; x++) {
			float angle = atan2(y - cy, x - cy) + PI;
			float dist = ofDist(x, y, cx, cy);
			float hue = angle / TWO_PI * 255;
			float sat = ofMap(dist, 0, w / 4, 0, 255, true);
			float bri = ofMap(dist, w / 4, w / 2, 255, 0, true);

			img.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
		}
	}

	img.reloadTexture();
	return img;
}