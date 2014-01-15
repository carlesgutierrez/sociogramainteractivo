/*
 *  ofxStoredSiBlobVO.cpp
 *
 *  Created by Peter Uithoven on 5/6/11.


	SociogramaInteractivo adaptation to create Blob with id references to the others
 */

#import "ofxStoredSiBlobVO.h"


ofxStoredSiBlobVO::ofxStoredSiBlobVO(ofxCvBlob& newBlob)
{
	// a quick "shallow" copy 
	update(newBlob);
	
	id = -1;
	lastDetectedTime = 0;
	
	bcollision = false;
	
	indexIdReferences.reserve(9999); // TODO: MAX BLOBS EXPECTED : maxNumBlobs
}

void ofxStoredSiBlobVO::update(ofxCvBlob& newBlob)
{
	// a quick "shallow" update 
	area = newBlob.area;
	length = newBlob.length;
	boundingRect = newBlob.boundingRect;
	centroid = newBlob.centroid;
	hole = newBlob.hole;
	pts = newBlob.pts;
	nPts = newBlob.nPts;
	
}