

#include "m_pd.h"
#include "sdMain.h"
#include <string>
#include <vector>

using namespace std;
typedef struct spatdif
{
    t_object x_obj;
    t_outlet *m_out, *b_out;
    sdScene scene;
    sdOSCResponder responder;
    double writeTime;
    double queryTime;
} t_spatdif;

t_class *spatdif_class;

extern "C" {
    #include <string.h>
    void spatdif_setup(void);
}

vector<string> splitMessage(string &str){
    vector<string> res;
    size_t current = 0, found;
    while((found = str.find_first_of(' ', current)) != string::npos){
        res.push_back(string(str, current, found - current));
        current = found + 1;
    }
    res.push_back(string(str, current, str.size() - current));
    return res;
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

void spatdif_interpret(t_spatdif *x, t_symbol *s, int argc, t_atom *argv)
{
    int i, count = 0, rargc = 0, argCount = 0, ttCount = 1;
    const char* typetags;
    vector<string> addressVector, elementVector;

    t_atom *rargv;
    
    string str, returnedStr;
    str = s->s_name;
    if(str == "dump"){
        
        post(x->scene.dump().c_str());
        return;
        
    }
    
    for ( i = 0; i < argc; i++)
    {
        str += ' ';
        if (argv[i].a_type == A_FLOAT)
            str += doubleToString((double)(argv[i].a_w.w_float));
        else if (argv[i].a_type == A_SYMBOL)
            str += argv[i].a_w.w_symbol->s_name;
    }

    returnedStr = x->responder.forwardOSCMessage(str);
    
    elementVector = splitMessage(returnedStr);
    addressVector = splitAddress(elementVector[0]);
    if(elementVector.size() > 1){
        typetags = elementVector[1].c_str();
    }
    
    count = elementVector.size() + addressVector.size() -1; // avoid counting twice
    if(elementVector.size() > 1){ // with arguments
        count -= 1; // type tag
    }
    count -= 2; // ommit spatdif +  the first element is not counted as an argument
    
    if(count < 0) count = 0;
    
    rargv = (t_atom*)malloc(sizeof(t_atom) * count);
    
    vector<string>::iterator it = addressVector.begin() + 2; // skip spatdif header and second element
    while (it != addressVector.end()) {
        string element = *it;
        SETSYMBOL(&rargv[argCount], gensym(element.c_str()));
        it++;argCount++;
    }
    
    if (elementVector.size() > 1) { // with arguments
        
        it = elementVector.begin() + 2;
        while (it != elementVector.end()) {
            string tmpStr = *it;
            switch (typetags[ttCount]) {
                case 'i':
                case 'f':
                case 'd':
                    SETFLOAT(&rargv[argCount], (float)(stringToDouble(tmpStr)));
                    break;
                case 's':
                    SETSYMBOL(&rargv[argCount], gensym(tmpStr.c_str()));
                    break;
            }
            it++; argCount++; ttCount++;
        }
    }

    outlet_anything(x->m_out, gensym(addressVector[1].c_str()), count, rargv);
    if(rargv){
        free(rargv);
    }
    outlet_bang(x->b_out);
}

void *spatdif_new(void)
{
    t_spatdif *x = (t_spatdif *)pd_new(spatdif_class);
    x->m_out = outlet_new(&x->x_obj, gensym("anything"));
    x->b_out = outlet_new(&x->x_obj, gensym("bang"));
    
    x->responder.setScene(&x->scene);
    return (void *)x;
}

void spatdif_setup(void)
{
    spatdif_class = class_new(gensym("spatdif"), (t_newmethod)spatdif_new,
    	0, sizeof(t_spatdif), 0, A_GIMME, 0);
    class_addanything(spatdif_class, (t_method)spatdif_interpret);
}

