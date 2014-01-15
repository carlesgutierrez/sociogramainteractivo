/*
 *  myGuiManager.cpp
 */

#include "ofMain.h"
#include "myGuiManager.h"

// SINGLETON initalizations
bool myGuiManager::instanceFlag = false;
myGuiManager* myGuiManager::single = NULL;

//----------------------------------------------

myGuiManager* myGuiManager::getInstance()
{
    if(! instanceFlag)
    {
        single = new myGuiManager();
        instanceFlag = true;
        return single;
    }else{
        return single;
    }
}

//----------------------------------------------

myGuiManager::myGuiManager()
{
	ofAddListener(ofEvents().mouseMoved, this, &myGuiManager::mouseMove);
	ofAddListener(ofEvents().mousePressed, this, &myGuiManager::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &myGuiManager::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &myGuiManager::mouseReleased);
	ofAddListener(ofEvents().keyReleased, this, &myGuiManager::keyReleased);
}

//----------------------------------------------

myGuiManager::~myGuiManager()
{
	
	ofRemoveListener(ofEvents().mouseMoved, this, &myGuiManager::mouseMove);
	ofRemoveListener(ofEvents().mousePressed, this, &myGuiManager::mousePressed);
	ofRemoveListener(ofEvents().mouseDragged, this, &myGuiManager::mouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &myGuiManager::mouseReleased);
	ofRemoveListener(ofEvents().keyReleased, this, &myGuiManager::keyReleased);
	

}

//----------------------------------------------

void myGuiManager::setup()
{
	
	// GUI style
	//ofxControlPanel::setBackgroundColor(simpleColor(150, 150, 150, 50));
	ofxControlPanel::setTextColor(simpleColor(200, 200, 200, 255));
	ofxControlPanel::setForegroundColor(simpleColor(254, 203, 255, 170));
	
	gui.loadFont("fonts/verdana.ttf", 10);
	gui.setup("Sociograma Interactivo v3",  ofGetWidth()*0.50, 0,  ofGetWidth()*0.65, ofGetHeight()*0.90);

	
	// PANELS OF GUI
	addPanelGeneralSettings();
	
	//------------------ 
	//SETTINGS AND EVENTS
	
	//load from xml!
	gui.loadSettings("controlPanelSettings.xml");
	
	if(gui.getValueF("hidegui"))gui.toggleView();
	
	if(gui.getValueF("hidemouse")){
		bmouse = false;
		ofHideCursor();
	}
	else {
		bmouse = true;
		ofShowCursor();
	}
	
	//if you want to use events call this after you have added all your gui elements
	gui.setupEvents();
	gui.enableEvents();
	
	//  -- this gives you back an ofEvent for all events in this control panel object
	ofAddListener(gui.guiEvent, this, &myGuiManager::guiEvents);


}

//----------------------------------------------

void myGuiManager::update()
{
	gui.update();
}

//----------------------------------------------

void myGuiManager::draw()
{
	gui.draw();
}

//--------------------------------------------------------------

