#pragma once

#include "ofMain.h"
#include "ofxSpatDIF.h"
#include "ofxSndfile.h"
#include "spatDIFPlayer.h"

class spatdifApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
//		void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
//		void gotMessage(ofMessage msg);

    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void loadSceneInfo(sdScene * myScene);
    void playSndfile(SNDFILE *infile);
    void audioOut(float * output, int bufferSize, int nChannels);

    void runSchedule();

    bool playScene();
    bool stopScene();
    void ambipanning_calc(int ID, int channel);

    bool parseDeviceList(ofSoundStream mySoundStream);

    ofxSpatDIF * spatDifObject;

    ofImage logoImg;
    
    string currentFileName;
    string currentFilePath;
    string rootPath;

    bool bHaveScene;
    bool bNewFileLoaded;
    bool bPlayScene;

    double sceneFirstEventTime;
    double sceneLastEventTime;

    double sceneTime;
    long long audioTime;
    long long frameCounter;

    ofEasyCam cam; // add mouse controls for camera movement
        
    ofSoundStream soundStream;
    vector <float> audioSignals[8]; // audio signals

    ofPoint locspeakers[8];
    ofPoint locaudio;
    float   audioAmplitudes[8]; // audio volumes for channels
    float   prevAudioAmplitudes[8]; // audio volumes for channels
    int     bufferSize;
    int     sampleRate;
    float 	volume;
    double frameDuration;
    long    numInterpSteps;


    ofTrueTypeFont TTF;
    ofTrueTypeFont TTFtiny;
    ofSpherePrimitive sphere;

    string entityName;
    vector <ofxSpatDIFPlayer> player;

    bool bScheduleLock;

    vector <string>deviceNames;
    bool	menuState;
    int		numMenuItems;
    ofPoint menuClickPos;
    
};
