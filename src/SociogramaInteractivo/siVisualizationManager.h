/*
 *  siVisualizationManager.h
 Carles Gutierrez 28 Abril 2012
 Concep and Visualization by Daniel Rojas
 */

#ifndef _siVisualizationManager
#define _siVisualizationManager

#include "ofMain.h"
#include "ofxStoredSiBlobVO.h"
#include "myGuiManager.h"
#include "ofxGLWarper.h"

class siVisualizationManager
{
public:
	void setup(int w, int h);
	void update(vector<ofxStoredSiBlobVO> & blobs);
	void draw(vector<ofxStoredSiBlobVO> & blobs, int basex, int basey);
	float factorGrosor;
	
private:

	void updateCollisions(vector<ofxStoredSiBlobVO> & blobs);
	void actualizaNodos(vector<ofxStoredSiBlobVO> & _blobs, int id1, int id2,int dist);
	
	float conectionlineWidth(float distan);
	void sidrawLine(ofVec2f posBlobi, ofVec2f posBlobn,float gros);
	void drawBlobsCircles(vector<ofxStoredSiBlobVO> & blobs, int baseX, int baseY, int rawW, int rawH, int winW, int winH, float circlewidth);
	void drawBlobsLines(vector<ofxStoredSiBlobVO> & blobs, int baseX, int baseY, int rawW, int rawH, int winW, int winH);
	
	void updateGUIparams();
	
	vector<int> setRelationsAllin(vector<ofxStoredSiBlobVO> & blobs, vector<int> allids);
	vector<int> getAllIdsAvaible(vector<ofxStoredSiBlobVO> _blobs);

	int searchArrayIdFromBlobId(vector<ofxStoredSiBlobVO> _blobs, int blobId);
	void removeBlobFromLostBlobFromAllPossiblePlaces(vector<ofxStoredSiBlobVO> & _blobs, int id);
	
	int searchArrayIdFromBlobIdReferences(vector<int> vectoridreferences, int blobIdReference);
	
	vector<ofxStoredSiBlobVO> lastsiblobs;

	bool bmodeAllin;
	
	int windowW;
	int windowH;
	int trackingWidth; 
	int trackingHeight;
	int smallwinW;
	int smallwinH;
	

	ofxGLWarper warper;
	bool bwarpingcorners;
	bool lastbwarpingcorners;
	bool bsavingcorners;
	bool lastbsavingcorners;
	
};

#endif

