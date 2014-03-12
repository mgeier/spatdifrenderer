/**

 
*/

extern "C" { // c-headers
    #include "ext.h"							// standard Max include, always required
    #include "ext_obex.h"						// required for new style Max object
    #include <string.h>
}

#include "sdMain.h"

typedef struct _spatdif 
{
	t_object ob;			// the object itself (must be first)
    sdScene * scene;        // pointer is dynamically allocated in the loading function
    bool sceneLoaded;
    sdOSCResponder responder;
    void *outlet1;
	void *outlet2;
    
} t_spatdif;

void *spatdif_new(t_symbol *s, long argc, t_atom *argv);
void spatdif_free(t_spatdif *x);
void spatdif_assist(t_spatdif *x, void *b, long m, long a, char *s);

void spatdif_read(t_spatdif *x, t_symbol *s);
void spatdif_doread(t_spatdif *x, t_symbol *s, short ac, t_atom *av);

void spatdif_write(t_spatdif *x, t_symbol *s);
void spatdif_dowrite(t_spatdif *x, t_symbol *s, short ac, t_atom *av);

int getNumberOfEntities(t_spatdif *x);
string getEntityName(t_spatdif *x, int ID);
void spatdif_dumpScene(t_spatdif *x);
vector<string> splitAddress(string &str);
void spatdif_interpret(t_spatdif *x, t_symbol *s, int argc, t_atom *argv);

void *spatdif_class;

int C74_EXPORT main(void)
{	
	t_class *c;
	
	c = class_new("spatdif", (method)spatdif_new, (method)spatdif_free, (long)sizeof(t_spatdif), 0L, A_GIMME, 0);
	   
    
    class_addmethod(c, (method)spatdif_read,            "read",     A_DEFSYM, 0);
    class_addmethod(c, (method)spatdif_write,           "write",    A_DEFSYM, 0);
    class_addmethod(c, (method)spatdif_interpret,       "anything", A_GIMME, 0);
    
    class_addmethod(c, (method)spatdif_assist,			"assist",		A_CANT, 0);
    
    class_register(CLASS_BOX, c);
	spatdif_class = c;

    post("spatdif © 2014 all rights reserved",0);
	return 0;
}

void spatdif_assist(t_spatdif *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) { // inlet
		sprintf(s, "I am inlet %ld", a);
	} 
	else {	// outlet
		sprintf(s, "I am outlet %ld", a); 			
	}
}

void spatdif_free(t_spatdif *x)
{
    if(x->scene) {
        free(x->scene);
    }
}

void *spatdif_new(t_symbol *s, long argc, t_atom *argv)
{
	t_spatdif *x = NULL;

	// object instantiation, NEW STYLE
	if ((x = (t_spatdif *)object_alloc((t_class *) spatdif_class))) {
        
        x->sceneLoaded = false;
        
        x->outlet2 = outlet_new((t_object *)x, 0L);
        x->outlet1 = outlet_new((t_object *)x, 0L);
	}
	return (x);
}

#pragma mark -
#pragma mark file loading

void spatdif_read(t_spatdif *x, t_symbol *s)
{
	defer_low(x,(method)spatdif_doread, s, 0, 0L);
}

