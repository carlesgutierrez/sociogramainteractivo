#ifndef _UTILS_H
#define _UTILS_H

#include "ofMain.h"

//mode type to connect
enum MyEnumType { binarymode, hexamode };

//FADE UTILS
float  fadeTo(float value, float maxValue, float  aFadeVel);
float  fadeIn(float value, float maxValue, float  aFadeVel);
float  fadeOut(float value, float minValue, float  aFadeVel);

//MATH UTILS
ofVec2f centroidAverage(vector <ofVec2f>  points);
float cos_deg( float A );
float sin_deg( float A );

//DATE UTILS
string getFullDateSec();
string getFullDateDay();

//FILE UTILS
char* doCleanAbsolutePath(char* path, string parsername, string newPath);
string ofToAbsoluteDataPath(string path);
long ofFileSize(string path);

//HEXA UTILS
uint32_t hexaToUint32_t(string hexadata);
int32_t hexaToInt32_t(string hexadata);
uint16_t hexaToUint16_t(string hexadata);
float hexaToFloat(string hexadata);


#endif

