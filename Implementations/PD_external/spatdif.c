#include "m_pd.h"
#include "sdMain.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;
typedef struct spatdif
{
    t_object x_obj;
    t_outlet *m_out, *b_out;
    sdScene * scene;
    sdOSCResponder responder;
} t_spatdif;

t_class *spatdif_class;

extern "C" {
    #include <string.h>
    void spatdif_setup(void);

}

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

void spatdif_save(t_spatdif *x, t_symbol *s){
    ofstream ofs(s->s_name);
    if(!ofs){
        post("spatdif: unable to open file %s", s->s_name);
        return;
    }
    ofs << sdSaver::XMLFromScene(x->scene)<< endl; // file closed automatically by the destructor of ofstream
}

void spatdif_load(t_spatdif *x, t_symbol *s){
    ifstream ifs(s->s_name);
    if(!ifs.is_open()){
        post("spatdif: unable to open file %s", s->s_name);
        return;
    }
    string xmlString;
    while ( ifs.good() )
    {
        string str;
        getline(ifs,str);
        xmlString.append(str);
    }
	// if(x->scene) {
	// 	free(x->scene); // free scene instance if a scene existed before
	// }
	//     x->scene = new(sdScene); // dynmically instanciate scene
	//     x->responder.setScene(x->scene); // tie responder to scene	
	
    *x->scene = sdLoader::sceneFromXML(xmlString); // file closed automatically by the destructor of ofstream	
}



void spatdif_interpret(t_spatdif *x, t_symbol *s, int argc, t_atom *argv)
{
    int i, rargc = 0;
    const char* typetags;
    bool autoAdvance = false;
    t_atom *rargv;
    string command;
    vector<sdOSCMessage> returnedMessageVector;
    vector<string> addressVector;

    command = s->s_name;
    
    // handle non OSC Message
    if(command == "dump"){
        post(x->scene->dump(false).c_str());
        return;
    }else if(command == "load"){
        spatdif_load(x, argv[0].a_w.w_symbol);
        return;
    }else if(command == "save"){
        spatdif_save(x, argv[0].a_w.w_symbol);
        return;
    }else if(command == "bang"){ // recursive call
        command = "/spatdifcmd/getEventSetsFromAllEntities";
        autoAdvance = true;
    }
    // if OSC message
    sdOSCMessage message(command);
    
    for ( i = 0; i < argc; i++){
        if (argv[i].a_type == A_FLOAT)
            message.appendFloat(argv[i].a_w.w_float);
        else if (argv[i].a_type == A_SYMBOL)
            message.appendString(argv[i].a_w.w_symbol->s_name);
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
            SETSYMBOL(&rargv[atomCount], gensym(element.c_str()));
            ait++;
            atomCount++;
        }

        for(int i = 1; i < static_cast<int>(typetags.size()); i++){
        	switch(typetags[i]){
                case 'i':{
                    SETFLOAT(&rargv[atomCount], static_cast<float>(returnedMessage.getArgumentAsInt(argCount)));
                    break;
                }
            	case 'f':{
                	SETFLOAT(&rargv[atomCount], returnedMessage.getArgumentAsFloat(argCount));
                	break;
                } 
                case 's':{
                    SETSYMBOL(&rargv[atomCount], gensym(returnedMessage.getArgumentAsString(argCount).c_str()));
                    break;
                }
        	}
        	atomCount++;
            argCount++;
        }
        
        outlet_anything(x->m_out, gensym(addressVector[1].c_str()), numAtoms, rargv);
        if(rargv){
            free(rargv);
        }
        it++;
    }
    //done bang from the second outlet
    if(autoAdvance){
        x->responder.setQueryTime(x->responder.getQueryTime() + x->responder.getInterval());
    }
    outlet_bang(x->b_out);
}

void spatdif_free(t_spatdif *x)
{
	if(x->scene){
		free(x->scene);
	}
}

void *spatdif_new(void)
{
    t_spatdif *x = (t_spatdif *)pd_new(spatdif_class);
    x->m_out = outlet_new(&x->x_obj, gensym("anything"));
    x->b_out = outlet_new(&x->x_obj, gensym("bang"));
        
    x->scene = new(sdScene); // dynmically instanciate scene
    x->responder.setScene(x->scene); // tie responder to scene
    
    return (void *)x;
}

void spatdif_setup(void)
{
    spatdif_class = class_new(gensym("spatdif"), (t_newmethod)spatdif_new,
    	(t_method)spatdif_free, sizeof(t_spatdif), 0, A_GIMME, 0);

    class_addanything(spatdif_class, (t_method)spatdif_interpret);
}

