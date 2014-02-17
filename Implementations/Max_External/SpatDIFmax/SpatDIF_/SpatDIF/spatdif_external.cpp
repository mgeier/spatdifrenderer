/**

 
*/

#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object

#include "sdMain.h"

typedef struct _spatdif 
{
	t_object ob;			// the object itself (must be first)
    
    
    
} t_spatdif;

//// standard set
void *spatdif_new(t_symbol *s, long argc, t_atom *argv);
void spatdif_free(t_spatdif *x);
void spatdif_assist(t_spatdif *x, void *b, long m, long a, char *s);

void spatdif_read(t_spatdif *x, t_symbol *s);
void spatdif_doread(t_spatdif *x, t_symbol *s, short ac, t_atom *av);

void *spatdif_class;


int C74_EXPORT main(void)
{	
	t_class *c;
	
	c = class_new("spatdif", (method)spatdif_new, (method)spatdif_free, (long)sizeof(t_spatdif), 0L, A_GIMME, 0);
	
	/* you CAN'T call this from the patcher */
    class_addmethod(c, (method)spatdif_assist,			"assist",		A_CANT, 0);
    class_addmethod(c, (method)spatdif_read,            "read",         A_DEFSYM, 0);

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
    
}

void *spatdif_new(t_symbol *s, long argc, t_atom *argv)
{
	t_spatdif *x = NULL;

	// object instantiation, NEW STYLE
	if ((x = (t_spatdif *)object_alloc((t_class *)spatdif_class))) {

        
        
        
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
	short       path_id;
	t_fourcc	outtype;
	char        *buffer;
	t_ptr_size	size;
	
	if (!s->s_name[0]) {
		if(open_dialog(filename, &path_id, &outtype, 0L, 0)) return;
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
	
	sysfile_geteof(filehandle, &size);			// allocate memory
	buffer = sysmem_newptr(size);
	sysfile_read(filehandle, &size, buffer); 	// read in the file
	
//	new_node = datatree_from_xml(buffer);		// build scene from data in buffer
    
//	if(!node) {
//		object_error((t_object *)x, "\"%s\" contains no readable ambidecode-coefs data", filename);
//		sysfile_close(filehandle);						// close, dealloc, etc
//		sysmem_freeptr(buffer);
//		return;
//	}
    
	sysfile_close(filehandle);						// close, dealloc, etc
    sysmem_freeptr(buffer);
    
}
