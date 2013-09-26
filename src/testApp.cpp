#include "testApp.h"
#include "run.h"


//--------------------------------------------------------------
void testApp::setup(){
    // OPENFRAMEWORKS
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
	ofEnableSmoothing();

    fontSize = 24;
    y = fontSize*1.2;
    width=3; // number of players to have above and below

    myFont.loadFont("8-BIT WONDER.ttf", fontSize);

    // SCOREBOARD
    runs.push_back(new run("jedahan",100));
    runs.push_back(new run("drlivormortis",200));
    runs.push_back(new run("steveintro",10));
    runs.push_back(new run("theterg",88));
    runs.push_back(new run("pete",75));
    runs.push_back(new run("red",160));
    runs.push_back(new run("blue",2500));
    runs.push_back(new run("pinkie",105));
    runs.push_back(new run("inkie",858));
    runs.push_back(new run("dot",7725));
    runs.push_back(new run("blinky",104));
    runs.push_back(new run("green",940));
    runs.push_back(new run("yellow",90));
    runs.push_back(new run("orange",33));
    runs.push_back(new run("hooplalo",18));

    debug = true;
    scan = true;

    name = runs[0]->name;
    cout << name << endl;

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
            name = text->getTextString();
            runs.push_back(new run(name));
            cout << "HERE COMES A NEW CHALLENGER! " + runs.back()->name << endl;
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
    fontSize = 24;
    y = ofGetHeight()/2 - (width * fontSize * 1.2);

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
            for(run * r : runs) {
                if(!(r->name.compare(name))){
                    r->score += ofToInt(ocrResult);
                    break;
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
        for(run * r: runs){
            ofDrawBitmapStringHighlight(r->name + ": " + ofToString(r->score), 20, y += 20);
        }
    } else {
        int me=0;
        ofBackground(0,0,0);
        img.draw(0, 0);
        ofSetColor(255,0,255);

        myFont.drawString("WATTCRANK",ofGetWidth()/2-100, fontSize*1.8);
        ofSetColor(255);


        // find what index the current run is in
        for(int i=0; i<runs.size(); i++){
            if(runs[i]->name==name){
                me = i;
            }
        }
        ofSetColor(0,255,255,64);
        ofRect(10,y-fontSize*1.2-20,ofGetWidth()/2+(5*fontSize),(2*width+1)*fontSize*1.2+40);
        ofSetColor(255);
        for(int j=max(me-width,0);j<=max(me+width,2*width);j++){
            if(j<runs.size()){
                if(j==me){ofSetColor(255,255,0);}
                myFont.drawString(ofToString(j) + " " + runs[j]->name, 20, y);
                myFont.drawString(ofToString(runs[j]->score), ofGetWidth()/2, y);
                y += fontSize*1.2;
                if(j==me){ofSetColor(255,255,255);}
            }
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