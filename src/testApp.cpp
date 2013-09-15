#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    rescan = true;
	panel.setup();
	panel.addPanel("Preprocessing");
    
	tess.setup();
	tess.setWhitelist("1234567890");
	tess.setBlacklist("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,");
	tess.setAccuracy(ofxTesseract::ACCURATE);
    tess.setMode(ofxTesseract::WORD);
	
	img.loadImage("joule00.jpg");
	img.setImageType(OF_IMAGE_GRAYSCALE);
	img.update();
    
    panel.addSlider("x", "x", 222, 0, img.getWidth());
    panel.addSlider("y", "y", 160, 0, img.getHeight());
    panel.addSlider("w", "w", 64, 0, img.getWidth());
    panel.addSlider("h", "h", 32, 0, img.getHeight());
    
    vector<ofxMacamPs3EyeDeviceInfo*> deviceList = ofxMacamPs3Eye::getDeviceList();
	
	for (int i = 0; i < deviceList.size(); i++) {
		ofxMacamPs3Eye * camera = new ofxMacamPs3Eye();
		camera->setDeviceID(deviceList[i]->id);
		camera->setDesiredFrameRate(180);
		camera->initGrabber(320, 240);
		cameras.push_back(camera);
	}
	
}

//--------------------------------------------------------------
void testApp::update(){
    for (int i = 0; i < cameras.size(); i++) {
		cameras[i]->update();
	}

    if(rescan) {
        ocrResult = runOcr(panel.getValueI("x"), panel.getValueI("y"), panel.getValueI("w"), panel.getValueI("h"));
        cropped.update();
        panel.clearAllChanged();
        cout << ocrResult;
        rescan = false;
    }
}

// run the ocr
string testApp::runOcr(int x, int y, int w, int h) {
    img.setFromPixels(cameras[0]->getPixelsRef());
	cropped.cropFrom(img,x,y,w,h);
	return tess.findText(cropped);
}



//--------------------------------------------------------------
void testApp::draw(){
	ofPushMatrix();
	
	ofTranslate(300, 0);
	
    for (int i = 0; i < cameras.size(); i++) {
		cameras[i]->draw(i * cameras[i]->getWidth(),0);
		ofDrawBitmapString(ofToString(cameras[i]->getRealFrameRate()), i * cameras[i]->getWidth() + 20, 20);
	}
	
	if(cameras.size() == 0){
		ofDrawBitmapString("No PS3Eye found. :(", 20, 20);
	}

	ofPushMatrix();
	
	ofTranslate(0, 480);
    
	ofSetColor(0);
	vector<string> lines = ofSplitString(ocrResult, "\n");
	for(int i = 0; i < lines.size(); i++) {
		ofDrawBitmapString(lines[i], 10, 20 + i * 12);
	}
	
	ofSetColor(255);
	img.draw(0, 0);
    ofSetColor(255,0,0);
	cropped.draw(panel.getValueI("x"), panel.getValueI("y"));
	ofSetColor(255);

	ofPopMatrix();
	
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    rescan = true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
