#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	//GUI SINGLETON CLASS
	myGuiManager::getInstance()->setup();
	
	//we run at 60 fps!
	ofSetVerticalSync(true);
	
	simanager.setup(640,480);

}

//--------------------------------------------------------------
void testApp::update(){

	ofBackground(0, 0, 0);

	
	//GUI SINGLETON CLASS
	myGuiManager::getInstance()->update();
	
	simanager.update();
	
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	
	simanager.draw();
	
	myGuiManager::getInstance()->draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	simanager.keyPressed(key);
	//myGuiManager::getInstance()->keyReleased(key);
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

//--------------------------------------------------------------
void testApp::exit() {
	
	simanager.exit();
	
}
