/*
 *  siManager.cpp
 *
 *  Carles Gutierrez Software
 Sociograma Interactivo with Daniel Rojas
 */

#import "siManager.h"

//--------------------------------------------------------------
void siManager::setup(int w, int h)
{
	sitrackingmanager.setup(w,h);
	sivisualizationmanager.setup(w,h);
	
	bdrawtracking = true;
	bdrawvisualization = true;
}

//--------------------------------------------------------------
void siManager::update()
{
	sitrackingmanager.update();
	sivisualizationmanager.update(sitrackingmanager.blobsManager.blobs);
	
	bdrawtracking = myGuiManager::getInstance()->gui.getValueF("rendertrackingdraw");
}

//--------------------------------------------------------------
void siManager::draw()
{
	if(bdrawtracking)sitrackingmanager.draw(0,0);
	if(bdrawvisualization)sivisualizationmanager.draw(sitrackingmanager.blobsManager.blobs, 0, 0);
}

//--------------------------------------------------------------
void siManager::keyPressed(int key)
{
	sitrackingmanager.keyPressed(key);
	
	/*
	if (key == 't') {
		bdrawtracking = !bdrawtracking;
	}
	if (key == 'v') {
		bdrawvisualization = !bdrawvisualization;
	}*/
}

//--------------------------------------------------------------
void siManager::exit(){
	sitrackingmanager.exit();
}
