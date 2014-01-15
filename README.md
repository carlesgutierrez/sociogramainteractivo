#sociogramainteractivo#
![sociogramainteractivo](http://media.tumblr.com/tumblr_m3ux06VDdL1qa7rwa.jpg)
===================== 

Interactive installation SociogramaInteractivo

A project of Daniel Rojas (original design idea) & Carles Gutierrez. 
Developed with OpenFrameWorks (c++) by Carles Gutierrez.

=====================
Project Info: http://sociogramainteractivo.net/

SociogramaInteractivo bassically is designed to represent Sociograma, in real time, of the people tracked from ceiling in a specific area. Over them, projecting on the floor: pink circles and lines with their pattern physic relations appears.

By this release the project is being designed to be used in the art field or others for a interactive installation that allow to use the relational feedbacks of Sociogramas.

=====================
Install

Compiled in OSX 

This project use several addons and is compiled with OF 008

Addons:

ofxOpenCv
ofxKinect
ofxControlPanel
ofxGLWarper
ofxXmlSettings
ofxBlobsManager
ofxPatterns

To install external addons execute script: clone_addons.sh

=====================
Tecnical Info : http://sociogramainteractivo.net/post/239368570/tecnica

SI project allow to use diferents camera modes: 

Webcam camera
Kinect camera ( 1, 2, or 3)

Check in siTrackingManager.h : 

define NUMKINECTS 1 //set until 3 cameras maximum

or

//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
//otherwise, is using KINECT camera

=====================


