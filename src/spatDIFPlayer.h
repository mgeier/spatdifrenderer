//
//  spatDIFPlayer.h
//  SpatDIFrenderer
//
//  Created by jasch on 28/11/2013.
//
//

#ifndef SpatDIFrenderer_spatDIFPlayer_h
#define SpatDIFrenderer_spatDIFPlayer_h


#include "ofMain.h"
#include "ofxSpatDIF.h"
#include "ofxSndfile.h"

class ofxSpatDIFPlayer{
    
public:
    ofxSpatDIFPlayer()
    {
        entityName = "";
        numMedia = 0;
        mediaID = NULL;
        filePath = "";
        mediaStartTime = 1.0;;
    }
    
    virtual ~ofxSpatDIFPlayer(){
    
    
    };
    
    void initialize(sdEntityCore * myEntity, string myRootPath)
    {
        rootPath = myRootPath;
        
        entity = myEntity;
        entityName = entity->getName();
        
        // get first filpath and time of media
        sdEvent* mediaLocationEvent = entity->getFirstEvent(SD_MEDIA_LOCATION);
        filePath = mediaLocationEvent->getValueAsString();
        mediaStartTime = mediaLocationEvent->getTime();
        
        firstEventTime = entity->getFirstTimeTag();
        lastEventTime = entity->getLastTimeTag();
        
        ofLog(OF_LOG_NOTICE, "player: loaded entity %s with media %s starting at %f, entity limits %f %f", entityName.c_str(), filePath.c_str(), mediaStartTime, firstEventTime, lastEventTime);
        
        sndfileObject = new(ofxSndfile);
        string infilename = rootPath+filePath;
        sndfileObject->loadSoundFile(infilename.c_str());
        
        isPlaying = false;

    }
    
    void restart(){
    
    }
    
    sdEntityCore * entity;
    ofxSndfile * sndfileObject;
    string rootPath;
    
    string entityName;
    int numMedia;
    string * mediaID;
    string filePath;
    double mediaStartTime;
    
    double firstEventTime;
    double lastEventTime;

    double prevEventTime;
    double nextEventTime;
    
    ofPoint currPosition;
    double currDistance;
    
    ofPoint nextPosition;
    double nextDistance;
    
    bool reachedNExtEvent;
    bool isPlaying;
    
    float amplitude[32];
    
};
#endif
