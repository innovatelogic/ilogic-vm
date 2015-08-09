#include "EventDesc.h"
#include "Actor.h"


//----------------------------------------------------------------------------------------------
// EventAbstract::EventAbstract()
// : ReceiverObject(NULL)
// , EventId(Event_None)
// {
// 
// }
// //----------------------------------------------------------------------------------------------
// EventAbstract::~EventAbstract()
// {
// 
// }

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
// EventDesc::EventDesc()
// {
// 
// }
// //----------------------------------------------------------------------------------------------
// EventDesc::~EventDesc()
// {
// 	Event.disconnect(ReceiverObject);
// 	ReceiverObject = NULL;
// 	EventId = Event_None;
// }
//----------------------------------------------------------------------------------------------
// void EventDesc::CallEvent(const CActor * Sender)
// {
// 	Event(Sender, EventId);
// }

//----------------------------------------------------------------------------------------------
// EventKeyInputProxy::EventKeyInputProxy()
// {
// 
// }
// 
// //----------------------------------------------------------------------------------------------
// EventKeyInputProxy::~EventKeyInputProxy()
// {
// 
// }
// 
// //----------------------------------------------------------------------------------------------
// void EventKeyInputProxy::Execute(const CActor * Sender, const EventInput& InputData)
// {
// 	Event(Sender, InputData);
// }

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
template<class T> class DerivedClass;

std::vector< DerivedClass<int>* > VecTest;

template<class T> class BaseClass
{
public:
	T *Value;
};

template<class T> class DerivedClass : public BaseClass<T>
{
public:

};

