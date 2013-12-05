//
//  ofxSpatDIF.cpp
//
//  Created by jasch on 27/9/2013.
//

#include "ofxSpatDIF.h"

#include <iostream>
#include <fstream>

ofxSpatDIF::ofxSpatDIF()
{
	
}

ofxSpatDIF::~ofxSpatDIF()
{

}

void ofxSpatDIF::loadFile(char * buffer)
{
}

void ofxSpatDIF::loadSceneFromFile(string filePath, int type)
{
    if(type == 0) { //xml file
        
        ifstream ifsXML(filePath.c_str());
        string xmlString;
        
        // load entire file onto a string
        if (ifsXML.is_open())
        {
            while ( ifsXML.good() )
            {
                string str;
                getline(ifsXML,str);
                xmlString.append(str);
            }
            ifsXML.close();
            // give sdLoader the XMLstring and get a sdScene
            myScene = sdLoader::sceneFromXML(xmlString);
//            myScene.dump();
        }
        
    }else if(type == 1) { //json file
        
        ifstream ifsJSON(filePath.c_str());
        string jsonString;
    
        if (ifsJSON.is_open())
        {
            while ( ifsJSON.good() )
            {
                string str;
                getline(ifsJSON,str);
                jsonString.append(str);
            }
            ifsJSON.close();
            // give sdLoader the JSONstring and get a sdScene, The result should be identical
            myScene = sdLoader::sceneFromJSON(jsonString);
//            myScene.dump();
        }
    }
    currentFilePath = filePath;
}

unsigned int ofxSpatDIF::getNumberOfEntities(sdScene theScene)
{
    return theScene.getNumberOfEntities();
}

string ofxSpatDIF::getEntityName(sdScene * theScene, int ID)
{
    int counter = 0;
    vector <sdEntityCore*> myEntityVector = theScene->getEntityVector();
    vector <sdEntityCore*>::iterator it = myEntityVector.begin();
    
    while( it != myEntityVector.end()) {
        if(counter == ID){
            return (*it)->getName();
        }
        it++;
        counter++;
    }
    return NULL;
}