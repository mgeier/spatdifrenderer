/*
 *  ofxSndfile.cpp
 *
 *
 */

#include "ofxSndfile.h"

ofxSndfile::ofxSndfile() {
    
	sfinfo.format = 0;
    looped = 0;
    loopcounter = 0;
}

ofxSndfile::~ofxSndfile() {
	sf_close(infile);
}

bool ofxSndfile::loadSoundFile(const char *infilename) {
	
    if (! (infile = sf_open (infilename, SFM_READ, &sfinfo))){
        ofLogVerbose("SF OPEN routine Not able to open input file %s", infilename) ;
        ofLogError("libsndfile", sf_strerror(NULL)) ;
        return false;
	} else{
        ofLogVerbose("SF OPEN : file loaded:", infilename);
        dumpInfo();
        return true;
	}
}

void ofxSndfile::dumpInfo() {
    
    ofLogVerbose( "libsndfile infos\n  number of frames " + ofToString(sfinfo.frames)
        + "\n  samplerate " + ofToString(sfinfo.samplerate)
        + "\n  number of channels " + ofToString(sfinfo.channels)
//        + "\n  format " + ofToString(()sfinfo.format)
        + "\n  sections " + ofToString(sfinfo.sections)
        + "\n  seekable " + ofToString(sfinfo.seekable)
    );
    printf("format is 0x%08X\n\n", sfinfo.format);

}

int ofxSndfile::getNextFrame(SNDFILE *infile, float * buffer, int blocksize) {

    int k, m, readcount ;

    if( (readcount = sf_readf_float (infile, buffer, blocksize)) > 0) {
        // readcount returns blocksize unless the end is reached
        if(readcount < blocksize){
            if(looped){
                sf_seek(infile, (sf_count_t)0, SEEK_SET);
                loopcounter++;
            }else {
                state = 0;
            }
        }
    }
    return readcount;
}

int seekSoundFile(SNDFILE *infile, sf_count_t frames, int whence = SEEK_CUR){

    return sf_seek(infile, frames, whence) ;
    
    // possible 'whence' values
    // Absolute:
    // SEEK_SET  - The offset is set to the start of the audio data plus offset (multichannel) frames.
    // Relative
    // SEEK_CUR  - The offset is set to its current location plus offset (multichannel) frames.
    // Abolute from the end
    // SEEK_END  - The offset is set to the end of the data plus offset (multichannel) frames.
    
}








