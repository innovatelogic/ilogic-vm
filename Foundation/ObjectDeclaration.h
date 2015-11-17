//----------------------------------------------------------------------------------------------
// OpenES: Open Entertainment System
// Copyright (C) 2010  Yura Gunko email: yura.gunko@gmail.com
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------------------------

#ifndef __h_declaration__
#define __h_declaration__

#ifdef _WIN32
#pragma once
#endif

//----------------------------------------------------------------------------------------------
// class factory registration
//----------------------------------------------------------------------------------------------
#define REGISTER_CLASS(CLASS, BASE_CLASS)\
        REGISTER_CLASS_FACTORY_A(CLASS)\
		CLASS_INSTANCE(CLASS, BASE_CLASS)

#define REGISTER_CLASS_A(CLASS, BASE_CLASS)\
		REGISTER_CLASS_FACTORY_A(CLASS)\
		BEGIN_AUTO_CLASS(CLASS)

#define END_REGISTER_CLASS(CLASS, BASE_CLASS)\
		END_AUTO_CLASS(CLASS)\
		CLASS_INSTANCE_EX(CLASS, BASE_CLASS)

#define REGISTER_CLASS_PURE(CLASS, BASE_CLASS)\
		BEGIN_AUTO_CLASS(CLASS)

#define END_REGISTER_CLASS_PURE(CLASS, BASE_CLASS)\
		END_AUTO_CLASS(CLASS)\
		CLASS_INSTANCE_PURE(CLASS, BASE_CLASS)

#define REGISTER_CLASS_PURE_NOBASE(CLASS)\
		BEGIN_AUTO_CLASS(CLASS)

#define END_REGISTER_CLASS_PURE_NOBASE(CLASS)\
		END_AUTO_CLASS(CLASS)\
		CLASS_INSTANCE_PURE_NOBASE(CLASS)

//----------------------------------------------------------------------------------------------
#define REGISTER_CLASS_ACTION(CLASS, BASE_CLASS)\
		REGISTER_FUNC_ACTION(CLASS, BASE_CLASS)\
		CLASS_INSTANCE(CLASS, BASE_CLASS)

//----------------------------------------------------------------------------------------------
#define REGISTER_CLASS_ACTION_EX(CLASS, BASE_CLASS)\
		REGISTER_FUNC_ACTION(CLASS, BASE_CLASS)\
		BEGIN_AUTO_CLASS(CLASS)

