/*
 *  BlobsManager.h
 *  A manager for openCV blobs. By remembering blobs and comparing them 
 *  it will identify blobs, even though they are moving. 
 *  It's basicly a lot of things you need to do when working with blobs. 
 *  
 *  Created by Peter Uithoven on 5/6/11.
 */
 
/*
 * Carles Gutierrez 2013, Modification to allow special blobs with Sociograma Interactivo information about
 */

#ifndef _ofxBlobsManager
#define _ofxBlobsManager

#include "ofMain.h"
#include "ofxStoredSiBlobVO.h"
#include "ofxOpenCv.h"

class ofxBlobsSiManager
{
public:

	//the closest stored blob that are withing this distance, will be merged into new found blob 
	int maxMergeDis; 
	// by what percentage the blob movement will be normalized
	float normalizePercentage;

	// enable a filter for blobs that show up shorter than minDetectedTime
	bool enableMinDetectedTimeFilter;
	// the minimum time a blob has to be detected before the blob manager will add the blob 
	// (last time detected - first time detection) 
	// not necessarily continuously (as long as undetected doesn't cross maxUndetectedTime)  
	int minDetectedTime;
	
	// enable the storage of blobs that disapeared shorter than maxUndetectedTime
	bool enableUndetectedBlobs;
	// the maximum time a blob can go undetected before the blob manager will remove the blob
	int maxUndetectedTime;
	
	// normally it will every new blob a sequentially higher id, 
	// but with giveLowestPossibleIDs enabled it will try to give the lowest id's available. 
	bool giveLowestPossibleIDs;
	// the max amount of blobs when doing non sequential id's
	int maxNumBlobs;
	
	// the final resulting blobs
	vector<ofxStoredSiBlobVO> blobs;

	// possible new blobs 
	vector<ofxStoredSiBlobVO> candidateBlobs;
	
	
	// also debug draw the candidate blobs
	bool debugDrawCandidates;
	
	ofxBlobsSiManager();
	void update(vector<ofxCvBlob>& newCVBlobs);
	void debugDraw(int baseX, int baseY, int inputWidth, int inputHeight, int displayWidth, int displayHeight);
	bool hasBlob(int blobID);
	void removeBlob(ofxStoredSiBlobVO& targetBlob, vector<ofxStoredSiBlobVO>& blobs);
	
private:
	int sequentialID;
	int sequentialCandidateID;
	
	vector<ofxStoredSiBlobVO*> findCloseBlobs(ofxCvBlob& newBlob,vector<ofxStoredSiBlobVO>& blobs);
};

#endif