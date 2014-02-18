

#include "m_pd.h"
#include "sdMain.h"
 
typedef struct spatdif
{
    t_object x_obj;
    t_outlet *m_out, *b_out;
    sdScene scene;
    double writeTime;
    double queryTime;
} t_spatdif;

t_class *spatdif_class;

extern "C" {
    #include <string.h>
    void spatdif_setup(void);
    void spatdif_interpret(t_spatdif *x, t_symbol *s, int argc, t_atom *argv);
}


void spatdif_interpret(t_spatdif *x, t_symbol *s, int argc, t_atom *argv)
{
    int num_elements, cmd_length, i;
    char **commands,  *command;
    
    command = s->s_name;
    
    for ( i = 0; i < argc; i++)
    {
        if (argv[i].a_type == A_FLOAT)
            post("float: %f", argv[i].a_w.w_float);
        else if (argv[i].a_type == A_SYMBOL)
            post("symbol: %s", argv[i].a_w.w_symbol->s_name);
    }

}

void *spatdif_new(t_symbol *selector, int argcount, t_atom *argvec)
{
    t_spatdif *x = (t_spatdif *)pd_new(spatdif_class);
    x->m_out = outlet_new(&x->x_obj, gensym("anything"));
    x->b_out = outlet_new(&x->x_obj, gensym("bang"));
    return (void *)x;
}

void spatdif_setup(void)
{

    spatdif_class = class_new(gensym("spatdif"), (t_newmethod)spatdif_new,
    	0, sizeof(t_spatdif), 0, A_GIMME, 0);
    class_addanything(spatdif_class, (t_method)spatdif_interpret);


}

