#pragma once

#include "ofMain.h"
#include "ofxTesseract.h"
#include "ofxMacamPs3Eye.h"
#include "ofxUI.h"
#include "ofTrueTypeFont.h"
#include "run.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

        ofxTesseract tess;
        ofImage img, cropped;
        ofPoint source, destination;

        ofxUICanvas *gui;

        string ocrResult;
        bool init;
        bool reset;
        bool debug;
        bool dragging;
        bool scan;

        int fontSize;
        float y;
        int width;
        float seconds;
        float lastFrameNum;
        float maxSeconds;

        string name;
        int runIndex;

        ofxMacamPs3Eye * camera;

        vector<run*> runs;

        void exit();
        void guiEvent(ofxUIEventArgs &e);

        ofTrueTypeFont myFont, secondsFont;
};
