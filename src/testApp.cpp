#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // OPENFRAMEWORKS
    ofSetVerticalSync(true);
	ofEnableSmoothing();

    // SCOREBOARD
    vector<int> totals;
    total = 0;
    source = ofPoint(ofGetHeight()/2, ofGetWidth()/2);
    destination = ofPoint(source.x+100,source.y+60);
    rescan = false;
    init = false;
    name = "Jonathan";

    // OCR
	tess.setup();
	tess.setWhitelist("1234567890.");
	tess.setBlacklist("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,");
	tess.setAccuracy(ofxTesseract::ACCURATE);
    tess.setMode(ofxTesseract::WORD);

    // SAMPLE IMAGE
	img.loadImage("joule00.jpg");
	img.setImageType(OF_IMAGE_GRAYSCALE);
	img.update();

    // UI
    float dim = 16;

    gui = new ofxUICanvas();
    gui->addLabel("wattCRANK", OFX_UI_FONT_LARGE);
    gui->addSpacer();
    gui->addTextInput("NAME", name);
    gui->addSpacer();
    gui->addLabel("'R' TO RESCAN", OFX_UI_FONT_MEDIUM);
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");

    // CAMERA
    vector<ofxMacamPs3EyeDeviceInfo*> deviceList = ofxMacamPs3Eye::getDeviceList();

    if (deviceList.size()>0){
        for (int i = 0; i < deviceList.size(); i++) {
            ofxMacamPs3Eye * ps3eye = new ofxMacamPs3Eye();
            ps3eye->setDeviceID(deviceList[i]->id);
            ps3eye->setDesiredFrameRate(180);
            ps3eye->initGrabber(320, 240);
            cameras.push_back(ps3eye);
        }
        init=true;
    }

}

void testApp::guiEvent(ofxUIEventArgs &e){
    if(e.widget->getName() == "RESET"){
        cout << "SCORE " << total << endl;
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        toggle->setValue(false);
        total=0;
    } else if (e.widget->getName() == "NAME"){
        ofxUITextInput *text = (ofxUITextInput *) e.widget;
        name = text->getTextString();
        text->setTextString(name);
        cout << "NAME " << name << endl;
    }
}

void testApp::exit()
{
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}

//--------------------------------------------------------------
void testApp::update(){

    if(init){
        cameras[0]->update();
        img.setFromPixels(cameras[0]->getPixelsRef());
    }

    if(rescan) {
        ocrResult = tess.findText(cropped);
        cout << ocrResult;
        total += ofToInt(ocrResult);
        rescan = false;
    }

    cropped.cropFrom(img,source.x,source.y,destination.x-source.x,destination.y-source.y);
    cropped.update();

}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
    for (int i = 0; i < cameras.size(); i++) {
		cameras[i]->draw(i * cameras[i]->getWidth(),0);
	}
	
	if(cameras.size() == 0){
		ofDrawBitmapString("No PS3Eye found. :(", 20, 20);
	}

    ofDrawBitmapStringHighlight(ocrResult, 20, 60);

	ofSetColor(255);
	img.draw(0, 0);
    ofSetColor(0,255,0);
	cropped.draw(source.x, source.y);
	ofSetColor(255);

    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key)
    {
        case 'g':
            gui->toggleVisible();
            break;
        case 'j':
            img.loadImage("joule00.jpg");
            break;
        case 'r':
            rescan = true;
            break;
        case 'f':
			ofToggleFullscreen();
			break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if(!gui->isHit(x,y)) {
        this->destination.x = x;
        this->destination.y = y;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(!gui->isHit(x,y)) {
        this->source.x = x;
        this->source.y = y;
        this->destination.x = x;
        this->destination.y = y;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    rescan = true;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}