/*
 *  siManager.cpp
 *
 *  Carles Gutierrez Software
 Sociograma Interactivo with Daniel Rojas
 */

#import "siManager.h"

//--------------------------------------------------------------
void siTrackingManager::setup(int w, int h)
{
	trackingWidth = w;
	trackingHeight = h;
	
	smallwinW = 320;
	smallwinH = 240;
	
	scaleWinFactorW = (float)smallwinW / (float)trackingWidth;
	scaleWinFactorH = (float)smallwinH / (float)trackingHeight;
	
	#ifdef _USE_LIVE_VIDEO
	
		vidGrabber.setVerbose(true);
		vidGrabber.initGrabber(trackingWidth,trackingHeight);
	
		colorImg.allocate(trackingWidth,trackingHeight);
		grayImage.allocate(trackingWidth,trackingHeight);
		grayBg.allocate(trackingWidth,trackingHeight);
		grayDiff.allocate(trackingWidth,trackingHeight);

	#else

	initKinects();
	cout << "Setup NUMKINECTS= " << NUMKINECTS << " kinect.width= " << kinect1.width << "kinect.height= " << kinect1.height << endl;	
	setupMergeImages(NUMKINECTS, kinect1.width, kinect1.height);

				
	#endif
	
	setupGUIParams();

}


//--------------------------------------------------------------
void siTrackingManager::initKinects()
{
	// zero the tilt on startup
	angle = 0;
	
	if(NUMKINECTS > 0)
	{
		kinect1.listDevices();

		kinect1.init();//(false, false);
		//kinect1.setVerbose(true);
		kinect1.open("A00367A11165033A");
		//kinect.init(true);  //shows infrared image
		

		
	
		grayImage1.allocate(kinect1.width, kinect1.height);
		grayThresh1.allocate(kinect1.width, kinect1.height);
		grayThreshFar1.allocate(kinect1.width, kinect1.height);
		
			bkinect1Pixels = false;
	}
	if(NUMKINECTS > 1)
	{

		kinect2.init();//(false, false);
		//kinect2.setVerbose(true);
		kinect2.open("A00363804971050A");
		//kinect.init(true);  //shows infrared image
		

		
	
		grayImage2.allocate(kinect2.width, kinect2.height);
		grayThresh2.allocate(kinect2.width, kinect2.height);
		grayThreshFar2.allocate(kinect2.width, kinect2.height);
		
			bkinect2Pixels = false;
	}
	else if(NUMKINECTS == 3)
	{
		
		kinect3.init();//(false, false);
		//kinect2.setVerbose(true);
		kinect3.open("A00367A11084033A");
		//kinect.init(true);  //shows infrared image
		
				
		grayImage3.allocate(kinect3.width, kinect3.height);
		grayThresh3.allocate(kinect3.width, kinect3.height);
		grayThreshFar3.allocate(kinect3.width, kinect3.height);
		
			bkinect3Pixels = false;
		
	}
	
}

//--------------------------------------------------------------
void siTrackingManager::setupMergeImages(int numKinects,int width, int height)
{		
	img1.allocate(width, height, OF_IMAGE_GRAYSCALE);
	img2.allocate(width, height, OF_IMAGE_GRAYSCALE);
	img3.allocate(width, height, OF_IMAGE_GRAYSCALE);
	
	fboMerger.allocate(width, numKinects*height, GL_RGB);
	resMergeGrayFBO.allocate(width, numKinects*height);
	resMergeColorFBO.allocate(width, numKinects*height);
	
	FBOpixels.allocate(width, numKinects*height, OF_IMAGE_COLOR);
	
	gapKinect = -5;
}


//--------------------------------------------------------------
void siTrackingManager::setupGUIParams(){

#ifdef _USE_LIVE_VIDEO
	//get background laser if Gui is check it
	bool mycheckboxguibk = myGuiManager::getInstance()->gui.getValueF("bLearnBakground");
	
	if(lastbkchange != mycheckboxguibk){ 
		cout << "bgetbackground true!" << endl;
		bLearnBakground = true;
		lastbkchange = mycheckboxguibk;
	}

	threshold = myGuiManager::getInstance()->gui.getValueF("threshold");//55
	
#else
	
	nearThreshold = myGuiManager::getInstance()->gui.getValueF("nearThreshold"); //230
	farThreshold = myGuiManager::getInstance()->gui.getValueF("farThreshold"); //70
	
	angle = myGuiManager::getInstance()->gui.getValueF("angletilt"); // 0
	
//	kinect1.setCameraTiltAngle(angle);
//	kinect2.setCameraTiltAngle(angle);
//	kinect3.setCameraTiltAngle(angle);
	
#endif
	
	blobsManager.normalizePercentage = myGuiManager::getInstance()->gui.getValueF("normalizePercentage");//0.7;
	blobsManager.giveLowestPossibleIDs = myGuiManager::getInstance()->gui.getValueB("giveLowestPossibleIDs");//true;
	blobsManager.maxUndetectedTime = myGuiManager::getInstance()->gui.getValueF("maxUndetectedTime");//500;
	blobsManager.minDetectedTime = myGuiManager::getInstance()->gui.getValueF("minDetectedTime");//2000;
	blobsManager.debugDrawCandidates = myGuiManager::getInstance()->gui.getValueB("debugDrawCandidates");//true;
	blobsManager.maxMergeDis = myGuiManager::getInstance()->gui.getValueF("maxMergeDis");//20
	
}

