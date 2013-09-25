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

        string runOcr(ofPoint pos, int w, int h);
    
        ofxTesseract tess;
        ofImage img, cropped;
        string ocrResult;
        ofxUICanvas *gui;
        bool rescan;
    
    	vector<ofxMacamPs3Eye*> cameras;

        int total;
        vector<int> totals;

        bool init;

        void exit();
        void guiEvent(ofxUIEventArgs &e);

        float w, h;
        ofPoint position;

};
