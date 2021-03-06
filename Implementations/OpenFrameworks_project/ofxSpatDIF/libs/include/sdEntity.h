
#ifndef ____sdEntity__
#define ____sdEntity__

#include <set>
#include <string>
#include <iostream>
#include "sdConst.h"

using namespace std;

/*!
 A pure virtual class of event. This class maintains data of time, descriptor, and value and responsible for converting incoming and outgoing data.
 */

class sdEvent{
    
protected:
    double time;/*!< time of the event */
    EDescriptor descriptor;/*!<descriptor of the event*/
    void *value;/*!<value of the event. the subclasses must allocates this void pointer */
    
public:
    /*! @name Setter
     @{ */
    
    /*!
     set all three parameters at once
     @param time time of event
     @param desciptor desciprot of event declared in sdConst.h
     @param value value of event
     */
    void set(double time, EDescriptor descriptor, void* value);
    
    /*! @} */

    /*! @name Time handling
     @{ */
    
    /*! set time of the event
     @param time time of the event */
    void setTime(double time);
    
    /*! set time of the event
     @param time time of the event as String */
    void setTime(string timeString);
    
    /*!returns time as double*/
    double getTime(void);
    
    /*!returns time as string*/
    string getTimeAsString(void);
    
    /*! @} */

    /*! @name Value handling
     @{ */
    
    /*! descriptor and value should be set at once. because this function allocates memory to void* value based on the descriptor*/
    virtual bool setValue(EDescriptor descriptor, void* value) = 0;
    
    /*!overloaded function*/
    virtual bool setValue(string descriptor, string value) = 0;
    
    /*!returns descriptor as Enum*/
    EDescriptor getDescriptor(void);
    
    /*!returns descriptor as string. a routine for conversion should be implemented in derived classes*/
    virtual string getDescriptorAsString(void) = 0;
    
    /*!returns value through void pointer. It should be casted to propoer type manually.*/
    void* getValue(void);

    /*!returns value as string. a routine for conversion should be implemented in derived classes */
    virtual string getValueAsString(void) = 0;

    /*! @} */
};


/*!
 a helper class for sorting events
 */
class sdEventCompare{
public:
    bool operator()(sdEvent* e1, sdEvent* e2){
        if(e1->getTime() < e2->getTime()) // sort by time
            return true;
        else
            return false;
    }
};

/*! sdEntity
 sdEntity is a pure abstract class. This class maintains and handles all events associated to relevant descriptors. This class is also responsible for answering queries about it's relevant descriptors.
*/
class sdEntity{

protected:
    multiset<sdEvent*, sdEventCompare> eventSet; /*!< maintains pointers to all relevant sdEvents */

public:
    /*!< desctructor. this function erases all contents of eventSet. 
     Subsequently, all allocated buffers stored in the eventSet will be destroyed*/
    ~sdEntity();
    
    /*! @name EventHandling
     @{ */
    
    /*! return a multiset of sdEvent pointers. To get events in the specific timeframe, use start and end arguments.*/

    multiset<sdEvent*, sdEventCompare> getEventSet(void);
    multiset<sdEvent*, sdEventCompare> getEventSet(double start, double end);
    multiset<sdEvent*, sdEventCompare> getEventSet(double start, double end, EDescriptor descriptor);

    /*! return number of registerd events in the eventSet*/
    int getNumberOfEvents(void);
    
    /*!
     return all events related to the given descriptor
     @param time the time of the event in second
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    sdEvent* getEvent(double time, EDescriptor descriptor);
    
    /*!
     return the very first event with the specified descriptor.
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    sdEvent* getFirstEvent(EDescriptor descriptor);
    
    /*!
     return the time tag of the first event
     */
    double getFirstTimeTag();
    
    /*!
     return the timeTag of the very last event with the specified descriptor.
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    sdEvent* getLastEvent(EDescriptor descriptor);

    /*!
     return the time tag of the last event
     */
    double getLastTimeTag();

    
    
    /*!
     return next event from the given time index that holds the specified descriptor.
     @param time index
     @param descriptor the descriptor of the event declared in sdConst.h
     */
    virtual sdEvent* getNextEvent(double time, EDescriptor descriptor);
    
    /*!
     this function is the only way to instantiate sdEvent.
     */
    virtual sdEvent* addEvent(double time, EDescriptor descriptor, void* value) = 0;
    virtual sdEvent* addEvent(string time, string descriptor, string value) = 0;
    
    /*<
     remove an event from the set
     @param event the event to be removed
     */
    void removeEvent(sdEvent *event);
    
    /*!
     remove an event at the specified time and descriptor
     @param time the time of sdEvent to be removed.
     @param descriptor the descriptor of sdEvent to be removed
     */
    void removeEvent(double time, EDescriptor descriptor);
    virtual void removeEvent(string time, string descriptor) = 0;
    
    /*!
     remove all events in the eventSet
     */
    void removeAllEvents();
    
    /*!
     @}
     */
    
    /*! @name Getting value directly
     @{
     */
    /*!
     query method. it simply returns a pointer to the buffer, where the designated data are stored. returns null if not found. 
     @param time 
     @param descriptor specify descriptor defined in sdConst.h
     */
    void* getValue(double time, EDescriptor descriptor);
    /*!
     @} */

};


#endif /* defined(____sdEntity__) */

