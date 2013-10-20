#include "spatdifRenderEngine.h"

//--------------------------------------------------------------
void spatdifApp::setup(){
    
    ofSetEscapeQuitsApp(false);
    ofSetVerticalSync(true);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(30);    
    logoImg.loadImage("SpatDIF_logo.png");
    TTF.loadFont("lucidagrande.ttf", 8, 1, 1, 0);
    sphere.setRadius(0.05);
    
    ofEnableDepthTest();
    cam.setPosition(0.2, 0.4, 10);
    cam.setNearClip(0.01);

    
	ofSetCircleResolution(64);
    
    spatDifObject = new(ofxSpatDIF);
    
    sndfileObject = new(ofxSndfile);
    
    ofBackground(255,255,255);
    
	ofSetFrameRate(60);
//    const char	*infilename = "../../../data/insect_simulated.aiff";
//	const char	*infilename = "../../../data/beating_loop.aiff";
  const char	*infilename = "/Users/jasch/Documents/_sounds/pana_03.aif";
//  const char	*infilename = "/Users/jasch/Documents/_sounds/scelsiPax.aiff";
    
    if( sndfileObject->loadSoundFile(infilename) ) {
        sndfileObject->looped = true;
        sndfileObject->state = 1;
        sndfileObject->framecounter = 0;
    }
    
//    loadSndfile(infilename);
    
        
    bufferSize          = 256;
	sampleRate 			= 44100;
//	volume				= 0.707107f;
	volume				= 1.0;
    
	for(int i=0; i < 2;i++) {
		audioAmplitudes[i] = 1.0f;
		
		float a = i * ( 2*PI / 8 ) - (0.5 * PI)-(0.125 * PI);
		float rad = 300;
		float x = cosf(a)*rad + ofGetWidth()/2;
		float y = sinf(a)*rad + ofGetHeight()/2;
		
		locspeakers[i].set(x,y);        
	}	
	soundStream.listDevices();  
	soundStream.setDeviceID(2);
	soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);

}

//--------------------------------------------------------------
void spatdifApp::update(){
    

}

//--------------------------------------------------------------
void spatdifApp::draw(){
    
    ofBackground(255, 255, 255, 255);
    ofEnableAntiAliasing();
    ofEnableDepthTest();

    
// start of camera transform
    //    ofVec3f target;
    //    target.set(0.0f, 0.0f, 0.0f);
    //    cam.setTarget(target);
    cam.setDistance(22);
    cam.begin();
    ofRotateY(0);
	ofRotateX(0);
	ofRotateZ(0);
    ofSetLineWidth(1.0);
    
	ofNoFill();
    ofSetColor(0, 0, 0, 63);
    ofRect(-10, -10, 0, 20, 20);
    float size = 10;
    float division = 20;
    float step = (2*size) / (division);
    
    for(int i = 0; i <= (division); i++) {
        ofLine(-size, (-size + i*step), 0, size, (-size + i*step), -0.01 ); // x-direction grid
        ofLine( (-size + i*step), -size, 0, (-size + i*step), size, -0.01); // y-direction grid
    }
    float arrowLength = 1;
    float arrowSize = arrowLength/10;
    
    ofSetLineWidth(2.0);
    ofSetColor(191,0,0, 127);
    ofLine(-arrowLength, 0, 0, arrowLength, 0, 0);
    ofLine(arrowLength, 0, 0, arrowLength-arrowSize, arrowSize, 0);
    ofLine(arrowLength, 0, 0, arrowLength-arrowSize, -arrowSize, 0);
    
    ofSetColor(0, 191, 0, 127);
    ofLine(0, -arrowLength, 0, 0, arrowLength, 0);
    ofLine(0, arrowLength, 0, arrowSize, arrowLength-arrowSize, 0);
    ofLine(0, arrowLength, 0, -arrowSize, arrowLength-arrowSize, 0);
    
    ofSetColor(0, 0, 191,127);
    ofLine(0, 0, -arrowLength, 0, 0, arrowLength);
    ofLine(0, 0, arrowLength,  arrowSize, 0, arrowLength-arrowSize);
    ofLine(0, 0, arrowLength, -arrowSize, 0, arrowLength-arrowSize);
    
	ofPopMatrix();
    
    if(haveScene) {
        ofScale(1.0, 1.0, 1.0);
        ofSetColor(255, 127, 0, 255);
        ofFill();
        ofPoint oldPoint;
        double currentTime = 0.0;

        sdEntityCore * entity = spatDifObject->myScene.getEntity(string(entityName));
        double* pos = static_cast<double*>(entity->getValue(currentTime , SD_POSITION));
        if(pos != NULL) {
            
            ofRect(pos[0]-0.05,pos[1]-0.05,pos[2], 0.1, 0.1);
            oldPoint.x = pos[0];
            oldPoint.y = pos[1];
            oldPoint.z = pos[2];
        }
        
        while (true) {
            sdEvent* sEvent = entity->getNextEvent(currentTime, SD_POSITION);
            if(sEvent == NULL) break;
            
            pos = static_cast<double*>(entity->getValue(sEvent->getTime(), SD_POSITION));
            
            ofRect(pos[0]-0.05,pos[1],pos[2],0.1, 0.1);
            ofSetLineWidth(1.0);
            ofLine(oldPoint.x, oldPoint.y, oldPoint.z, pos[0], pos[1], pos[2]);
            
            oldPoint.x = pos[0];
            oldPoint.y = pos[1];
            oldPoint.z = pos[2];
            currentTime = sEvent->getTime();
        }
        
    }
    cam.end();
    // end of camera transform
    ofDisableAntiAliasing();
    ofDisableDepthTest();
    // the GUI
    ofFill();
    ofSetColor(242);
    ofRect(0, 0, ofGetWidth(), 40);
    
    ofSetLineWidth(0.5);
    ofSetColor(0, 0, 0, 63);
    ofNoFill();
    ofRect(5, 5, 60, 16);
    ofSetColor(0, 0, 0, 63);
    TTF.drawString("open file", 9, 18);
    
    TTF.drawString(ofToString( (float)( (sndfileObject->framecounter) * bufferSize) / sampleRate), 100, 18);
    
    if(!playScene){
        ofLine(80, 5, 95, 13);
        ofLine(80, 21, 95, 13);
        ofLine(80, 5, 80, 21);
    }else{
    
    }
    
}

