#include "Ring.h"

Ring::Ring(int radius) {
	bDrawNormals = false;
	bDrawTexture = false;

	createCircle(circle, radius, 100); // RAD, RES
	createNormals(circle, normals);

	for(int i = 0; i < circle.size(); i++) {
		outerRim.push_back(circle[i] + normals[i] * 30);
		innerRim.push_back(circle[i] - normals[i] * 30);
	}

	textureFbo.allocate(TEXTURE_RESOLUTION * TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_RGBA);
	resultFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}

void Ring::setSize(ofResizeEventArgs &resize) {
	resultFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}

void Ring::setColor(ofColor _color) {
	color = _color;
}

void Ring::setColorIndicator(ofColor _color) {
	colorIndicator = _color;
}

void Ring::update() {
	updateTexture();

	ofSetColor(255);
	resultFbo.begin(); {
		ofClear(0);
		glPushMatrix(); {
			glTranslatef( ofGetWidth() * 0.5, ofGetHeight() * 0.5, 0 );
			drawCircle();
			drawNormals();
			drawRim();
		} glPopMatrix();
	
		if (bDrawTexture)
			fbo.draw(0, 0);
	} resultFbo.end();
}

void Ring::updateTexture() {
	textureFbo.begin(); {
		for (int i = 0; i < TEXTURE_RESOLUTION; i++) {
			//drawGradRect(ofRectangle(i * TEXTURE_WIDTH, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT), ofColor::black, ofFloatColor(ofMap(i, 0, TEXTURE_RESOLUTION - 1, 0, 1), 0.2, 0.4));
			float r = color.r;
			float g = color.g;
			float b = color.b;

			ofColor tmpColor;
			
			if (r < 10 &&
				g < 10 &&
				b < 10) {
				tmpColor = ofColor::red;
			} else {
				tmpColor = color;
			}
			drawGradRect(ofRectangle(i * TEXTURE_WIDTH, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT), ofColor::black, tmpColor);
		}
	} textureFbo.end();
	tex = textureFbo.getTextureReference();
}

ofFbo Ring::getResult() {
	return resultFbo;
}

void Ring::drawCircle() {
	ofNoFill();
	ofEnableSmoothing();

	int t = circle.size();
	for(int i = 0; i < t; i++) {
		int j = (i + 1) % t;
		
		float p = i / (float)t;
		
		if (bDrawNormals) {
			ofSetColor(p * 255, 0, (1 - p) * 255);
			ofLine(circle[i].x, circle[i].y, circle[j].x, circle[j].y);
		}
	}
	
	ofDisableSmoothing();
}

void Ring::drawNormals() {
	ofNoFill();
	ofEnableSmoothing();

	glPushMatrix();
	glScalef(1.1, 1.1, 1.0);
	
	int t = circle.size();
	for(int i = 0; i < t; i++) {
		if (bDrawNormals) {
			ofSetColor(255, 255, 255);
			ofLine(circle[i].x, circle[i].y, circle[i].x + normals[i].x * 10, circle[i].y + normals[i].y * 10);
		}
	}
	
	glPopMatrix();

	ofDisableSmoothing();
}

void Ring::drawRim() {
	glPushMatrix();
	glScalef( 2.0, 2.0, 1.0 );
	
	//--- smoothing.
	ofStyle style = ofGetStyle();
	if(style.smoothing && !style.bFill) {
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	//--- draw rim.
	tex.bind();

	int t;
	t = innerRim.size();
	
	GLfloat* ver_coords = new GLfloat[(t + 1) * 4];
	GLfloat* tex_coords = new GLfloat[(t + 1) * 4];
	
	for( int i = 0; i < (t + 1); i++) {
		int j = i % t;
		float p = i / (float)t;
		
		ver_coords[i * 4 + 0] = innerRim[j].x;
		ver_coords[i * 4 + 1] = innerRim[j].y;
		ver_coords[i * 4 + 2] = outerRim[j].x;
		ver_coords[i * 4 + 3] = outerRim[j].y;
		
		ofPoint tx1 = tex.getCoordFromPercent(p, 1);
		ofPoint tx2 = tex.getCoordFromPercent(p, 0);
		
		tex_coords[i * 4 + 0] = tx1.x;
		tex_coords[i * 4 + 1] = tx1.y;
		tex_coords[i * 4 + 2] = tx2.x;
		tex_coords[i * 4 + 3] = tx2.y;
	}
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, ver_coords);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, (t + 1) * 2);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	delete[] ver_coords;
	delete[] tex_coords;
	
	tex.unbind();

	glPopMatrix();
}
