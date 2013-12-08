#include "spatdifRenderEngine.h"

//--------------------------------------------------------------

void spatdifApp::setup(){
    
    ofSetEscapeQuitsApp(false);
    ofSetVerticalSync(true);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(30);    
    logoImg.loadImage("SpatDIF_logo.png");
    TTF.loadFont("lucidagrande.ttf", 8, 1, 1, 1);
    TTFtiny.loadFont("lucidagrande.ttf", 0.5, 1, 1, 1);
    sphere.setRadius(0.05);
    
    ofEnableDepthTest();
    cam.setPosition(0.2, 0.4, 10);
    cam.setNearClip(0.01);
    
	ofSetCircleResolution(64);
    
    spatDifObject = new(ofxSpatDIF);
    
    
    ofBackground(255,255,255);
    
	ofSetFrameRate(60);
    
    bufferSize          = 256;
	sampleRate 			= 44100;
    frameDuration = (double)bufferSize / (double)sampleRate;
//    ofLog(OF_LOG_VERBOSE, "frameDuration is %f", frameDuration);

    
//	volume				= 0.707107f;
	volume				= 1.41;
    
	for(int i = 0; i < 2;i++) {
	
        audioAmplitudes[i] = 1.0f;

//		float a = i * ( 2 * PI / 4 ) - (0.5 * PI) - (0.125 * PI);
//		float rad = 1.0;
//		float y = cosf(a) * rad;
//		float x = sinf(a) * rad;
//		
//		locspeakers[i].set(x,y);
	}
    locspeakers[0].set(-1.0, 1.0, 0.0);
    locspeakers[1].set(1.0, 1.0, 0.0);
    
//    printf("locspeakers %f %f\n", locspeakers[0].x, locspeakers[0].y );
//    printf("locspeakers %f %f\n", locspeakers[1].x, locspeakers[1].y );
    
    
    parseDeviceList(soundStream);

    soundStream.setDeviceID(2);
	soundStream.setup(this, 2 , 0, sampleRate, bufferSize, 4);

    sceneTime = 0.0;
}

