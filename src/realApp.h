#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxBox2d.h"

class realApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void exit(); 

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void guiEvent(ofxUIEventArgs &e);
//////////////////////
		vector <ofVec2f> loadPoints(string file);

		ofxBox2d					box2d;
		vector <ofxBox2dCircle>		circles;
		vector <ofxBox2dPolygon>	triangles;
		vector <ofxBox2dRect> rects;
};