void spatdif_doread(t_spatdif *x, t_symbol *s, short ac, t_atom *av)
{
	t_filehandle filehandle = 0;
	char        filename[MAX_FILENAME_CHARS];
	char        filename2[MAX_FILENAME_CHARS];
    
	short       path_id;
	t_fourcc	outtype;
	char        *buffer;
	t_ptr_size	size;
	
	if (!s->s_name[0]) {
		if(open_dialog(filename, &path_id, &outtype, 0L, 0)) {
            return;
        }
    } else {
		strcpy(filename, s->s_name);
		if(locatefile_extended(filename, &path_id, &outtype, 0L, 0)) {
			object_error((t_object *)x, "can't find file \"%s\"", filename);
			return;
		}
	}
	if (path_opensysfile(filename, path_id, &filehandle, READ_PERM)) {
		object_error((t_object *)x, "can't open file \"%s\"", filename);
		return;
	}
    int len = strlen(filename);
    for(int i = 0; i < len; i++){
        filename2[i] = toupper(filename[i]);
    }
    string checker(filename2);
    int found = checker.rfind("XML", checker.length());
    bool haveValidXMLFile = false;
    if(found >= 0){
        haveValidXMLFile = true;
    }
    found = checker.rfind("JSON", checker.length());
    bool haveValidJSONFile = false;
    if(found >= 0){
        haveValidJSONFile = true;
    }
    if(haveValidXMLFile || haveValidJSONFile) {
        sysfile_geteof(filehandle, &size);			// allocate memory
        buffer = sysmem_newptr(size);
        sysfile_read(filehandle, &size, buffer); 	// read in the file
        
        string sceneBuffer(buffer);
        
        if(x->scene) {
            free(x->scene);
        }
        x->scene = new(sdScene);

        *x->scene = sdLoader::sceneFromXML(sceneBuffer);

        x->sceneLoaded = true;
        x->responder.setScene(x->scene); // tie responder to scene
        x->scene->dump();
    
        sysfile_close(filehandle);						// close, dealloc, etc
        sysmem_freeptr(buffer);
    }else {
        object_error((t_object *)x, "%s not a valid file", filename);
        return;
    }
    
}

#pragma mark -
#pragma mark file saving

void spatdif_write(t_spatdif *x, t_symbol *s)
{
    defer_low(x,(method)spatdif_dowrite, s, 0, 0L);
}

void spatdif_dowrite(t_spatdif *x, t_symbol *s, short ac, t_atom *av)
{
    t_fourcc filetype = 'TEXT', outtype;
    t_ptr_size filepos;
	t_filehandle file;
	char filename[MAX_FILENAME_CHARS] = "spatdif_scene.xml";
	short path;
    string sceneBuffer;
    t_ptr_size err;
	t_ptr_size count;
    
    if(x->scene == NULL){
        object_error((t_object *)x, "no scene data to write");
        return;
    }
	
	if (!s->s_name[0]) {
		if(saveasdialog_extended(filename, &path, &outtype, &filetype, 1)) // non-zero: user cancelled
			return;
	} else {
		strcpy(filename, s->s_name);
		path = path_getdefault();
	}
	
    sceneBuffer = sdSaver::XMLFromScene(x->scene);

	if (path_createsysfile(filename, path, 'TEXT', &file)) {
		object_error((t_object *)x, "%s: couldn't write data to file \"%s\"", filename);
		return;
	}
	
	count = strlen(sceneBuffer.c_str());
	err = sysfile_write(file, &count, sceneBuffer.c_str());
	sysfile_getpos(file, &filepos);
	sysfile_seteof(file, filepos);
	sysfile_close(file);

}

#pragma mark -
#pragma mark utilities

int getNumberOfEntities(t_spatdif * x)
{
    return x->scene->getNumberOfEntities();
}

string getEntityName(t_spatdif *x, int ID)
{
    int counter = 0;
    vector <sdEntityCore*> myEntityVector = x->scene->getEntityVector();
    vector <sdEntityCore*>::iterator it = myEntityVector.begin();
    
    while( it != myEntityVector.end()) {
        if(counter == ID){
            return (*it)->getName();
        }
        it++;
        counter++;
    }
    return NULL;
}

void spatdif_dumpScene(t_spatdif *x)
{
    cout << "--------------- SpatDIF scene Dump ---------------" << endl;

    if(x->sceneLoaded) {
        int numExtensions = x->scene->getNumberOfActivatedExtensions();
        cout << "number of active extensions: " << numExtensions << endl;
        
        int numEntities = x->scene->getNumberOfEntities();
        cout << "number of entities: " << numEntities << endl;
        
        
        for(int i = 0; i < numEntities; i++) {
            string result = x->scene->getEntityName(i);
            cout << "entity name " << i<< ":" << result << endl;
        }
        
        x->scene->dump();
    }
}

#pragma mark -
#pragma mark utilities

