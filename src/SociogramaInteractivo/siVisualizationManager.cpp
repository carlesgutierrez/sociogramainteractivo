/*
 *  siVisualizationManager.cpp
 *
 *  Carles Gutierrez Software
	Sociograma Interactivo with Daniel Rojas
 */

#import "siVisualizationManager.h"

//--------------------------------------------------------------
void siVisualizationManager::setup(int w, int h)
{
	factorGrosor = 500;
	
	trackingWidth = w;
	trackingHeight = h;
	
	smallwinW = 320;
	smallwinH = 240;
	
	/*
	float pos_draw_colorX = smallwinW * 0.1;
	float pos_draw_colorY = smallwinH * 0.1;
	
	float pos_draw_grayX = smallwinW + smallwinW * 0.1;
	float pos_draw_grayY = smallwinH * 0.1;
	
	float pos_draw_thersholdX = smallwinW * 0.1;
	float pos_draw_thersholdY = smallwinH + smallwinH * 0.1;
	
	float pos_draw_backgroundX = smallwinW + smallwinW * 0.1;
	float pos_draw_backgroundY = smallwinH + smallwinH * 0.1;
	 */
	
	warper.setup(); //initializates ofxGLWarper
	warper.activate();// this allows ofxGLWarper to automatically listen to the mouse and keyboard input and updates automatically it's matrixes.
	bwarpingcorners = false;
	lastbwarpingcorners = false;
	
	warper.load("warpConfig.xml");	
	bsavingcorners = false;
	lastbsavingcorners = bsavingcorners;
	
}

//--------------------------------------------------------------
void siVisualizationManager::update(vector<ofxStoredSiBlobVO> & blobs)
{
	windowW = ofGetWidth();
	windowH = ofGetHeight();
	
	//get all ids for set mode allin
	vector<int> allidsvector = getAllIdsAvaible(blobs);
	
	if(!myGuiManager::getInstance()->gui.getValueF("modocontacto")){
		setRelationsAllin(blobs, allidsvector);
	}
	updateCollisions(blobs);

	updateGUIparams();	
}

//--------------------------------------------------------------
void siVisualizationManager::updateGUIparams(){
	
	factorGrosor = myGuiManager::getInstance()->gui.getValueF("factorgrosor");
	
	bwarpingcorners = myGuiManager::getInstance()->gui.getValueF("warpingcorners");
	
	if(bwarpingcorners != lastbwarpingcorners )
	{
		if (warper.isActive()) {
			warper.deactivate(); //once you are done with the Wrapping you should call this method, so it realeses the keyboard and mouse and stop processing the transformation matrixes.
			// this will reduce the amount of precessing needed.
		}else {
			warper.activate();
		}
		
		lastbwarpingcorners = bwarpingcorners;
	}
	
	bsavingcorners = myGuiManager::getInstance()->gui.getValueF("savewarpingcorners");
											   
	if(bsavingcorners != lastbsavingcorners){
		warper.save("warpConfig.xml");
		lastbsavingcorners = bsavingcorners;
	}
	
}	

//--------------------------------------------------------------
void siVisualizationManager::draw(vector<ofxStoredSiBlobVO> & blobs, int baseX, int baseY){	
	
	if(myGuiManager::getInstance()->gui.getValueF("rendervisualizationdraw")){	
		
		warper.begin();	///all the things that are drawn AFTER begin and BEFORE end ofxGLWarper's methods are afected by ofxGLWarper.
		
		if(myGuiManager::getInstance()->gui.getValueF("renderWarpRect")){
			ofPushStyle();
			ofNoFill();
			ofSetColor(255,255,255);
				ofRect(0,0,windowW,windowH);
			ofPopStyle();
		}
		
		drawBlobsCircles(blobs, baseX, baseY, trackingWidth, trackingHeight, windowW, windowH, myGuiManager::getInstance()->gui.getValueF("circleWidth"));
		drawBlobsLines(blobs, baseX, baseY, trackingWidth, trackingHeight, windowW, windowH);	
		
		warper.end();
	}
}


//--------------------------------------------------------------
void siVisualizationManager::drawBlobsCircles(vector<ofxStoredSiBlobVO> & blobs, int baseX, int baseY, int _trackingWidth, int _trackingHeight, int winW, int winH, float circlewidth)
{
	ofPushStyle();
	
	ofPushMatrix();
	ofTranslate(baseX, baseY, 0);
	
	//ofSetColor(color.r,color.g,color.b,color.a);
	ofSetLineWidth(circlewidth);
	ofSetCircleResolution(myGuiManager::getInstance()->gui.getValueF("circleresolution")); // 100
	
	for( int i = 0; i < blobs.size(); i++ ) 
	{
		
		//DRAW BLOB		
		float x = (blobs[i].centroid.x/_trackingWidth)*winW;
		float y = (blobs[i].centroid.y/_trackingHeight)*winH;
		
		//CIRCLE VISUALIZATION
		ofNoFill();
		
		if(blobs[i].bcollision == true) ofSetColor(255,0,255);
		else ofSetColor(255,255,255);

		ofCircle(x, y, myGuiManager::getInstance()->gui.getValueF("circlesize")); //30
		
		//DRAW INFO
		//ofSetColor(20,20,20);
		//if(blobs[i].id >= 10) x -= 4;
		//ofDrawBitmapString(ofToString(blobs[i].id),x-4,y+5);
		
	}
	
	ofPopMatrix();
	
		ofPopStyle();
}

