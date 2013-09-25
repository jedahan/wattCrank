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
    debug = true;
    scan = true;

    name = runs[0].name;

    int w = 50;
    source = ofPoint(ofGetWidth()/2 - w, ofGetHeight()/2 - w);
    destination = ofPoint(source.x+w,source.y+w);

    init = false;
    dragging = false;

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
    gui->addToggle("STOP", &scan);
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
    string ev = event.widget->getName();
    if( ev == "START" || ev == "STOP"){
        ofxUIToggle *toggle = (ofxUIToggle *) event.widget;
        scan = toggle->getValue();
        if(scan){
            toggle->getLabelWidget()->setLabel("STOP");
        } else {
            toggle->getLabelWidget()->setLabel("START");
        }
    } else if (ev == "NAME"){
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

    if(!dragging && scan){
        if(init){
            camera->update();
            img.setFromPixels(camera->getPixelsRef());
        }

        cropped.cropFrom(img,source.x,source.y,destination.x-source.x,destination.y-source.y);
        cropped.update();

        // Every 60 frames, rescan
        if(ofGetFrameNum()>180 && !(ofGetFrameNum()%60)) {
            ocrResult = tess.findText(cropped);
            for(int i=0; i<runs.size();i++){
                if(runs[i].name.compare(name)==0){
                    runs[i].score += ofToInt(ocrResult);
                }
            }
        }

        ofSort(runs,run::sort);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

//    camera->draw(0,0);

    if(debug){
        img.draw(0, 0);

        ofDrawBitmapStringHighlight(ocrResult, ofGetWidth()-60, 20);

        ofSetColor(255,0,255, 127);
        ofRect(source,destination.x-source.x,destination.y-source.y);
        ofSetColor(255);

        int y = 160;
        for(run r: runs){
            ofDrawBitmapStringHighlight(r.name + ": " + ofToString(r.score), 20, y += 20);
        }
    }

    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key)
    {
        case '1':
            gui->toggleVisible();
            break;
        case '2':
            debug = ! debug;
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
    if(debug){
        if(!gui->isHit(x,y)) {
            destination.x = x;
            destination.y = y;
        }
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(debug){
        if(!gui->isHit(x,y)) {
            source.x = x;
            source.y = y;
            destination.x = x+2;
            destination.y = y+2;
        }
    }
    dragging = true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if(debug){
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
    }
    dragging = false;
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