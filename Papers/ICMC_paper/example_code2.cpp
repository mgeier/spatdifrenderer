sdScene sce = sdLoader::sceneFromXML(xmlString);
sdEntityCore* ins = sce.getEntity("insect");
sdEvent* posEvt = ins->getFirstEvent(SD_POSITION);
sdEvent* medLocEvt = ins->getFirstEvent(SD_MEDIA_LOCATION);
cout << "Entity Name: " << ins->getName() << endl;
cout << "First event is at: " << posEvt->getValueAsString() << endl;
cout << "Attached Sound File: " << medLocEvt->getValueAsString() << endl;
cout << "Attached at: " << medLocEvt->getTimeAsString() << endl;