void siVisualizationManager::drawBlobsLines(vector<ofxStoredSiBlobVO> & blobs, int baseX, int baseY, int _trackingWidth, int _trackingHeight, int winW, int winH){
	
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(baseX, baseY, 0);
	
	
	//DRAW REALTIONS
	//LINES
	for(int i=0;i< blobs.size();i++){
		
		//Find Blob1 pos
		ofVec2f posBlobi = ofVec2f((blobs[i].centroid.x/_trackingWidth)*winW, (blobs[i].centroid.y/_trackingHeight)*winH);;
		//cout << "Blob I[" << i << "] centroidX= " <<  posBlobi.x << " centroidY= " << posBlobi.y << endl;
		//cout << "blobs[i].indexIdReferences.size() = " << blobs[i].indexIdReferences.size() << endl;
		
		for(int j=0;j< blobs[i].indexIdReferences.size();j++)
		{
			
			int idblob = searchArrayIdFromBlobId(blobs, blobs[i].indexIdReferences[j]);
			//cout << "i= " << i << " j= " << j << endl;
			//cout << "blobs[i].indexIdReferences[j] = " << blobs[i].indexIdReferences[j] << endl;
			//cout << "idblob = " << idblob << endl;
			
			//Find BlobN pos
			ofVec2f posBlobn = ofVec2f((blobs[idblob].centroid.x/_trackingWidth)*winW, (blobs[idblob].centroid.y/_trackingHeight)*winH);
			//cout << "Blob N[" << idblob << "] centroidX= " <<  posBlobn.x << " centroidY= " << posBlobn.y << endl;
			
			float dist_b1b2 = ofDist((posBlobi.x*trackingWidth)/windowW, (posBlobi.y*trackingHeight)/windowH, (posBlobn.x*trackingWidth)/windowW, (posBlobn.y*trackingHeight)/windowH);
			
			float linewidht = conectionlineWidth(dist_b1b2);
			
			sidrawLine(posBlobi,posBlobn, linewidht);
			
		}
		
	}
	
	ofPopMatrix();
	ofPopStyle();
}


//--------------------------------------------------------------
vector<int> siVisualizationManager::setRelationsAllin(vector<ofxStoredSiBlobVO> & blobs, vector<int> allids)
{
	
	//cout << "SetRelationsAll in, blobs size " << blobs.size() << endl;
	
	for(int i=0;i< blobs.size();i++)
	{
		//cout << "allids.size() =" << allids.size() << endl;

		//TODO: THIS MAKES BREAK FLOW 
		//cout << "blobs[i].indexIdReferences.size = " << blobs[i].indexIdReferences.size() << endl;
		//Clear 
		blobs[i].indexIdReferences.clear();
		
		for(int j=0;j<allids.size();j++)
		{
			//cout << "j = " << j << endl;
			//if(allids[j] != blobs.id)
			//{
			blobs[i].indexIdReferences.push_back(allids[j]);
			//cout << "blobs[" << i << "] allids[" << j << "]= " << allids[j] << endl;
			//}
		}
	}
}


//--------------------------------------------------------------
vector<int> siVisualizationManager::getAllIdsAvaible(vector<ofxStoredSiBlobVO> _blobs){
	
	vector<int> allids;
	allids.resize(_blobs.size());
	
	for(int i=0;i<_blobs.size();i++){
		allids[i] = _blobs[i].id;
	}
	
	return allids;
}


//--------------------------------------------------------------
void siVisualizationManager::removeBlobFromLostBlobFromAllPossiblePlaces(vector<ofxStoredSiBlobVO> & _blobs, int id){
	
	for(int i = 0 ; i < _blobs.size(); i++)
	{
		for(int j=0; j< _blobs[i].indexIdReferences.size(); j++)
		{
			//if ID is localized at the indexIdReferences then removed from this vector!
			if(id == _blobs[i].indexIdReferences[j])
			{
				std::vector<int>::iterator iterBlob = _blobs[i].indexIdReferences.begin();
				std::vector<int>::iterator iditerRemovedblob = _blobs[i].indexIdReferences.erase(iterBlob + j);
			}
		}
	}
}


