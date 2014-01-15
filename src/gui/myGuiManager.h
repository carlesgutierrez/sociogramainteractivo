/*
 *  myGuiManager.cpp
 */


#ifndef _MANAGER_GUI
#define _MANAGER_GUI

#include "ofxControlPanel.h"


class myGuiManager
{
public:	
	myGuiManager();
	~myGuiManager();
	
	// variables & methods for singleton
private:
    static bool	instanceFlag;
    static myGuiManager *single;	
public: 
	static myGuiManager* getInstance();
	// end singleton
	
	// Methods
	void setup();
	void update();
	void draw();
	
	//gui ADD methods
	void addPanelGeneralSettings();
	
	ofxControlPanel	gui;
	
	bool bmouse;

private:

	
	// GUI 
	void guiEvents(guiCallbackData & data);
	void mouseDragged(ofMouseEventArgs &e);
	void mousePressed(ofMouseEventArgs &e);
	void mouseReleased(ofMouseEventArgs &e);
	void mouseMove(ofMouseEventArgs &e);
	void keyReleased(ofKeyEventArgs &e);
	
};

#endif