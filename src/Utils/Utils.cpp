#include  "Utils.h"

//FADE UTILS
//--------------------------------------------------------------

float  fadeTo(float aSrcValue, float aDestValue, float  aFadeVel)
{
	return  (aDestValue - aSrcValue  > 0)? fadeIn(aSrcValue, aDestValue, aFadeVel) : fadeOut(aSrcValue, aDestValue, aFadeVel);
}

//--------------------------------------------------------------

float  fadeIn(float value, float maxValue, float  aFadeVel)
{
	float Error = fabsf(value - maxValue)/maxValue;
    //if( fabsf(value - maxValue) >= 0.025f)
	if( Error >= 0.01f)
    {
        value += (maxValue - value )*aFadeVel;
    }
	else
	{
		value = maxValue;
	}
    return value;
}

//--------------------------------------------------------------

float fadeOut(float value, float minValue, float  aFadeVel)
{
    if( fabsf(value - minValue) >= 0.000125f)
    {
        value -= (value -minValue)*aFadeVel;
    }
	else
	{
		value = minValue;
	}
    return value;
}

//MATH UTILS

// Average (centroid) among points.
// Addition is sometimes useful for calculating averages too.
// Changed from OF to do the same with a vector
//
ofVec2f centroidAverage(vector <ofVec2f>  points) {
	
	float x = 0.f;
	float y = 0.f;
	for( int i=0; i<points.size(); i++) {
		x += points[i].x;
		y += points[i].y;
	}
	x /= points.size();
	y /= points.size();
	return ofVec2f(x,y);
}


//--------------------------------------------------------------

float cos_deg( float A )
{
      return cos( A / 180.0 * PI );
}

//--------------------------------------------------------------

float sin_deg( float A )
{
      return sin( A / 180.0 * PI );
}

//DATE UTILS
//--------------------------------------------------------------

string getFullDateSec()
{
	string dia;
	if (ofGetDay() < 10)dia = "0"+ofToString(ofGetDay());
	else dia = ofToString(ofGetDay());
	
	string month;
	if (ofGetMonth() < 10)month = "0"+ofToString(ofGetMonth());
	else month = ofToString(ofGetMonth());
	
	string segundos;
	if (ofGetSeconds() < 10)segundos = "0"+ofToString(ofGetSeconds());
	else segundos = ofToString(ofGetSeconds());
	
	string minutos;
	if (ofGetMinutes() < 10)minutos = "0"+ofToString(ofGetMinutes());
	else minutos = ofToString(ofGetMinutes());
	
	string horas;
	if (ofGetHours() < 10)horas = "0"+ofToString(ofGetHours());
	else horas = ofToString(ofGetHours());
	
	
	string dateSec = ofToString(ofGetYear())+"_"+month+"_"+dia+"_"+horas+"_"+minutos+"_"+segundos;
	
	return dateSec;
}

//--------------------------------------------------------------

string getFullDateDay()
{
	string dia;
	if (ofGetDay() < 10)dia = "0"+ofToString(ofGetDay());
	else dia = ofToString(ofGetDay());
	
	string month;
	if (ofGetMonth() < 10)month = "0"+ofToString(ofGetMonth());
	else month = ofToString(ofGetMonth());
	
	string dateDay = ofToString(ofGetYear())+"_"+month+"_"+dia;
	
	return dateDay;
}


//FILE UTILS
//--------------------------------------------------------------

string ofToAbsoluteDataPath(string path)
{
	string fullPath = ofToDataPath(path, true);
	path = fullPath.substr( 0, fullPath.find("bin")+3) + "/data/"+path;
	cout << "new abosulte Bad:" << path << endl;
	return path;
}


//--------------------------------------------------------------
char* doCleanAbsolutePath(char* path, string parsername, string newPath)
{
	char newpathfixed[1024]		= "";
	string binname				= parsername;
	
	char * cstr_path2fix,  * p;
	cstr_path2fix				= "";
	p							= "";
	int charsdone				= 0;
	int totalcharsdone			= 0;
	
	cstr_path2fix				= path;
	
	//Start the parser
	p=strtok (cstr_path2fix,"/");
	
	while (p!=NULL)
	{
		cout << p << endl;
		
		if(binname.compare(p) == 0)
		{
			strcat (newpathfixed , "/");
			strcat (newpathfixed , newPath.c_str());
			
			int lenghNewPath = newPath.length()+1;
			
			newpathfixed[totalcharsdone+lenghNewPath]=0;
			
			break;
			
		}else {
			charsdone = strlen(p);
			totalcharsdone += charsdone + 1;
			cout << "totalcharsdone=" << totalcharsdone;
			
			strcat (newpathfixed , "/");
			strcat (newpathfixed , p);
			
		}
		
		p=strtok(NULL,"/");
		
	}
	
	cout << "Result newpathfixed = " << newpathfixed << endl;
	
	delete cstr_path2fix;
	delete p;
	
	return newpathfixed;
}

//--------------------------------------------------------------

long ofFileSize(string path)
{
	FILE * pFile;
	long size = 0;
	path = ofToAbsoluteDataPath("./") + path;
	pFile = fopen (path.c_str(),"rb");
	if (pFile==NULL){ 
		perror ("Error opening file");
	}else{
		fseek (pFile, 0, SEEK_END);
		size = ftell (pFile);
		fclose (pFile);
		printf ("Size of myfile.txt: %ld bytes.\n",size);
	}
	return size;
}

//HEXA UTILS
//--------------------------------------------------------------
uint32_t hexaToUint32_t(string hexadata){
	
	std::string s("0x"+hexadata);
	uint32_t v;
	std::istringstream iss(s);
	iss >> std::setbase(0) >> v;
	return v;
	
}

//--------------------------------------------------------------
int32_t hexaToInt32_t(string hexadata){
	
	std::string s("0x"+hexadata);
	int32_t v;
	std::istringstream iss(s);
	iss >> std::setbase(0) >> v;
	return v;
	
}
//--------------------------------------------------------------
uint16_t hexaToUint16_t(string hexadata){
	
	std::string s("0x"+hexadata);
	uint16_t v;
	std::istringstream iss(s);
	iss >> std::setbase(0) >> v;
	return v;
	
}

//--------------------------------------------------------------
float hexaToFloat(string value) {
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << "0x"+value;
	ss >> x;
	return reinterpret_cast<float&>(x);
}
