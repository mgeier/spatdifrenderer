//
//  sdScene.h
//

#ifndef ____sdScene__
#define ____sdScene__

#include <string>
#include <vector>
#include "sdConst.h"
#include "sdInfo.h"
#include "sdEntityCore.h"
#include "sdEntityExtension.h"

using namespace std;

/*! is responsible for 
 - adding, removing, and maintaining sdEntityCores.
 - attaching and detaching extensions to the exisiting and newly instantiated cores
 */
class sdScene {

private:
    /*! a vector of sdEntityCores */
    vector <sdEntityCore*> entityVector;
    
    /*! a vector of activated extensions. A sdScene automatically instantiates the descendants of sdEntityExtensions and attached to a sdEntityCore, when it is instantiated. */
    vector <EExtension> activatedExtensionVector;
    
    /*! ordering flag */
    EOrdering ordering;
    
    /*! contains "info" part of the meta section*/
    sdInfo info;


public:
    
    /*! @name Constructors
     @{
     */
    
    /*! constructor */
    sdScene(void);
    
    /*! constructor with sdInfo  */
    sdScene(sdInfo info);
    
    /*! constructor with sdInfo and ordering*/
    sdScene(sdInfo info, EOrdering ordering);
    
    /*!
     @}
     */
    
    /*! @name Meta data handing
     @{
     */
    
    /*! sets an instance of sdInfo, which contains the "info" part of the meta section 
     @param info a sdInfo
     */
    void setInfo(sdInfo info);
    
    /*! gets an instance of sdInfo, which contains the "info" part of the meta section
     @param info a sdInfo
     */
    sdInfo getInfo(void);

    /*! 
     gets an enum  ordering
     */
    EOrdering getOrdering(void);

    /*!
     gets ordering as String
     */
    string getOrderingAsString(void);
    
    /*! sets an enum of ordering
     @param ordering the ordering enum declared in sdConst.h
     */
    void setOrdering(EOrdering ordering);
    
    /*!
     @}
     */
    
    /*! @name Entity handling
     @{
     */
    
    /*! return name of the entity at the specified index
     @param index
     */
    string getEntityName(int index);
    
    /*! search an entity in the entity vector by its name and return the pointer. returns null if the entity can not be found.
     @param name the name of a designated entity
     */
    sdEntityCore* getEntity(string name);
    
    /*! returns a pointer to the entityVector */
    vector <sdEntityCore*> getEntityVector(void);
    
    /*! returns the number of entity in the entityVector*/
    unsigned int getNumberOfEntities(void);
    
    /*! add a sdEntityCore to this scene. this function is the only way to generate an instance of sdEntityCore because the constructor of sdEntityCore is intentionally declared as a private function. Thus, the constcutor can be called only by its friend i.e. this class. This prevent users to create entities not attached to any sdScene. Thus, all sdSceneCore are always aware of the list of activated extensions.this function returns a pointer to the new instance of sdEntityCore.
        @param name the name of new sdEntityCore
     */
    sdEntityCore* addEntity(string name, EKind kind = SD_SOURCE);
    
    /*! remove the sdEntityCore specified by name from the entityVector]
     @param name the name of a sdEntityCore to be removed from the entityVector
     */
    void removeEntity(string name);
    
    /*! remove the entity refered by the pointer from the entityVector
     @param entityCore a pointer to a sdEntityCore to be removed from the entityVector
     */
    void removeEntity(sdEntityCore *entityCore);
   
    /*! remove all entities from the entityVector*/
    void removeAllEntities(void);
    
    /*!
     @}
     */
    
    /*! @name Value handling
     @{
     */
    
    /*! set timed data by specifying name of the target entity, descriptor, and time.
     sdScene forwards the request to a proper sdEntityCore. 
     a properly allocated void pointer should be provided as the fourth argument
     @param name name of target eneity
     @param time time of the newly added event
     @param descriptor enum of target descriptor
     @param value allocated void pointer containing value(s)
     */
    void setValue(string name, double time, EDescriptor descriptor,  void* value);
    
    /*! query timed data by specifying name of the target entity, descriptor, and time.
     sdScene forwards the query to a proper sdEntityCore and return the answer.
     a properly allocated void pointer should be provided as the fourth argument
     @param name  name of target eneity
     @param descriptor enum of target descriptor
     @param value allocated void pointer containing value(s)
     */
    void* getValue(string name, double time, EDescriptor descriptor);
    
    /*!
     @}
     */
    
    /*! @name Extension handling
     @{
     */
    
    /*! return an enum EExtension at the specified index */
    EExtension getActivatedExtension(int index);
    
    /*! returns number of activated Extensions*/
    int getNumberOfActivatedExtensions(void);
     
    /*! activate an extension specified by enum EExtension. This function instantiates instances of the designated extension (i.e. a subclass of sdEntityExtension ) and attached them to all existing sdEntityCores in the scene. The added extensions will be also attached to all newly instantiated sdEntityCores.
     @param extension enum EExtension of extension to be added
     */
    void addExtension(EExtension extension);
    
    /*! activate an extension specified by a string. 
     @param extension string of extension to be added
     */
    void addExtension(string extension);

    /*! check if the specified extension is activated
     @param extension enum EExtension of extension to be checked
     */
    bool isExtensionActivated(EExtension extension);

    
    /*! deactivate an extension specified by enum EExtension. This function removes instances of the designated extension (i.e. a subclass of sdEntityExtension )  attached  to all existing sdEntityCores in the scene.
        Thus, all events data stored in the extension will be lost.
     @param extension enum EExtension of extension to be removed
     */
    void removeExtension(EExtension extension);
    
    /*! deactivate an extension specified by a string
     @param extension string of extension to be removed
     */
    void removeExtension(string extension);

    /*! 
     remove all exntesions from the extensionVector and sdEntityCores in the scene.
     The data stored in the instances of sdEntityExtension will be lost.
     */
    void removeAllExtensions(void);

    /*!
     @}
     */
    
    /*! @name Varification
     @{
     */
    
    /*! varifies the content of scene by sending all containing data (meta and timed) on the standard output. It is useful for debugging the scene*/
    void dump(void);
    
    /*!
     @}
     */
};

/*** inline implementations ***/

inline sdScene::sdScene(void){
    sdInfo info(string("unknown"), string("unknown"), sdDate(), string("unknown"), string("unknown"), string("unknown"));
    ordering = SD_TIME;
}

inline sdScene::sdScene(sdInfo info){
    setInfo(info);
    ordering = SD_TIME;
}

inline sdScene::sdScene(sdInfo info, EOrdering ordering){
    setInfo(info);
    sdScene::ordering = ordering;
}

inline void sdScene::setInfo(sdInfo info){
    sdScene::info = info;
}

inline sdInfo sdScene::getInfo(void){
    return info;
}

inline vector <sdEntityCore*> sdScene::getEntityVector(void){
    return entityVector;
}

inline EOrdering sdScene::getOrdering(void){
    return ordering;
}

inline string sdScene::getOrderingAsString(void){
    return ordering == SD_TIME ? string("time") : string("track");
}

inline void sdScene::setOrdering(EOrdering ordering ){
    sdScene::ordering = ordering;
}
#endif /* defined(____sdScene__) */