void spatdifApp::keyPressed  (int key){
    switch(key){
            case 'o':
                ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a SpatDIF File...");
            if (openFileResult.bSuccess){
                processOpenFileSelection(openFileResult);
            }
            break;
    
    
    }
}

void spatdifApp::mouseMoved(int x, int y ){

}

void spatdifApp::mouseDragged(int x, int y, int button){

}

void spatdifApp::mousePressed(int x, int y, int button){

}

void spatdifApp::mouseReleased(int x, int y, int button)
{
    if(x > 10 && x < 110 && y > 10 && y < 30){
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a SpatDIF File...");
		if (openFileResult.bSuccess){
			processOpenFileSelection(openFileResult);
		}else {
		}
    }
}

void spatdifApp::dragEvent(ofDragInfo dragInfo){

}

void spatdifApp::windowResized(int w, int h){
	
}

void spatdifApp::processOpenFileSelection(ofFileDialogResult openFileResult)
{
    currentFileName = openFileResult.getName();
    currentFilePath = openFileResult.getPath();

    // check filesignature for xml or json
    string checker = ofToUpper(currentFileName);
    int found = checker.rfind("XML");
    if(found >= 0){
        spatDifObject->loadSceneFromFile(currentFilePath, 0); // xml file type 0
        loadSceneInfo(&spatDifObject->myScene);
        haveScene = true;
        return;
    }
    found = checker.rfind("JSON");
    if(found >= 0){
        spatDifObject->loadSceneFromFile(currentFilePath, 1); // json file type 1
        loadSceneInfo(&spatDifObject->myScene);
        haveScene = true;
        return;
    }
    ofLogError("Not a valid SpatDIF file type", currentFileName.c_str());
}

void spatdifApp::playSndfile(SNDFILE *infile)
{
    sndfileObject->state = 1;
    sndfileObject->seekSoundFile(infile, 0, SEEK_CUR);

}

void spatdifApp::loadSceneInfo(sdScene * myScene)
{
    int numEntities = myScene->getNumberOfEntities();
    for(int i = 0; i < numEntities; i++) {
        entityName = spatDifObject->getEntityName(myScene, i);
        printf("ofxSpatDIF: entity found %s\n", entityName.c_str());
    }
    
    // run through all events of one entity and extract their time/position
    sdEntityCore * entity = myScene->getEntity(string(entityName));
    double* pos = static_cast<double*>(entity->getValue(0.0 , SD_POSITION));
    if(pos != NULL){
        cout << "position of " << entityName <<" at 0.0: " << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
    }
    double currentTime = 0.0;
    while (true) {
        sdEvent* sEvent = entity->getNextEvent(currentTime, SD_POSITION);
        if(sEvent == NULL) { break; }
        pos = static_cast<double*>(entity->getValue(sEvent->getTime(), SD_POSITION));
        cout << "position of " << entityName <<" at " << sEvent->getTimeAsString() << ": " << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
        currentTime = sEvent->getTime();
    }
}

bool playFile(){

}

bool stopFile(){}

bool pauseFile(){}

bool resumeFile(){}


void spatdifApp::audioOut(float * output, int bufferSize, int nChannels){
    
    float sample[2];
    float buffer[bufferSize * sndfileObject->sfinfo.channels];

    
    if(sndfileObject->state == 1) {
        sndfileObject->getNextFrame(sndfileObject->infile, buffer, bufferSize);
    }
    
    // get the  samples into the local buffer

    for (int i = 0; i < bufferSize; i++) {
        if(sndfileObject->state == 1){
            
            if(sndfileObject->sfinfo.channels == 2) {
                sample[0] = buffer[i * sndfileObject->sfinfo.channels];
                sample[1] = buffer[i * sndfileObject->sfinfo.channels + 1];
            }else{
                sample[0] = buffer[i * sndfileObject->sfinfo.channels];
                sample[1] = buffer[i * sndfileObject->sfinfo.channels]; 
            }
            
        }else{
            sample[0] = sample[1] = 0.0;
        }
        
    // fill the local buffer into the output stream
        for(int j = 0; j < nChannels; j++) {
            output[i * nChannels + j] = sample[j] * volume * audioAmplitudes[j];
        }
    }
}