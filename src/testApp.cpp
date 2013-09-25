#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // OPENFRAMEWORKS
    ofSetVerticalSync(true);
	ofEnableSmoothing();

    // SCOREBOARD
    vector<int> totals;
    total = 0;
    position = ofPoint(ofGetHeight()/2, ofGetWidth()/2);
    w = h = 120;
    rescan = true;
    init = false;

    // OCR
	tess.setup();
	tess.setWhitelist("1234567890");
	tess.setBlacklist("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,");
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
    gui->addSlider("W", 0, 320, &w, 95, dim);
    gui->addSlider("H", 0, 240, &h, 95, dim);
    gui->addSpacer();
    gui->add2DPad("CENTER", ofPoint(0,img.width), ofPoint(0,img.height), &position, img.width/8, img.height/8);
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
    } else if(e.widget->getName() == "W") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        w = slider->getScaledValue();
    } else if(e.widget->getName() == "H") {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        h = slider->getScaledValue();
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
    }

    if(rescan) {
        ocrResult = runOcr(position,w,h);
        cropped.update();
        cout << ocrResult;
        total += ofToInt(ocrResult);
        rescan = false;
    }
}

// run the ocr
string testApp::runOcr(ofPoint pos, int w, int h) {
    if(cameras.size() > 0){
        img.setFromPixels(cameras[0]->getPixelsRef());
    }
    cropped.cropFrom(img,pos.x,pos.y,w,h);
    return tess.findText(cropped);
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
	cropped.draw(position.x, position.y);
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
    this->position.x = x;
    this->position.y = y;
    rescan = true;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    this->position.x = x;
    this->position.y = y;
    rescan = true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

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