#define END_REGISTER_CLASS_ACTION(CLASS, BASE_CLASS)\
		END_AUTO_CLASS(CLASS)\
		CLASS_INSTANCE_EX(CLASS, BASE_CLASS)

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
#define REGISTER_CLASS_FACTORY_A(CLASS)\
static CObjectAbstract* Generator##CLASS(const char *Name, const CObjectAbstract *pParent)\
{\
	CObjectAbstract *pObject = new CLASS(pParent);\
	if (pObject)\
	{\
		pObject->SetName(Name);\
		pObject->SetType(#CLASS);\
		pObject->SetTypeId(NObjectFactory::GetId(#CLASS));\
		pObject->FinishGeneration();\
		return pObject;\
	}\
	return nullptr;\
}\
static CObjectAbstract* CopyGenerator##CLASS(const CObjectAbstract *Source, const CObjectAbstract *pParent)\
{\
	CObjectAbstract *pObject = new CLASS(*static_cast<const CLASS*>(Source));\
	CActor *pAParent = const_cast<CActor*>(static_cast<const CActor*>(pParent));\
	if (pAParent != nullptr && static_cast<const CActor*>(pObject))\
	{\
		const_cast<CActor*>(static_cast<const CActor*>(pObject))->SetParent(pAParent);\
		pAParent->AddChildNode(const_cast<CActor*>(static_cast<const CActor*>(pObject)));\
		pObject->FinishGeneration();\
	}\
	return pObject;\
}

//----------------------------------------------------------------------------------------------
#define BEGIN_PROPS_DECLARATION(CLASS)\
const Property_Base* Arr[] = {


//----------------------------------------------------------------------------------------------
#define ASSEMBLE_PROPS(CLASS)\
};\
	if (ARRAY_COUNT_SIZE(Arr))\
{\
	m_nSize = ARRAY_COUNT_SIZE(Arr);\
	m_pProps = new const Property_Base*[m_nSize];\
	for (unsigned int Index = 0; Index < m_nSize; ++Index)\
	{\
		*(m_pProps + Index) = *(Arr + Index);\
	}\
}

//----------------------------------------------------------------------------------------------
#define BEGIN_AUTO_CLASS(CLASS)\
class AutoRegisterProps##CLASS : public IPropertiesAllocator\
{\
public:\
	AutoRegisterProps##CLASS()\
	: m_pProps(0)\
	, m_nSize(0)\
	, m_pInterfaces(0)\
	, m_nIntfSize(0)\
	{\
		BEGIN_PROPS_DECLARATION(CLASS)
		

//----------------------------------------------------------------------------------------------
#define BEGIN_INTERFACE_DECLARATION(CLASS)\
const SInterfaceDecl* ArrIntf[] = {


//----------------------------------------------------------------------------------------------
#define ASSEMBLE_INTERFACES(CLASS)\
};\
if (ARRAY_COUNT_SIZE(ArrIntf))\
{\
	m_nIntfSize = ARRAY_COUNT_SIZE(ArrIntf);\
	m_pInterfaces = new const SInterfaceDecl*[m_nIntfSize];\
	for (unsigned int Index = 0; Index < m_nIntfSize; ++Index)\
	{\
		*(m_pInterfaces + Index) = *(ArrIntf + Index);\
	}\
}

//----------------------------------------------------------------------------------------------
#define END_AUTO_CLASS(CLASS)\
ASSEMBLE_PROPS(CLASS)\
CLASS_ENDL(CLASS)

//----------------------------------------------------------------------------------------------
#define CLASS_ENDL(CLASS)\
	}\
	virtual const Property_Base** GetProps() const { return m_pProps; }\
	virtual unsigned int GetSize() const { return m_nSize; }\
	virtual const SInterfaceDecl** GetInterfaces() const { return m_pInterfaces; }\
	virtual unsigned int GetIntfSize() const { return m_nIntfSize; }\
private:\
	const Property_Base **m_pProps;\
	unsigned int m_nSize;\
	const SInterfaceDecl **m_pInterfaces;\
	unsigned int m_nIntfSize;\
};

//----------------------------------------------------------------------------------------------
#define CLASS_INSTANCE(CLASS, BASE_CLASS)\
namespace {\
volatile static NObjectFactory::CAuto<CLASS, BASE_CLASS> ClassRegistration##CLASS(#CLASS,\
	Generator##CLASS,\
	CopyGenerator##CLASS,\
	CLASS::ThisClassName(),\
	CLASS::GetPanelBaseClassName());\
}

//----------------------------------------------------------------------------------------------
#define CLASS_INSTANCE_INTERFACE(CLASS)\
namespace\
{\
	volatile static NObjectFactory::CAutoInterface<CLASS, AutoRegisterProps##CLASS> ClassRegistration##CLASS(#CLASS);\
}

//----------------------------------------------------------------------------------------------
#define CLASS_INSTANCE_EX(CLASS, BASE_CLASS)\
namespace\
{\
volatile static NObjectFactory::CAutoEx<CLASS, BASE_CLASS, AutoRegisterProps##CLASS> ClassRegistration##CLASS(#CLASS,\
									 Generator##CLASS,\
									 CopyGenerator##CLASS,\
									 CLASS::ThisClassName(),\
									 CLASS::GetPanelBaseClassName());\
}

//----------------------------------------------------------------------------------------------
#define CLASS_INSTANCE_PURE(CLASS, BASE_CLASS)\
namespace\
{\
	volatile static NObjectFactory::CAutoEx<CLASS, BASE_CLASS, AutoRegisterProps##CLASS> ClassRegistration##CLASS(#CLASS,\
	nullptr,\
	nullptr,\
	CLASS::ThisClassName(),\
	CLASS::GetPanelBaseClassName());\
}

//----------------------------------------------------------------------------------------------
#define CLASS_INSTANCE_PURE_NOBASE(CLASS)\
	namespace\
{\
	volatile static NObjectFactory::CAutoPureEx<CLASS, void, AutoRegisterProps##CLASS> ClassRegistration##CLASS(#CLASS,\
	nullptr,\
	nullptr,\
	CLASS::ThisClassName(),\
	CLASS::GetPanelBaseClassName());\
}