//--------------------------------------------------------------
void siTrackingManager::updateGUIParams(){
	
#ifdef _USE_LIVE_VIDEO
	//get background laser if Gui is check it
	bool mycheckboxguibk = myGuiManager::getInstance()->gui.getValueF("bLearnBakground");
	
	if(lastbkchange != mycheckboxguibk){ 
		cout << "bgetbackground true!" << endl;
		bLearnBakground = true;
		lastbkchange = mycheckboxguibk;
	}
	
	threshold = myGuiManager::getInstance()->gui.getValueF("threshold");//55
#else
	
	nearThreshold = myGuiManager::getInstance()->gui.getValueF("nearThreshold"); //230
	farThreshold = myGuiManager::getInstance()->gui.getValueF("farThreshold"); //70
	
	angle = myGuiManager::getInstance()->gui.getValueF("angletilt"); // 0
	
	if(lastangle != angle){		
	//	kinect1.setCameraTiltAngle(angle);
	//	kinect2.setCameraTiltAngle(angle);
	//	kinect3.setCameraTiltAngle(angle);
		lastangle = angle;
	}
	
#endif
	
	blobsManager.normalizePercentage = myGuiManager::getInstance()->gui.getValueF("normalizePercentage");//0.7;
	blobsManager.giveLowestPossibleIDs = myGuiManager::getInstance()->gui.getValueB("giveLowestPossibleIDs");//true;
	blobsManager.maxUndetectedTime = myGuiManager::getInstance()->gui.getValueF("maxUndetectedTime");//500;
	blobsManager.minDetectedTime = myGuiManager::getInstance()->gui.getValueF("minDetectedTime");//2000;
	blobsManager.debugDrawCandidates = myGuiManager::getInstance()->gui.getValueB("debugDrawCandidates");//true;
	blobsManager.maxMergeDis = myGuiManager::getInstance()->gui.getValueF("maxMergeDis");//20

}

//--------------------------------------------------------------
void siTrackingManager::update()
{
	windowW = ofGetWidth();
	windowH = ofGetHeight();
	
	bool bNewFrame = false;
	
	updateGUIParams();
	
#ifdef _USE_LIVE_VIDEO
	
	threshold = myGuiManager::getInstance()->gui.getValueF("camThreshold");
	
	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();
	
	if (bNewFrame){
		colorImg.setFromPixels(vidGrabber.getPixels(), trackingWidth,trackingHeight);
		
		grayImage = colorImg;
		
		if(myGuiManager::getInstance()->gui.getValueF("InvertHorizontal"))grayImage.mirror(false,true); // invert horizontally source image
		
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
		
		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		
		if(myGuiManager::getInstance()->gui.getValueF("InvertHorizontal"))grayDiff.mirror(false,true); // invert horizontally source image
		
		if(myGuiManager::getInstance()->gui.getValueF("blurHeavily"))grayDiff.blurHeavily(); // blurHeavily
		
		else if(myGuiManager::getInstance()->gui.getValueF("blurGaussian"))
		{
			grayDiff.dilate_3x3(); 
			grayDiff.blurGaussian(13); // blurHeavily
		}
		else if(myGuiManager::getInstance()->gui.getValueF("blur"))
		{
			grayDiff.dilate_3x3(); 
			grayDiff.blur(13); // blurHeavily
		}
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, ((trackingWidth+(trackingWidth*0.1))*trackingHeight)/3, 10, false);	// find holes
		blobsManager.update(contourFinder.blobs);
	}
#else
	
	updateGUIParams();
	updateKinects();
	
	//GUI SINGLETON CLASS
	myGuiManager::getInstance()->update();
	
#endif 
}