void spatdifApp::update(){
    
}

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
    
    // the grid and CS
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
    
    if(bHaveScene) {
        
        ofSetColor(0, 63, 127);
        ofRect(locspeakers[0].x-0.1, locspeakers[0].y-0.1, 0.2, 0.2);
        ofRect(locspeakers[1].x-0.1, locspeakers[1].y-0.1, 0.2, 0.2);
//        TTFtiny.drawStringAsShapes("L", locspeakers[0].x-0.2, locspeakers[0].y);
//        TTFtiny.drawStringAsShapes("R", locspeakers[0].x+0.2, locspeakers[0].y);
//        
        ofScale(1.0, 1.0, 1.0);

        
        if(!bPlayScene) {
            ofSetColor(191);
        }else{
            ofSetColor(255, 127, 0);
        }
        
        
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
        while (currentTime < sceneTime) {
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
        if(bPlayScene && !bScheduleLock){
            for(int i = 0; i < player.size(); i++){
                ofSetColor(0, 127, 255, 255);
                ofRect( player[i].currPosition.x-0.125, player[i].currPosition.y-0.125, 0.25, 0.25);
            }
        }
    }
    cam.end();
    // end of camera transform
    ofDisableAntiAliasing();
    ofDisableDepthTest();
    
    // the GUI
    ofFill();
    ofSetColor(242);
    ofRect(0, 0, ofGetWidth(), 44);
    
    ofSetLineWidth(1.0);
    ofSetColor(0, 0, 0, 127);
    
    ofNoFill();
    ofRect(5, 5, 100, 16);
    TTF.drawString("open file", 9, 18);

//    ofSetColor(0, 0, 0, 63);
//    ofRect(5, 23, 100, 16);
//    TTF.drawString("setup speakers", 9, 36);
    
    if(!bHaveScene){
        ofSetColor(191);
    }else{
        ofSetColor(0);
    }

    TTF.drawString(ofToString( sceneTime, 2) , 110, 36);

    int rightEdge = ofGetWidth()-20;
    ofLine(130, 5, 130, 22);
    ofLine(130, 13, rightEdge, 13);
    ofLine(rightEdge, 5, rightEdge, 22);
    
    if(bPlayScene) {
        int linePos = 130 + (rightEdge - 130) * (sceneTime / sceneLastEventTime);
        ofSetLineWidth(2.0);
        ofLine( linePos, 5, linePos, 22);
        ofSetLineWidth(1.0);
        ofNoFill();
        ofSetColor(232);
        ofRect(5, 50, 10, 100);
        ofRect(17, 50, 10,100);
        ofFill();
        ofSetColor(127);
        ofRect(5, 150-audioAmplitudes[0]*100, 10, audioAmplitudes[0]*100);
        ofRect(17, 150-audioAmplitudes[1]*100, 10, audioAmplitudes[1]*100);

        TTF.drawString(ofToString("L", 4), 5, 168);
        TTF.drawString(ofToString("R", 4), 17, 168);

        TTF.drawString("L "+ofToString(audioAmplitudes[0], 4), 5, 180);
        TTF.drawString("R "+ofToString(audioAmplitudes[1], 4), 5, 192);
        
        
    }
    // play tringle

    if(!bPlayScene){
        ofLine(110, 5, 125, 13);
        ofLine(110, 21, 125, 13);
        ofLine(110, 5, 110, 21);
    }else{
        ofRect(110, 5, 6, 16);
        ofRect(118, 5, 6, 16);
    }

    
//    // Menu
//    ofFill();
//    ofSetColor(232, 232, 232);
//    ofRect(5, 3, 188, 18);
//    ofNoFill();
//    ofSetColor(127, 127, 127);
//    ofRect(5, 3, 188, 18);
//    ofFill();
//    ofTriangle(5+177,7,5+185, 7, 5+181, 13);
//    ofSetColor(0, 0, 0);
//    TTF.drawString(deviceNames[0],5+5, 16);

    
    
//    if(menuState) {
//		int numMenuItems = (int)deviceNames.size();
//        
//		ofSetColor(232, 232, 232);
//		ofFill();
//		ofRect(5, 21, 188, numMenuItems*18);
//        
//		vector<string>::iterator it;
//		int i;
//		for ( it = deviceNames.begin(), i = 0 ; it < deviceNames.end(); it++) {
//				ofNoFill();
//				ofSetColor(232, 232, 232);
//				ofRect(5, 21+(i*18), 188, 18);
//				ofSetColor(0, 0, 0);
//				TTF.drawString(deviceNames[i], 9, 35+i*18 );
//				i++;
//		}
//		
//		ofNoFill();
//		ofSetColor(127, 127, 127);
//		ofRect(5, 21, 188, numMenuItems*18);
//	}
//    
//    
}

