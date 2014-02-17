sdScene scene = sdLoader::sceneFromXML(xmlString);
sdEntityCore* insect = scene.getEntity("insect");
sdEvent* insectPositionEvent = insect->getFirstEvent(SD_POSITION);
sdEvent* insectMediaLocationEvent = insect->getFirstEvent(SD_MEDIA_LOCATION);

cout << "Entity Name: " << insect->getName() << endl;
cout << "First event is at: " << insectPositionEvent->getValueAsString() << endl;
cout << "Attached Sound File: " << insectMediaLocationEvent->getValueAsString() << endl;
cout << "Attached at: " << insectMediaLocationEvent->getTimeAsString() << endl;