vector<string> splitAddress(string &str){
    vector<string> res;
    size_t current = 0, found;
    str.erase(0,1); // erase first slash
    while((found = str.find_first_of('/', current)) != string::npos){
        res.push_back(string(str, current, found - current));
        current = found + 1;
    }
    res.push_back(string(str, current, str.size() - current));
    return res;
}

void spatdif_interpret(t_spatdif *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;//, rargc = 0;
    //    const char* typetags;
    bool autoAdvance = false;
    t_atom *rargv;
    string command;
    vector<sdOSCMessage> returnedMessageVector;
    vector<string> addressVector;
    
    
    
    command = s->s_name;
    
    object_post((t_object *)x, "spatdif-command is \"%s\"", command.c_str() );
    
    // handle non OSC Message
    if(command == "dump"){
        spatdif_dumpScene(x);
        return;
    }else if(command == "load"){
        if(argc > 0) {
            spatdif_read(x, argv[0].a_w.w_sym);
        }else{
            spatdif_read(x, NULL);
        }
        return;
    }else if(command == "save"){
        if(argc > 0) {
            spatdif_write(x, argv[0].a_w.w_sym);
        }else{
            spatdif_write(x, NULL);
        }        return;
    }else if(command == "bang"){ // recursive call
        command = "/spatdifcmd/getEventSetsFromAllEntities";
        autoAdvance = true;
    }
    // if OSC message
    sdOSCMessage message(command);
    

    
    for ( i = 0; i < argc; i++){
        if (argv[i].a_type == A_LONG) {
            message.appendInt(argv[i].a_w.w_long);
        } else if (argv[i].a_type == A_FLOAT) {
            message.appendFloat(argv[i].a_w.w_float);
        } else if (argv[i].a_type == A_SYM) {
            message.appendString(argv[i].a_w.w_sym->s_name);
        }
    }
    
    // access the SpatDIF Library and get answer(s)!
    returnedMessageVector = x->responder.forwardOSCMessage(message);
    
    // examine each returned message
    vector<sdOSCMessage>::iterator it = returnedMessageVector.begin();
    while (it != returnedMessageVector.end()) {
    	int numAtoms = 0, numArguments = 0, atomCount = 0, argCount = 0;
        sdOSCMessage returnedMessage = *it;
        vector<unsigned char> typetags = returnedMessage.getTypetags();
        string address = returnedMessage.getAddressAsString();
        addressVector = splitAddress(address); // separate elements in the address pattern
        
        // how many atoms to be sent out from the outlet ?
		numAtoms += (addressVector.size() - 2); // because we will omit the header /spatdif and the
		numArguments = returnedMessage.getNumberOfArgument();
		numAtoms += numArguments; // one arg one atom
        rargv = (t_atom*)malloc(sizeof(t_atom) * numAtoms); // allocate buffers
        
        // put address
        vector<string>::iterator ait = addressVector.begin() + 2; // skip spatdif header and second element
        while (ait != addressVector.end()) {
            string element = *ait;
            atom_setsym(&rargv[atomCount], gensym(element.c_str()));
            ait++;
            atomCount++;
        }
        
        for(int i = 1; i < typetags.size(); i++){
        	switch(typetags[i]){
                case 'i':{
                    atom_setfloat(&rargv[atomCount], static_cast<float>(returnedMessage.getArgumentAsInt(argCount)));
                    break;
                }
            	case 'f':{
                	atom_setfloat(&rargv[atomCount], returnedMessage.getArgumentAsFloat(argCount));
                	break;
                }
                case 's':{
                    atom_setsym(&rargv[atomCount], gensym(returnedMessage.getArgumentAsString(argCount).c_str()));
                    break;
                }
        	}
        	atomCount++;
            argCount++;
        }
        
        outlet_anything(x->outlet1, gensym(addressVector[1].c_str()), numAtoms, rargv);
        if(rargv){
            free(rargv);
        }
        it++;
    }
    //done bang from the second outlet
    if(autoAdvance){
        x->responder.setQueryTime(x->responder.getQueryTime() + x->responder.getInterval());
    }
    outlet_bang(x->outlet2);
}