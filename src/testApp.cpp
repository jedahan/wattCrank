#include "testApp.h"
#include "run.h"

//--------------------------------------------------------------
void testApp::setup(){
    // OPENFRAMEWORKS
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
	ofEnableSmoothing();

    // SCOREBOARD
    vector<run> runs;

    run blah;
    blah.setup("jedahan",100);
    runs.push_back(blah);

    name = runs[0].name;

    int w = 50;
    source = ofPoint(ofGetWidth()/2 - w, ofGetHeight()/2 - w);
    destination = ofPoint(source.x+w,source.y+w);

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
    cropped.cropFrom(img,source.x,source.y,destination.x-source.x,destination.y-source.y);

    // UI

    gui = new ofxUICanvas();
    gui->addLabel("wattCRANK", OFX_UI_FONT_LARGE);
    gui->addSpacer();
    gui->addTextInput("NAME", name)->setAutoClear(false);;
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
        ofxUIToggle *toggle = (ofxUIToggle *) event.widget;
        toggle->setValue(false);
    } else if (event.widget->getName() == "NAME"){
        ofxUITextInput *text = (ofxUITextInput *) event.widget;

        if(text->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
        {
            run blah;
            blah.setup(text->getTextString());
            runs.push_back(blah);
            name = text->getTextString();
        }
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

    // Every 60 frames, rescan
    if(ofGetFrameNum()>180 && !(ofGetFrameNum()%60)) {
        ocrResult = tess.findText(cropped);
        for(run r : runs){
            if(r.name.compare(name)==0){
                r.score += ofToInt(ocrResult);
            }
        }
    }

    ofSort(runs,run::sort);
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

    int y = 30;
    for(run r: runs){
        ofDrawBitmapStringHighlight(r.name + ": " + ofToString(r.score), ofGetWidth()-100, y += 20);
    }

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