//--------------------------------------------------------------
void siTrackingManager::draw(int x, int y){
	
	float pos_draw_colorX = smallwinW * 0.1;
	float pos_draw_colorY = smallwinH * 0.1;
	
	float pos_draw_grayX = smallwinW + smallwinW * 0.1;
	float pos_draw_grayY = smallwinH * 0.1;

	float pos_draw_thersholdX = smallwinW * 0.1;
	float pos_draw_thersholdY = smallwinH + smallwinH * 0.1;

	float pos_draw_backgroundX = smallwinW + smallwinW * 0.1;
	float pos_draw_backgroundY = smallwinH + smallwinH * 0.1;
	
	
#ifdef _USE_LIVE_VIDEO
	
	ofPushStyle();
	
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(pos_draw_colorX,pos_draw_colorY, smallwinW, smallwinH);
	//grayImage.draw(trackingWidth*0.1+ trackingWidth,trackingHeight*0.1, trackingWidth, trackingHeight);
	grayBg.draw(pos_draw_backgroundX, pos_draw_backgroundY, smallwinW, smallwinH);
	grayDiff.draw(pos_draw_thersholdX, pos_draw_thersholdY, smallwinW, smallwinH);
	
	// then draw the contours:
	
	//ofFill();
	//ofSetHexColor(0x333333);
	//ofRect(trackingWidth*0.1+ trackingWidth,trackingHeight*2+trackingHeight*0.1,trackingWidth,trackingHeight);
	ofSetHexColor(0xffffff);
	
	// we could draw the whole contour finder
	//contourFinder.draw(360,540);
	
	// or, instead we can draw each blob individually,
	// this is how to get access to them:
	
	ofPushMatrix();

	ofTranslate(pos_draw_thersholdX,pos_draw_thersholdY,0);
	
	ofScale(scaleWinFactorW, scaleWinFactorH, 1);
	
	for (int i = 0; i < contourFinder.nBlobs; i++){
		contourFinder.blobs[i].draw(0,0);
	}
	
	ofPopMatrix();
	
	// finally, a report:
	
	ofSetHexColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
	
	ofPopStyle();
	
#else
	
	ofPushStyle();
	
	ofSetColor(255,255,255);

	
	//--------------------------------------------------------------
	//UPDATE and DRAW
	//Necesary Create the FBO before ContournFinder
	
	//TODO: This is not a sincronized kinect Frames
	//Try
	//if(bkinect1Pixels && bkinect2Pixels && bkinect3Pixels)
	//and then set to false all of them
	
	if(bkinect1Pixels || bkinect2Pixels || bkinect3Pixels)
	{
		
        img1.mirror(true,true);
        img2.mirror(true,true);
        img3.mirror(true,true);
		
        fboMerger.begin();
		glClear(GL_COLOR_BUFFER_BIT);
		img1.draw(0,0,                        img1.width, img1.height);
		img2.draw(0,img2.height+gapKinect,    img2.width, img2.height);
		img3.draw(0,img3.height*2+gapKinect*2,img3.width, img3.height);
        fboMerger.end();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(resMergeGrayFBO, (int)myGuiManager::getInstance()->gui.getValueF("minSizeFilter"), (fboMerger.getWidth()*fboMerger.getHeight())/2, 100, false);
		//update blob contours
		blobsManager.update(contourFinder.blobs);
		
		fboMerger.readToPixels(FBOpixels);
		//resMergeColorTriple.setFromPixels(FBOpixels.getPixels(), fboMerger.getWidth(), fboMerger.getHeight());
		//resMergeGrayTriple  = resMergeColorTriple;
		resMergeColorFBO.setFromPixels(FBOpixels.getPixels(), FBOpixels.getWidth(), FBOpixels.getHeight());
		//GRAY
		resMergeGrayFBO  = resMergeColorFBO;
		
		//BLURY
		if(myGuiManager::getInstance()->gui.getValueF("blurHeavily"))
		{
			resMergeGrayFBO.blurHeavily();
		}
		

		//--------------------------------------------------------------
		//DRAW
		float scaleDrawfactor = 0.5;
		ofSetColor(255,255,255);
		
        kinect1.drawDepth    (0*scaleDrawfactor,        0*scaleDrawfactor,                    400*scaleDrawfactor, 300*scaleDrawfactor);
        kinect2.drawDepth    (0*scaleDrawfactor,        300*scaleDrawfactor+gapKinect,        400*scaleDrawfactor, 300*scaleDrawfactor);
        kinect3.drawDepth    (0*scaleDrawfactor,        600*scaleDrawfactor+(gapKinect*2),    400*scaleDrawfactor, 300*scaleDrawfactor);
		
        //TODO: COLOR OFF TO ALLOW BETTER PERFORMANCE
        kinect1.draw        (300*scaleDrawfactor,        0,                                400*scaleDrawfactor, 300*scaleDrawfactor);
        kinect2.draw        (300*scaleDrawfactor,        300*scaleDrawfactor+gapKinect,    400*scaleDrawfactor, 300*scaleDrawfactor);
        kinect3.draw        (300*scaleDrawfactor,        600*scaleDrawfactor+gapKinect*2,400*scaleDrawfactor, 300*scaleDrawfactor);
		
        img1.draw            (600*scaleDrawfactor,        0                             ,  400*scaleDrawfactor, 300*scaleDrawfactor);
        img2.draw            (600*scaleDrawfactor,        300*scaleDrawfactor+gapKinect,  400*scaleDrawfactor, 300*scaleDrawfactor);
        img3.draw            (600*scaleDrawfactor,        600*scaleDrawfactor+gapKinect*2,400*scaleDrawfactor, 300*scaleDrawfactor);
		
		fboMerger.draw(900*scaleDrawfactor,    0, fboMerger.getWidth(), fboMerger.getHeight());
		
		
		//CHECK WHERE IS DRAWING
		
		ofSetColor(0,0,255);
        ofNoFill();
        ofRect(900*scaleDrawfactor,    0,fboMerger.getWidth(), fboMerger.getHeight());
		
        ofSetColor(255,0,0);
        ofFill;
		
		//CONTOURS
		
        //blob tracking
        contourFinder.draw    (900*scaleDrawfactor,    0,fboMerger.getWidth(), fboMerger.getHeight());
        ofSetColor(0,255,0);
        //multi blob tracking

        //BLOBTRACKING
		
		blobsManager.debugDraw(900*scaleDrawfactor,    0, fboMerger.getWidth(), fboMerger.getHeight(), fboMerger.getWidth(), fboMerger.getHeight());
		
	}
	
	ofPopStyle();
	
#endif

		ofSetColor(255, 255, 255);
		myGuiManager::getInstance()->draw();
	
}