//--------------------------------------------------------------
void siVisualizationManager::updateCollisions(vector<ofxStoredSiBlobVO> & _blobs){
	
	
	//remove index references of the lost blobs from all existence blobs
	if(myGuiManager::getInstance()->gui.getValueF("modocontacto")){
		for(int j = 0 ; j < lastsiblobs.size(); j++)
		{

				int res = searchArrayIdFromBlobId(_blobs, lastsiblobs[j].id);
				if(res == -1){
					removeBlobFromLostBlobFromAllPossiblePlaces(_blobs,lastsiblobs[j].id);
				}

		}
	}
	 
	
	
	//clear collision status
	for(int j = 0 ; j < _blobs.size(); j++)
	{
		//Collision
		_blobs[j].bcollision = false;
	}
	
	if(_blobs.size() > 1)
	{
		// printf("nB%d\n",totalPersonas);
		for(int i =0 ; i < _blobs.size()-1; i++)
		{
			for(int j = i+1 ; j < _blobs.size(); j++)
			{
				float dist = ofDist((_blobs[i].centroid.x/trackingWidth)*windowW, (_blobs[i].centroid.y/trackingHeight)*windowH, (_blobs[j].centroid.x/trackingWidth)*windowW, (_blobs[j].centroid.y/trackingHeight)*windowH);
				
				actualizaNodos(_blobs, i, j, dist);
			}
		}
		
	}
	
	//save last results
	lastsiblobs = _blobs;
}

//--------------------------------------------------------------
int siVisualizationManager::searchArrayIdFromBlobId(vector<ofxStoredSiBlobVO> _blobs, int blobId)
{
	int arrayId = -1;
	
	for (int i=0; i< _blobs.size(); i++)
	{
		if(_blobs.at(i).id == blobId){
			arrayId = i;
		}
	}
	
	return arrayId;
	
}

//--------------------------------------------------------------
int siVisualizationManager::searchArrayIdFromBlobIdReferences(vector<int> vectoridreferences, int blobIdReference)
{
	int arrayId = -1;
	
	for (int i=0; i< vectoridreferences.size(); i++)
	{
		if(vectoridreferences.at(i) == blobIdReference){
			arrayId = i;
		}
	}
	
	return arrayId;
	
}

//--------------------------------------------------------------
void siVisualizationManager::sidrawLine(ofVec2f posBlobi, ofVec2f posBlobn,float gros)
{
    //vector blobs = gamuza.contourFinder.blobs;
	
    ofPushStyle();
	ofSetColor(255,255,255,200); // TODO: but we are not using alphas
	
	//cout << "Grosor: " << gros << endl;
	ofSetLineWidth(gros);//*gros*gros

	//Draw a line between blobi and blobN
	ofLine(posBlobi,posBlobn);
	
	//printf("pintado\n");
    ofPopStyle();
	
}

//--------------------------------------------------------------
float siVisualizationManager::conectionlineWidth(float distan)
{
    float grosor = (factorGrosor*factorGrosor)*1/(distan*distan+1);
	//cout << "factorGrosor*factorGrosor =" << factorGrosor*factorGrosor << endl;
	//cout << "distan*distan =" << distan*distan << endl;
	//cout << "grosor =" << grosor << endl;
	
	return grosor;
}

//--------------------------------------------------------------
void siVisualizationManager::actualizaNodos(vector<ofxStoredSiBlobVO> & _blobs, int id1, int id2,int dist)
{
	//cout << "BLob1=" << _blobs[id1].id << endl;
	//cout << "BLob2=" << _blobs[id2].id << endl;
	
	//cout << "dist=" << dist << endl;
	//cout << "circlesize*2=" << (myGuiManager::getInstance()->gui.getValueF("circlesize")) << endl;
	
	
	if(dist < (myGuiManager::getInstance()->gui.getValueF("circlesize"))*2)
	{
		//printf("rosa\n");
		
		_blobs[id1].bcollision = true;
		_blobs[id2].bcollision = true;
		
		if(myGuiManager::getInstance()->gui.getValueF("modocontacto"))
		{
			//cout << "this should add a new node" << endl;
			//cout << "_blobs[" <<id1 << "].id =" << _blobs[id1].id << endl;
			//cout << "_blobs[" <<id2 << "].id =" << _blobs[id2].id << endl;
			
			int okid1 = searchArrayIdFromBlobIdReferences(_blobs[id1].indexIdReferences, _blobs[id2].id);
			int okid2 = searchArrayIdFromBlobIdReferences(_blobs[id2].indexIdReferences, _blobs[id1].id);
			
			//Checking if such nodes where in contact before if not then add id to the vector
			if(okid1 == -1)_blobs[id1].indexIdReferences.push_back(_blobs[id2].id);
			if(okid2 == -1)_blobs[id2].indexIdReferences.push_back(_blobs[id1].id);
		}
	}
	else
	{
		
			//Update circle collision property for draw right color
			//printf("blanco\n");
			
			//cout << "_blobs[" << id1 << "] = " << _blobs[id1].bcollision << endl;
			//cout << "_blobs[" << id2 << "] = " << _blobs[id2].bcollision << endl;
			
			if(_blobs[id1].bcollision == true)_blobs[id1].bcollision = true;
			else  _blobs[id1].bcollision = false;
			if(_blobs[id2].bcollision == true)_blobs[id2].bcollision = true;
			else  _blobs[id2].bcollision = false;
		
	}
	

	
}
