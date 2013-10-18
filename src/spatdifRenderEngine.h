#pragma once

#include "ofMain.h"
#include "ofxSpatDIF.h"
#include "ofxSndfile.h"

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
    
        ofxSpatDIF * spatDifObject;
        ofxSndfile * sndfileObject;
    
//        void loadSndfile(const char * infilename);
        void playSndfile(SNDFILE *infile);

        void processOpenFileSelection(ofFileDialogResult openFileResult);
        void loadSceneInfo(sdScene * myScene);
    
        void audioOut(float * output, int bufferSize, int nChannels);

        bool playFile();
        bool stopFile();
        bool pauseFile();
        bool resumeFile();

        ofImage logoImg;
        
        string currentFileName;
        string currentFilePath;
    
        bool haveScene;
        bool newFileLoaded;
        bool playScene;
        
        ofEasyCam cam; // add mouse controls for camera movement
    
        string entityName;
        
        ofSoundStream soundStream;
        vector <float> audioSignals[8]; // audio signals
    
        ofPoint locspeakers[8];
        ofPoint locaudio;
        float   audioAmplitudes[8]; // audio volumes for channels
        int     bufferSize;
        int     sampleRate;
        float 	volume;

    
        ofTrueTypeFont TTF;
    
};