void spatdifApp::keyPressed  (int key) {
    switch(key) {
        case ' ':
            if(!bPlayScene){
                playScene();
            }else{
                stopScene();
            }
			break;
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
   
    if(x > 10 && x < 200
       && y > 3 && y < 21) {
		if(menuState != 0) {
			menuState = 0;
		} else {
			menuState = 1;
			parseDeviceList(soundStream);
		}
    }

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
    if(x > 110 && x < 125 && y > 5 && y < 21){
        if(bHaveScene){
            if(!bPlayScene) {
                playScene();
            }else{
                stopScene();
            }
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
    int slash = currentFilePath.find_last_of("/\\");
    rootPath = currentFilePath.substr(0, slash+1);
//    ofLog(OF_LOG_NOTICE, "rootPath %s", rootPath.c_str());

    // check filesignature for xml or json
    string checker = ofToUpper(currentFileName);
    int found = checker.rfind("XML");
    if(found >= 0){
        spatDifObject->loadSceneFromFile(currentFilePath, 0); // xml file type 0
        loadSceneInfo(&spatDifObject->myScene);
        bHaveScene = true;
        ofSetWindowTitle(currentFileName);
        return;
    }
    found = checker.rfind("JSON");
    if(found >= 0){
        spatDifObject->loadSceneFromFile(currentFilePath, 1); // json file type 1
        loadSceneInfo(&spatDifObject->myScene);
        bHaveScene = true;
        ofSetWindowTitle(currentFileName);
        return;
    }
    ofLogError("Not a valid SpatDIF file type", currentFileName.c_str());
}

void spatdifApp::playSndfile(SNDFILE *infile)
{
//    sndfileObject->state = 1;
//    sndfileObject->seekSoundFile(infile, 0, SEEK_SET);
}

void spatdifApp::loadSceneInfo(sdScene * myScene)
{
    sceneFirstEventTime = 0.0;
    sceneLastEventTime = 0.0;
    int numEntities = myScene->getNumberOfEntities();

    player.clear();
    
    for(int i = 0; i < numEntities; i++) {
        
        entityName = spatDifObject->getEntityName(myScene, i);
        
        printf("ofxSpatDIF: entity found %s\n", entityName.c_str());
        // run through all events of one entity and extract their time/position
        
        sdEntityCore * entity = myScene->getEntity(string(entityName));
        
        ofxSpatDIFPlayer * newPlayer = new ofxSpatDIFPlayer;
        
        newPlayer->initialize(entity, rootPath);
        
        if(newPlayer->lastEventTime >= sceneLastEventTime){
            sceneLastEventTime  = newPlayer->lastEventTime;
        }
        if(newPlayer->firstEventTime >= sceneFirstEventTime){
            sceneFirstEventTime  = newPlayer->firstEventTime;
        }        
        player.push_back( *newPlayer );
        
//        ofLog(OF_LOG_NOTICE, "loaded scene info %s", spatDifObject->currentFilePath.c_str() );

        /*
        double * pos = static_cast<double*>(player[i].entity->getValue(0.0 , SD_POSITION));
        if(pos != NULL){
            cout << "position of " << entityName <<" at 0.0: " << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
        }
        double currentTime = 0.0;
        while (true) {
            sdEvent* sEvent = player[i].entity->getNextEvent(currentTime, SD_POSITION);
            if(sEvent == NULL) { break; }
            pos = static_cast<double*>(player[i].entity->getValue(sEvent->getTime(), SD_POSITION));
            cout << "position of " << entityName <<" at " << sEvent->getTimeAsString() << ": " << pos[0] << ' ' << pos[1] << ' ' << pos[2] << endl;
            currentTime = sEvent->getTime();
        }
         */
        
    }
}

bool spatdifApp::playScene()
{
    bPlayScene = true;
    for( int i = 0; i <
        player.size(); i++){
        player[i].isPlaying = false;
    }
    sceneTime = 0.0;
    frameCounter = 0;
}

bool spatdifApp::stopScene()
{
    bPlayScene = false;
    for(int i = 0; i < player.size(); i++){
        player[i].sndfileObject->state = false;
    }
}

void spatdifApp::audioOut(float * output, int bufferSize, int nChannels){
    
    float sample[2];
    if(bHaveScene){
        
        float buffer[bufferSize * player[0].sndfileObject->sfinfo.channels];
        runSchedule();

        if(player[0].sndfileObject->state == 1) {
            player[0].sndfileObject->getNextFrame(player[0].sndfileObject->infile, buffer, bufferSize);
        }
        
        // get the  samples into the local buffer
        audioAmplitudes[0] = 0.0;
        audioAmplitudes[1] = 0.0;
        audioAmplitudes[2] = 0.0;
        audioAmplitudes[3] = 0.0;
        audioAmplitudes[4] = 0.0;
        audioAmplitudes[5] = 0.0;
        audioAmplitudes[6] = 0.0;
        audioAmplitudes[7] = 0.0;

        for (int i = 0; i < bufferSize; i++) {
            
            if(player[0].sndfileObject->state == 1){
                
                if(player[0].sndfileObject->sfinfo.channels == 2) {
                    sample[0] = buffer[i * player[0].sndfileObject->sfinfo.channels];
                    sample[1] = buffer[i * player[0].sndfileObject->sfinfo.channels + 1];
                }else{
                    
                    sample[0] = buffer[i * player[0].sndfileObject->sfinfo.channels];
                    sample[1] = buffer[i * player[0].sndfileObject->sfinfo.channels]; 
                }
                
            }else{
                sample[0] = sample[1] = 0.0;
            }
            
            // fill the local buffer into the output stream
            
            for(int j = 0; j < nChannels; j++) {
                for(int k = 0; k < player.size(); k++) {
                    output[i * nChannels + j] = sample[j] * volume * player[k].amplitude[j]; // here do the panning
                    float absSignal = abs(output[i * nChannels + j]);
                    if(audioAmplitudes[j] < absSignal){
                        audioAmplitudes[j] = absSignal;
                    }                                                            // collect the sample values
                }
            }
        }
        // the slide~ lowpass filter formula (n) = y (n-1) + ((x (n) - y (n-1))/slide)
        for(int j = 0; j < nChannels; j++) {
            audioAmplitudes[j] = prevAudioAmplitudes[j] + ((audioAmplitudes[j] - prevAudioAmplitudes[j]) / 5.0); // lowpass filter the audioAmplitudes for display purposes
            
            prevAudioAmplitudes[j] = audioAmplitudes[j];
        }
        frameCounter++;
    }

}

void spatdifApp::runSchedule()
{
    if(bPlayScene){
        bScheduleLock = true;
        sceneTime = (double)(frameCounter * bufferSize) / (double)sampleRate;
        
        // run the entities
        for(int i = 0; i < player.size(); i++){
            // load media
            if(sceneTime >= player[i].mediaStartTime && player[i].isPlaying == false){
                player[i].sndfileObject->state = 1;
                player[i].sndfileObject->framecounter = 0;
                player[i].sndfileObject->seekSoundFile( player[i].sndfileObject->infile, 0, SEEK_SET);
                player[i].isPlaying = true;
                
            }
            // get positions
            
            if(sceneTime >= player[i].firstEventTime) {
//                player[i].prevEventTime = sceneTime;
                // get current position
                double * pos = static_cast<double*>(player[i].entity->getValue(sceneTime, SD_POSITION));
                if(pos != NULL){
                    player[i].currPosition.x = pos[0];
                    player[i].currPosition.y = pos[1];
                    player[i].currPosition.z = pos[2];
                    ofLog(OF_LOG_VERBOSE, "found intial curr position" );
                }
                
                // get next position
                sdEvent* sEvent = player[i].entity->getNextEvent(sceneTime, SD_POSITION);
                if(sEvent != NULL) {
                    pos = static_cast<double*>(player[i].entity->getValue(sEvent->getTime(), SD_POSITION));
                    if(pos != NULL) {
                        player[i].nextPosition.x = pos[0];
                        player[i].nextPosition.y = pos[1];
                        player[i].nextPosition.z = pos[2];
                        
                        ofLog(OF_LOG_VERBOSE, "sceneTime %f - curr %f %f %f - next %f %f %f - prev %f next %f",
                              sceneTime,
                              player[i].currPosition.x, player[i].currPosition.y, player[i].currPosition.z,
                              player[i].nextPosition.x, player[i].nextPosition.y, player[i].nextPosition.z,
                              player[i].prevEventTime,
                              player[i].nextEventTime );
                        
                        if(sceneTime >= player[i].nextEventTime) {

                            numInterpSteps = (player[i].nextEventTime - sceneTime) / frameDuration;
                            
                            ofLog(OF_LOG_VERBOSE, "reached next eventTime %f at sceneTime %f, numInterpSteps %d", player[i].nextEventTime, sceneTime, numInterpSteps);

                            player[i].currPosition.x = player[i].nextPosition.x;
                            player[i].currPosition.y = player[i].nextPosition.y;
                            player[i].currPosition.z = player[i].nextPosition.z;
                        }
                        player[i].prevEventTime = player[i].nextEventTime;
                        player[i].nextEventTime = sEvent->getTime(); // NB it is critical to get nextEveenTime after the comparison to get the frame where it changes
                    }
                }
            }
            
            // calc panning
            
//            for(int j = 0; j < 2 ; j++){
//                float distance = player[i].currPosition.distance(locspeakers[j]);
//                player[i].amplitude[j] = 10.0 - distance ; //ofMap(distance, 0, 1., 1., 0., false);
//            }
            
            for(int j = 0; j < 2 ; j++){
                ambipanning_calc(i, j);
            }
            
        }
        
        // stop at end of scene
        if(sceneTime > sceneLastEventTime) {
            bPlayScene = false;
            for(int i = 0; i < player.size(); i++){
                player[i].sndfileObject->state = 0;
                
            }
        }
        bScheduleLock = false;
    }
}


void spatdifApp::ambipanning_calc(int ID, int channel)
{
    double center_size = 1.414213562373095;
//    double center_size = 0.1;
    double dist_gain;
    double order = 5.0;
    double dist;
    double dbunit = -0.5;
    double x = player[ID].currPosition.x;
    double y = player[ID].currPosition.y;
    double z = player[ID].currPosition.z;
    
    double spk_x = locspeakers[channel].x;
    double spk_y = locspeakers[channel].y;
    double spk_z = locspeakers[channel].z;
    double spk_dist = sqrt(spk_x*spk_x + spk_y*spk_y + spk_z*spk_z);

    dist = player[ID].currDistance = sqrt(x*x + y*y + z*z);

 
    if(dist < center_size) { // inside center_zone
//        dist_gain = 1.0 - (dist / center_size);
//        
//        
//        order *= (dist / center_size);        
        dist_gain = pow(10, (center_size) * dbunit * 0.05);

    } else {	// outside center_zone
        dist_gain = pow(10, (dist - center_size) * dbunit * 0.05);
    }
    player[ID].amplitude[channel] = dist_gain * pow( 0.5 + ( 0.5 * ((x * spk_x + y * spk_y + z * spk_z) / (dist * spk_dist))),   order);
}

// TODO: panning without crackling


bool spatdifApp::parseDeviceList(ofSoundStream mySoundStream)
{
    
    // capture the cout of the listdevices into the stringstream
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    ostringstream capturedString;
    cout.rdbuf( capturedString.rdbuf() );
    
	mySoundStream.listDevices();
    
    cout.rdbuf( oldCoutStreamBuf ); // restoring old cout.
    
//    printf("after capture \n %s", capturedString.str().c_str());
    
    
    std::string str1 ("device");
    std::string str2 ("[notice");

    std::size_t found = 0;
    std::size_t found2 = 0;

    while (found2 != std::string::npos) {
        found = capturedString.str().find(str1, found);
//        if (found != std::string::npos) {
//            std::cout << "'device' found at: " << found << '\n';
//            
//        }
//        
        found2 = capturedString.str().find(str2, found+10);
//        if (found2 != std::string::npos) {
//            std::cout << "'[notice ]' found at: " << found2 << '\n';
//        }
        if(found > capturedString.str().length()){
            break;
        }
        deviceNames.push_back(capturedString.str().substr(found+7, (found2-found)-8).c_str());
        found = found2+10;
    }
    
    for(int i = 0; i < deviceNames.size();i++){
        ofLog(OF_LOG_NOTICE, deviceNames[i] );
    }
}