//
//  sdConstant.h
//

#ifndef ____sdConstant__
#define ____sdConstant__

#include <string>
#include <iostream>
#include <sstream>
using namespace std;

/*!
    enum for descriptor. internally all descriptors are handled with this Enum
 */
typedef enum {
    SD_TYPE ,
    SD_PRESENT,
    SD_POSITION,
    SD_ORIENTATION,
    
    //descriptor for media extension
    SD_MEDIA_ID,
    SD_MEDIA_TYPE,
    SD_MEDIA_LOCATION,
    SD_MEDIA_CHANNEL,
    SD_MEDIA_TIME_OFFSET,
    SD_MEDIA_GAIN,
    
    SD_ERROR
} EDescriptor;

/*!
 enum for "type" descriptor.
 Currently only "point" is declared in the specification.
 */
typedef enum{
    SD_POINT
} EType;

/*!
 enum for ordering
 */
typedef enum {
    SD_TIME,
    SD_TRACK
} EOrdering;

/*!
 enum for kind. All sdEntityCore must define their kind when instantiated.
 */
typedef enum {
    SD_SOURCE,
    SD_SINK
    
} EKind;

/*!
 enum for extension. all sdEntityExtension must have one of these enum as a static variable in order to identify themselves
 */
typedef enum {
    SD_CORE,
    SD_MEDIA,
    SD_INTERPOLATION,
    SD_DIRECT_TO_ONE,
    SD_EXTENSION_ERROR
} EExtension;


/*!
 utility function that convert ints to a string
 */
inline string intsToString(int *it, int num){
    ostringstream os;
    for(int i=0;i<num;i++){
        os << it[i];
        if(i != (num-1))
            os << ' ';
    }
    return os.str();
}

/*!
 utility function that convert a int to a string
 */
inline string intToString(int it){
    return intsToString(&it, 1);
}


/*! 
 utility function that convert doubles to a string
 */
inline string doublesToString(double *db, int num){
    ostringstream os;
    for(int i=0;i<num;i++){
        os << db[i];
        if(i != (num-1))
            os << ' ';
    }
    return os.str();
}

/*!
 utility function that convert a double to a string
 */
inline string doubleToString(double db){
    return doublesToString(&db, 1);
}

/*!
 utility function that convert a bool to a string
 */
inline string boolToString(bool bl){
    return bl ? string("true") : string("false");
}

/*!
 utility function that convert a string to a bool
 */
inline bool stringToBool(string str){
    return str == "true" ? true : false;
}



/*!
 utility function that convert a string to doubles
 */
inline double *stringToDoubles(string str, double *db, int num){
    istringstream is(str);
    for(int i = 0; i < num; i++) {
        is >> db[i];
    }
    return db;
}

/*!
 utility function that convert a string to a double
 */
inline double stringToDouble(string str){
    double db;
    stringToDoubles(str, &db, 1);
    return db;
}

/*!
 utility function that converts a string to a int
 */
inline int *stringToInts(string str, int *it, int num){
    istringstream is(str);
    for(int i = 0; i < num; i++) {
        is >> it[i];
    }
    return it;
}

/*!
 utility function that convert int to a string
 */
inline int stringToInt(string str){
    int it;
    stringToInts(str, &it, 1);
    return it;
}

/*!
 utility function that converts a string to a descriptor. 
 */
inline const EDescriptor stringToDescriptor(string str, const string *ds, const EDescriptor *dc, const int num){
    for(int i = 0; i<num; i++ ){
        if(ds[i] == str)
            return dc[i];
    }
    return SD_ERROR;
}

/*!
 utility function that converts a descriptor to a string.
 */
inline const string descriptorToString(EDescriptor descriptor, const string *ds, const EDescriptor *dc, const int num){
    for(int i = 0; i<num; i++ ){
        if(dc[i] == descriptor)
            return ds[i];
    }
    return NULL;
}

inline const string extensionToString(EExtension extension){
    string extensionStr;
    if(extension == SD_MEDIA){
        extensionStr = "media";
    }else if(extension == SD_INTERPOLATION){
        extensionStr = "interpolation";
    }else if(extension == SD_DIRECT_TO_ONE){
        extensionStr = "directo-to-one";
    }else if(extension == SD_CORE){
        extensionStr = "core";
    }else{
        extensionStr = "error";
    }
    return extensionStr;
}

inline const EExtension stringToExtension(string extensionStr){
    EExtension ext;
    if(extensionStr == "media"){
        ext = SD_MEDIA;
    }else if(extensionStr == "interpolation"){
        ext = SD_INTERPOLATION;
    }else if(extensionStr == "direct-to-one"){
        ext = SD_DIRECT_TO_ONE;
    }else{
        ext = SD_EXTENSION_ERROR;
    }
    return ext;
}

inline const EExtension getRelevantExtension(EDescriptor descriptor){
    if(SD_TYPE <= descriptor && descriptor <= SD_ORIENTATION){
        return SD_CORE;
    }else if(SD_MEDIA_ID <= descriptor && descriptor <= SD_MEDIA_GAIN){
        return SD_MEDIA;
    }else{
        return SD_EXTENSION_ERROR;
    }
}

#endif