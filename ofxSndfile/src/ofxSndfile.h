/*
 *  ofxSndfile.h
 *
 *
 */

#ifndef SOUND_FILE_LOADER_H
#define SOUND_FILE_LOADER_H


#include "ofMain.h"
#include "sndfile.h"

#define FRAMESIZE 256
class ofxSndfile{
	
    public:
        ofxSndfile();
        ~ofxSndfile();

        bool loadSoundFile(const char *infilename);
        void dumpInfo();
    
        int getNextFrame(SNDFILE *infile, float * buffer, int blocksize);
        int seekSoundFile(SNDFILE *infile, sf_count_t frames, int whence);
    
        SNDFILE *infile; // define input and output sound files
        SF_INFO sfinfo;  // struct to hold info about sound file
    
        int state; // 0 = stopped, 1 = playing, 2 = paused
        bool looped;
        int loopcounter;
    
};
#endif
