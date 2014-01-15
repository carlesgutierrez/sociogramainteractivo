/*
 *  siManager.h
 Carles Gutierrez 28 Abril 2012
 */

#ifndef _siTrackingManager
#define _siTrackingManager

#include "ofMain.h"
#include "ofxBlobsSiManager.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "myGuiManager.h"

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
//otherwise, is using KINECT camera

#define NUMKINECTS 1 //3

class siTrackingManager
{
public:
	void setup(int w, int h);
	void update();
	void draw(int x, int y);
	void exit();
	
	void keyPressed(int key);

	
	
#ifdef _USE_LIVE_VIDEO
	ofVideoGrabber 		vidGrabber;
	
	ofxCvColorImage			colorImg;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	
	int 				threshold;
	bool				bLearnBakground;
	
	//GUI
	bool				lastbkchange;
	
#else
	
	//KINECTS Methods
	void updateKinects();
	void initKinects();
	void mergeKinects();
	void setupMergeImages(int numKinects,int width, int height);	
	

	//KINECTS VARS
	
	int gapKinect;
	
	//KINECT
	ofxKinect 			kinect3;
	ofxKinect 			kinect2;	
	ofxKinect 			kinect1;
	
	ofxCvGrayscaleImage 	grayImage1;
	ofxCvGrayscaleImage 	grayThresh1;
	ofxCvGrayscaleImage 	grayThreshFar1;	
	
	ofxCvGrayscaleImage 	grayImage2;
	ofxCvGrayscaleImage 	grayThresh2;
	ofxCvGrayscaleImage 	grayThreshFar2;
	
	ofxCvGrayscaleImage 	grayImage3;
	ofxCvGrayscaleImage 	grayThresh3;
	ofxCvGrayscaleImage 	grayThreshFar3;
	
	
	//Triple image computing
	bool bkinect1Pixels, bkinect2Pixels, bkinect3Pixels;
	ofPixels FBOpixels;
	ofFbo fboMerger;
	ofImage img1,img2,img3;
	ofxCvColorImage			resMergeColorFBO;
	ofxCvGrayscaleImage 	resMergeGrayFBO;
	
	bool				bThreshWithOpenCV;

	
	int 				nearThreshold;
	int					farThreshold;
	
	int					angle;
	int					lastangle;
	
#endif
	
	int	trackingWidth;
	int trackingHeight;
	
	int smallwinW;
	int smallwinH;
	
	float scaleWinFactorH;
	float scaleWinFactorW;
	
	int windowW;
	int windowH;
	
	ofxCvContourFinder 	contourFinder;
	ofxBlobsSiManager		blobsManager;
	
	//GUI
	void setupGUIParams();
	void updateGUIParams();

};

#endif