//----------------------------------------------------------------------------------------------
#define REGISTER_FUNC_ACTION(CLASS, BASE_CLASS)\
static CObjectAbstract * Generator##CLASS(const char *Name, const CObjectAbstract * Parent)\
{\
	CObjectAbstract * Object = new CLASS(Parent);\
	if (Object)\
	{\
		Object->SetName(Name);\
		Object->SetType(#CLASS);\
		Object->SetTypeId(NObjectFactory::GetId(#CLASS));\
		Object->FinishGeneration();\
		return Object;\
	}\
	return nullptr;\
}\
static CObjectAbstract * CopyGenerator##CLASS(const CObjectAbstract * Source, const CObjectAbstract * Parent)\
{\
	CObjectAbstract * Object = new CLASS(*static_cast<const CLASS*>(Source));\
	ActionBase * AParent = const_cast<ActionBase*>(static_cast<const ActionBase*>(Parent));\
	if (AParent != nullptr && static_cast<const ActionBase*>(Object))\
	{\
		const_cast<ActionBase*>(static_cast<const ActionBase*>(Object))->SetParent(AParent);\
		AParent->AddChildNode(const_cast<ActionBase*>(static_cast<const ActionBase*>(Object)));\
	}\
	return Object;\
}

//----------------------------------------------------------------------------------------------
// event management macro
//----------------------------------------------------------------------------------------------
/** event declaration */
#define REGISTER_EVENT_(INSTIGATOR_OBJECT, RECEIVER_OBJECT, FUNCTION, EVENT_ID)\
	assert(INSTIGATOR_OBJECT);\
{\
	bool bFind = false;\
	for (TVecConstEventsIterator Iter = INSTIGATOR_OBJECT->m_VEvents.begin(); Iter != INSTIGATOR_OBJECT->m_VEvents.end(); ++Iter)\
{\
	if (EVENT_ID == (*Iter)->EventId && RECEIVER_OBJECT == (*Iter)->m_receiverObject)\
	{\
		bFind = true;\
	}\
}\
	if (!bFind)\
{\
	EventDesc<CActor> * EventInfo = new EventDesc<CActor>();\
	assert(EventInfo);\
	EventInfo->m_receiverObject = RECEIVER_OBJECT;\
	EventInfo->EventId = EVENT_ID;\
	EventInfo->Event.connect(FUNCTION);\
	INSTIGATOR_OBJECT->m_VEvents.push_back(EventInfo);\
}\
}

//----------------------------------------------------------------------------------------------
/** event underregistration */
#define UNREGISTER_EVENT(INSTIGATOR_OBJECT, RECEIVER_OBJECT, EVENT_ID)\
assert(INSTIGATOR_OBJECT);\
for (TVecEventsIterator Iter = INSTIGATOR_OBJECT->m_VEvents.begin(); Iter != INSTIGATOR_OBJECT->m_VEvents.end(); ++Iter)\
{\
	if (EVENT_ID == (*Iter)->EventId && RECEIVER_OBJECT == (*Iter)->m_receiverObject)\
	{\
		delete (*Iter);\
		INSTIGATOR_OBJECT->m_VEvents.erase(Iter);\
		break;\
	}\
}
//----------------------------------------------------------------------------------------------
/** call event by ID */
#define CALL_EVENT_DELEGATE(EVENT_ID)\
for (TVecEventsIterator Iter = m_VEvents.begin(); Iter != m_VEvents.end(); ++Iter)\
{\
	if (EVENT_ID == (*Iter)->EventId)\
	{\
		(*Iter)->CallEvent(this);\
	}\
}

//----------------------------------------------------------------------------------------------
/** call event by ID */
#define CALL_EVENT_DELEGATE_SENDER(SENDER, EVENT_ID)\
	for (TVecEventsIterator Iter = m_VEvents.begin(); Iter != m_VEvents.end(); ++Iter)\
{\
	if (EVENT_ID == (*Iter)->EventId)\
	{\
		(*Iter)->CallEvent(SENDER);\
	}\
}

//----------------------------------------------------------------------------------------------
/** call event by ID */
#define CALL_EVENT_DELEGATE_OBJECT(OBJECT, SENDER, EVENT_ID)\
	for (TVecEventsIterator Iter = OBJECT->m_VEvents.begin(); Iter != OBJECT->m_VEvents.end(); ++Iter)\
{\
	if (EVENT_ID == (*Iter)->EventId)\
	{\
		(*Iter)->CallEvent(SENDER);\
	}\
}

//----------------------------------------------------------------------------------------------
// event proxy input management macro
//----------------------------------------------------------------------------------------------
/** event declaration */
#define REGISTER_EVENT_PROXY_INPUT(EVENT_OBJECT, RECIEVER_OBJECT, FUNCTION)\
	assert(EVENT_OBJECT);\
{\
	bool bFind = false;\
	for (TVecInputKeyEventsIterator Iter = EVENT_OBJECT->m_EventsInput.begin(); Iter != EVENT_OBJECT->m_EventsInput.end(); ++Iter)\
{\
	if (RECIEVER_OBJECT == (*Iter)->m_receiverObject)\
	{\
		bFind = true;\
	}\
}\
	if (!bFind)\
{\
	EventKeyInputProxy<IInputInterface> * EventInfo = new EventKeyInputProxy<IInputInterface>();\
	assert(EventInfo);\
	EventInfo->m_receiverObject = RECIEVER_OBJECT;\
	EventInfo->Event.connect(FUNCTION);\
	EVENT_OBJECT->m_EventsInput.push_back(EventInfo);\
}\
}

//----------------------------------------------------------------------------------------------
/** event underregistration */
#define UNREGISTER_EVENT_PROXY_INPUT(EVENT_OBJECT, RECIEVER_OBJECT)\
	assert(EVENT_OBJECT);\
	for (TVecInputKeyEventsIterator Iter = EVENT_OBJECT->m_EventsInput.begin(); Iter != EVENT_OBJECT->m_EventsInput.end(); ++Iter)\
{\
	if (RECIEVER_OBJECT == (*Iter)->m_receiverObject)\
	{\
	delete (*Iter);\
	EVENT_OBJECT->m_EventsInput.erase(Iter);\
	break;\
	}\
}

//----------------------------------------------------------------------------------------------
/** call event by ID */
#define CALL_EVENT_DELEGATE_PROXY_INPUT(PARAM)\
	for (TVecInputKeyEventsIterator Iter = m_EventsInput.begin(); Iter != m_EventsInput.end(); ++Iter)\
{\
	(*Iter)->Execute(this, PARAM);\
}

//----------------------------------------------------------------------------------------------
#define DESERIALIZE_OBJECT(OBJECT_CLASS)\
	DESERIALIZE_OBJECT_BY_TYPENAME(#OBJECT_CLASS)

//----------------------------------------------------------------------------------------------
#define DESERIALIZE_OBJECT_BY_TYPENAME(TYPE_NAME, TYPE_FAMILY_BASE)\
	DESERIALIZE_OBJECT_(TYPE_NAME, TYPE_FAMILY_BASE, this)

//----------------------------------------------------------------------------------------------
#define DESERIALIZE_OBJECT_(TYPE_NAME, TYPE_FAMILY_BASE, PARENT)\
	XML_CHECK_NODE(TYPE_NAME)\
{\
	XML_DEF_ATTRIBUTES_NODE(External);\
	XML_DEF_ATTRIBUTES_NODE(Transient);\
	XML_DEF_ATTRIBUTES_NODE(Value);\
	ValueParser ValueStore(Value);\
	if (Transient == "0" || Transient == "" || External != "")\
	{\
		TYPE_FAMILY_BASE *pNewObject = static_cast<TYPE_FAMILY_BASE*>(GENERATE_OBJECT_SDK(TYPE_NAME, ValueStore.GetValueString("Name").c_str(), PARENT));\
		if (pNewObject)\
		{\
			pNewObject->SetPlainDeserializationPos(PlainElementIndex++);\
			PARENT->AddChildNode(pNewObject);\
			if (External == "")\
			{\
				pNewObject->SuperDeserializer(XML_CURRENT_NODE);\
			}\
			else\
			{\
				pNewObject->SuperDeserializerExternal(XML_CURRENT_NODE);\
				pNewObject->LoadFromFile(External.c_str(), true);\
			}\
		}\
	}\
	else\
	{\
		for (size_t Index = 0; Index < PARENT->m_ChildNodes.size(); ++Index)\
		{\
			if (PARENT->m_ChildNodes[Index]->IsTransient() &&\
				PARENT->m_ChildNodes[Index]->GetName() == ValueStore.GetValueString("Name"))\
			{\
				PARENT->m_ChildNodes[Index]->SetPlainDeserializationPos(PlainElementIndex++);\
				PARENT->m_ChildNodes[Index]->SuperDeserializer(XML_CURRENT_NODE);\
				break;\
			}\
		}\
	}\
}\

#endif //__h_declaration__ 
