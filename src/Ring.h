#pragma once

#include "ofMain.h"
#include "Utils.h"

const int TEXTURE_RESOLUTION = 100;
const int TEXTURE_WIDTH = 10;
const int TEXTURE_HEIGHT = 100;

class Ring {

	public:
		Ring(int radius = 150);
		void update();
		ofFbo getResult();
		void setSize(ofResizeEventArgs &resize);

		template <typename T>
		void createCircle ( vector<T>& points, float radius, int resolution )
		{
			int t = resolution;
			for( int i=0; i<t; i++ )
			{
				float rad;
				rad = i / (float)t * TWO_PI - PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
			
				points.push_back( T() );
				points.back().x = radius * cos( rad );
				points.back().y = radius * sin( rad );
			}
		}
	
		template <typename T>
		void createNormals ( const vector<T>& line, vector<T>& normals )
		{
			int t = line.size();
			for( int i=0; i<t; i++ )
			{
				int j = ( i + 1 ) % t;
			
				float x = line[ j ].x - line[ i ].x;
				float y = line[ j ].y - line[ i ].y;
			
				float length = (float)sqrt( x * x + y * y );
				if( length > 0 )
				{
					float _x = x;
					x = y / length;
					y = -( _x / length );
				}
			
				normals.push_back( T() );
				normals.back().x = x;
				normals.back().y = y;
			}
		}

	private:
		void drawCircle();
		void drawNormals();
		void drawRim();

		vector<ofPoint> circle;
		vector<ofPoint> normals;
		vector<ofPoint> innerRim;
		vector<ofPoint> outerRim;
		ofTexture tex;
		ofFbo fbo, resultFbo;
		ofTrueTypeFont font;
		bool bDrawNormals;
		bool bDrawTexture;
};
