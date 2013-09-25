#pragma once

#include "ofMain.h"
#include "ofxTesseract.h"
#include "ofxMacamPs3Eye.h"
#include "ofxUI.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofxTesseract tess;
        ofImage img, cropped;
        ofPoint source, destination;

        ofxUICanvas *gui;

        string ocrResult;
        bool init;
        bool rescan;

        string name;

    	vector<ofxMacamPs3Eye*> cameras;

        int total;
        vector<int> totals;

        void exit();
        void guiEvent(ofxUIEventArgs &e);
};
