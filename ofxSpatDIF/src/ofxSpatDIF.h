//
//  ofxSpatDIF.h
//
//  Created by jasch on 27/9/2013.
//
//

#ifndef __ofxSpatDIF__
#define __ofxSpatDIF__

#include <iostream>

#include "sdConst.h"
#include "sdDate.h"
#include "sdEntity.h"
#include "sdEntityCore.h"
#include "sdEntityExtension.h"
#include "sdEntityExtensionMedia.h"
#include "sdInfo.h"
#include "sdLoader.h"
#include "sdOSCResponder.h"
#include "sdSaver.h"
#include "sdScene.h"

class ofxSpatDIFPlayer{
    
public:    
    ofxSpatDIFPlayer()
    {
        entityName = "";
        numMedia = 0;
        mediaID = NULL;

    }
    virtual ~ofxSpatDIFPlayer(){};
    
    string entityName;
    int numMedia;
    string * mediaID;

    
};

class ofxSpatDIF{
	
    public:
        
        ofxSpatDIF();
        ~ofxSpatDIF();
        
        void loadFile(char * buffer);
        void loadSceneFromFile(string filePath, int type);
        
        unsigned int getNumberOfEntities(sdScene theScene);
        string getEntityName(sdScene * theScene, int ID);
        sdScene myScene;
    
        ofxSpatDIFPlayer * player;
};



#endif /* defined(__ofxSpatDIF__) */