//--------------------------------------------------------------
void siTrackingManager::keyPressed(int key){
	switch (key){
#ifdef _USE_LIVE_VIDEO	
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case 's':
			vidGrabber.videoSettings();
#else
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case OF_KEY_RIGHT:
			gapKinect++;
			break;
		case OF_KEY_LEFT:
			gapKinect--;
			break;
		/*	
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
		*/
#endif
			
			break;
	}
}


	
//--------------------------------------------------------------
void siTrackingManager::updateKinects()
{
	kinect1.update();
	if(kinect1.isFrameNew())	// there is a new frame and we are connected
	{
		grayImage1.setFromPixels(kinect1.getDepthPixels(), kinect1.width, kinect1.height);
		grayThreshFar1 = grayImage1;
		grayThresh1 = grayImage1;
		grayThresh1.threshold(nearThreshold, true);
		grayThreshFar1.threshold(farThreshold);
		cvAnd(grayThresh1.getCvImage(), grayThreshFar1.getCvImage(), grayImage1.getCvImage(), NULL);
		img1.setFromPixels(grayImage1.getPixels(),grayImage1.width, grayImage1.height, OF_IMAGE_GRAYSCALE);
		
		bkinect1Pixels = true;
		
	}

	
	kinect2.update();
	if(kinect2.isFrameNew())	// there is a new frame and we are connected
	{
		grayImage2.setFromPixels(kinect2.getDepthPixels(), kinect2.width, kinect2.height);
		grayThreshFar2 = grayImage2;
		grayThresh2 = grayImage2;
		grayThresh2.threshold(nearThreshold, true);
		grayThreshFar2.threshold(farThreshold);
		cvAnd(grayThresh2.getCvImage(), grayThreshFar2.getCvImage(), grayImage2.getCvImage(), NULL);
		img2.setFromPixels(grayImage2.getPixels(),grayImage2.width, grayImage2.height, OF_IMAGE_GRAYSCALE);
		
		bkinect2Pixels = true;
	}
	
	kinect3.update();
	if(kinect3.isFrameNew())	// there is a new frame and we are connected
	{
		grayImage3.setFromPixels(kinect3.getDepthPixels(), kinect3.width, kinect3.height);
		grayThreshFar3 = grayImage3;
		grayThresh3 = grayImage3;
		grayThresh3.threshold(nearThreshold, true);
		grayThreshFar3.threshold(farThreshold);
		cvAnd(grayThresh3.getCvImage(), grayThreshFar3.getCvImage(), grayImage3.getCvImage(), NULL);
		img3.setFromPixels(grayImage3.getPixels(),grayImage3.width, grayImage3.height, OF_IMAGE_GRAYSCALE);
		
		bkinect3Pixels = true;
	}	
	
}

//--------------------------------------------------------------
void siTrackingManager::exit(){
		
#ifdef _USE_LIVE_VIDEO
		
#else
	//kinect1.setCameraTiltAngle(0); // zero the tilt on exit
	kinect1.close();
	
	//kinect2.setCameraTiltAngle(0); // zero the tilt on exit
	kinect2.close();
	
	//kinect3.setCameraTiltAngle(0); // zero the tilt on exit
	kinect3.close();
#endif

}
