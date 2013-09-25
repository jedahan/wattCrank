#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // OPENFRAMEWORKS
    ofSetVerticalSync(true);
	ofEnableSmoothing();

    // SCOREBOARD
    vector<int> totals;
    total = 0;
    name = "Jonathan";

    source = ofPoint(ofGetHeight()/2, ofGetWidth()/2);
    destination = ofPoint(source.x+10,source.y+10);
    rescan = false;
    init = false;
    reset = false;

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

    gui = new ofxUICanvas();
    gui->addLabel("wattCRANK", OFX_UI_FONT_LARGE);
    gui->addSpacer();
    gui->addTextInput("NAME", name);
    gui->addSpacer();
    gui->addToggle("RESET", &reset);
    gui->addLabel("'R' TO RESCAN", OFX_UI_FONT_MEDIUM);
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");

    // CAMERA
    vector<ofxMacamPs3EyeDeviceInfo*> deviceList = ofxMacamPs3Eye::getDeviceList();

    if (deviceList.size()>0){
        camera = new ofxMacamPs3Eye();
        camera->setDesiredFrameRate(180);
        camera->initGrabber(640, 480);
        init=true;
    }

}

void testApp::guiEvent(ofxUIEventArgs &event){
    if(event.widget->getName() == "RESET"){
        cout << "SCORE " << total << endl;
        ofxUIToggle *toggle = (ofxUIToggle *) event.widget;
        toggle->setValue(false);
        total=0;
    } else if (event.widget->getName() == "NAME"){
        ofxUITextInput *text = (ofxUITextInput *) event.widget;
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
        camera->update();
        img.setFromPixels(camera->getPixelsRef());
    }

    cropped.update();

    if(rescan) {
        ocrResult = tess.findText(cropped);
        cout << ocrResult;
        total += ofToInt(ocrResult);
        rescan = false;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    camera->draw(0,0);

	img.draw(0, 0);

    ofDrawBitmapStringHighlight(ocrResult, ofGetWidth()-60, 20);

    ofSetColor(255,0,255, 127);
    ofRect(source,destination.x-source.x,destination.y-source.y);
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
        destination.x = x;
        destination.y = y;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(!gui->isHit(x,y)) {
        source.x = x;
        source.y = y;
        destination.x = x+2;
        destination.y = y+2;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if(destination.x < source.x){
        int temp = destination.x;
        destination.x = source.x;
        source.x = temp;
    }
    if(destination.y < source.y){
        int temp = destination.y;
        destination.y = source.y;
        source.y = temp;
    }

    cropped.cropFrom(img,source.x,source.y,destination.x-source.x,destination.y-source.y);

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