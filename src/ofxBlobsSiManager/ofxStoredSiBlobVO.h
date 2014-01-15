/*
 *  ofxStoredSiBlobVO.h
 *
 *  Created by Peter Uithoven on 5/6/11.
 */

#ifndef _ofxStoredSiBlobVO
#define _ofxStoredSiBlobVO

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofxStoredSiBlobVO : public ofxCvBlob
{
public:
	int id;
	int lastDetectedTime; // the time when it was last detected
	int iniDetectedTime; // the time when it was first detected
	int dis; //used to sort on distance and to find the closest blob to merge with
	
	//Si Id references
	vector <int> indexIdReferences;
	bool bcollision;

	ofxStoredSiBlobVO(ofxCvBlob& newBlob);
	void update(ofxCvBlob& newBlob);
};

#endif