void myGuiManager::addPanelGeneralSettings()
{
	bool defaultValueBool = false;
	int defaultValueInt = 0;
	
	gui.setWhichPanel(0);
	gui.setWhichColumn(0);
	gui.addPanel("General settings", 4, false);
	
#ifdef _USE_LIVE_VIDEO
	
	gui.addToggle("Learn BACKGROUND","bLearnBakground" ,defaultValueBool ); // true
	gui.addSlider("Video Threshold","threshold", defaultValueInt, 0, 255, false); // 55
	
#else
	
	gui.addSlider("KINECT near Threshold","nearThreshold", defaultValueInt, 0, 255, false); // 230
	gui.addSlider("KINECT far Threshold","farThreshold", defaultValueInt, 0, 255, false); // 70
	gui.addToggle("b Thresh With OpenCV","bThreshWithOpenCV" ,defaultValueBool ); // true
	gui.addToggle("KINECT b Recording","bRecord" ,defaultValueBool ); // false
	gui.addToggle("KINECT b Playback","bPlayback" ,defaultValueBool ); // false

	gui.addSlider("KINECT angle tilt","angletilt", defaultValueInt, -30, 30, false); // 0
	gui.addSlider("Camera Threshold","camThreshold", defaultValueInt, 0, 255, false); // 70
	gui.addToggle("Invert Horizontal","InvertHorizontal" ,defaultValueBool ); //true	
	gui.addToggle("blur Heavily","blurHeavily" ,defaultValueBool ); //true	
	gui.addToggle("blur Gaussian 13","blurGaussian" ,defaultValueBool ); //true	
	gui.addToggle("blur 13","blur" ,defaultValueBool ); //true	
	
	
#endif
	
	gui.addSlider("Normalize Percentage","normalizePercentage", defaultValueInt, 0, 1, false); //	
	gui.addToggle("Give Lowest Possible IDs", "giveLowestPossibleIDs" ,defaultValueBool); //false
	gui.addSlider("Max Undetected Time","maxUndetectedTime", defaultValueInt, 0, 1000, true); //500
	gui.addSlider("Min Detected Time","minDetectedTime", defaultValueInt, 0, 3000, true); //2000
	gui.addToggle("debug Draw Candidates", "debugDrawCandidates" ,defaultValueBool ); //true
	gui.addSlider("max Merge Distance","maxMergeDis", defaultValueInt, 2, 300, true); // 20

	gui.setWhichColumn(1);
	gui.addToggle("Modo contacto", "modocontacto" ,defaultValueBool ); //true
	gui.addSlider("Circle Resolution","circleresolution", defaultValueInt, 1, 255, false); // 100
	gui.addSlider("Circle Size","circlesize", defaultValueInt, 1, 255, false); // 30
	gui.addSlider("Circle Width","circleWidth", defaultValueInt, 1, 255, false); // 30
	gui.addSlider("Linea factor grosor","factorgrosor", defaultValueInt, 250, 1000, false); // 500
	gui.addToggle("render Debug Draw", "renderDebug1Draw" ,defaultValueBool ); //true
	

	gui.setWhichColumn(2);
	gui.addToggle("render Visualizaton Draw", "rendervisualizationdraw" ,defaultValueBool ); //true
	gui.addToggle("render Trackgin Draw", "rendertrackingdraw" ,defaultValueBool ); //true
	gui.addToggle("render Extruded Camera", "renderExtrudedCam" ,defaultValueBool ); //true
	gui.addToggle("render Warp Rect", "renderWarpRect" ,defaultValueBool ); //true	
	gui.addToggle("Save WarpingCorners", "savewarpingcorners" ,defaultValueBool ); //true
	gui.addToggle("Activated WarpingCorners", "warpingcorners" ,defaultValueBool ); //true
	gui.addToggle("hide GUI", "hidegui" ,defaultValueBool ); //true
	gui.addToggle("hide Mouse", "hidemouse" ,defaultValueBool ); //true
	
	

	
}


//--------------------------------------------------------------
// This captures all our control panel events 
// - unless its setup differently in testApp::setup
//--------------------------------------------------------------

void myGuiManager::guiEvents(guiCallbackData & data)
{	
	
	// print to terminal if you want to 
	//this code prints out the name of the events coming in and all the variables passed
	printf("testApp::eventsIn - name is %s - \n", data.getXmlName().c_str());
	if( data.getDisplayName() != "" ){
		printf(" element name is %s \n", data.getDisplayName().c_str());
	}
	for(int k = 0; k < data.getNumValues(); k++){
		if( data.getType(k) == CB_VALUE_FLOAT ){
			printf("%i float  value = %f \n", k, data.getFloat(k));
		}
		else if( data.getType(k) == CB_VALUE_INT ){
			printf("%i int    value = %i \n", k, data.getInt(k));
		}
		else if( data.getType(k) == CB_VALUE_STRING ){
			printf("%i string value = %s \n", k, data.getString(k).c_str());
		}
	}
	
	printf("\n");

	
}



//----------------------------------------------

void myGuiManager::mouseMove(ofMouseEventArgs &e)
{

}

//----------------------------------------------

void myGuiManager::mouseDragged(ofMouseEventArgs &e)
{
	gui.mouseDragged(e.x, e.y, e.button);
}

//--------------------------------------------------------------
void myGuiManager::mousePressed(ofMouseEventArgs &e)
{
	gui.mousePressed(e.x, e.y, e.button);	
}

//--------------------------------------------------------------
void myGuiManager::mouseReleased(ofMouseEventArgs &e)
{
	gui.mouseReleased();
}

//--------------------------------------------------------------

void myGuiManager::keyReleased(ofKeyEventArgs &e)
{
	if( e.key == 'g' ){
		//
		gui.toggleView();
 	}
	else if(e.key == 'm'){
		
		bmouse = !bmouse;
		
		if (bmouse == true){
			ofShowCursor();
		}
		else{
			ofHideCursor();
		}
	}
}