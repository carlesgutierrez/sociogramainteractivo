/*
 *  siManager.h
 Carles Gutierrez 28 Abril 2012
 */

#ifndef _siManager
#define _siManager

#include "ofMain.h"
#include "siTrackingManager.h"
#include "siVisualizationManager.h"

class siManager
{
public:

	void setup(int w, int h);
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	
	siTrackingManager sitrackingmanager;
	siVisualizationManager sivisualizationmanager;

	bool bdrawtracking,bdrawvisualization;
};

#endif