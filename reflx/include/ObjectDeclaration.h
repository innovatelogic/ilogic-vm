#pragma once

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
		END_AUTO_CLASS()\
		CLASS_INSTANCE_EX(CLASS, BASE_CLASS)

#define REGISTER_CLASS_PURE(CLASS, BASE_CLASS)\
		BEGIN_AUTO_CLASS(CLASS)

#define END_REGISTER_CLASS_PURE(CLASS, BASE_CLASS)\
		END_AUTO_CLASS()\
		CLASS_INSTANCE_PURE(CLASS, BASE_CLASS)

#define END_REGISTER_CLASS_PURE_()\
		END_AUTO_CLASS()\
		CLASS_INSTANCE_PURE(CLASS, BASE_CLASS)

#define REGISTER_CLASS_PURE_NOBASE(CLASS)\
		BEGIN_AUTO_CLASS(CLASS)

#define END_REGISTER_CLASS_PURE_NOBASE(CLASS)\
		END_AUTO_CLASS()\
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
static IObjectAbstract* Generator##CLASS(const char *Name, const IObjectAbstract *pParent)\
{\
	IObjectAbstract *pObject = new CLASS(static_cast<const CObjectAbstract*>(pParent));\
	if (pObject)\
	{\
		pObject->SetName(Name);\
		pObject->SetType(#CLASS);\
		pObject->SetTypeId(oes::rflex::GetId(#CLASS));\
		pObject->FinishGeneration();\
		return pObject;\
	}\
	return nullptr;\
}\
static IObjectAbstract* CopyGenerator##CLASS(const IObjectAbstract *Source, const IObjectAbstract *pParent)\
{\
	return nullptr;\
}

//----------------------------------------------------------------------------------------------
#define BEGIN_PROPS_DECLARATION(CLASS)\
const oes::rflex::Property_Base* Arr[] = {


//----------------------------------------------------------------------------------------------
#define ASSEMBLE_PROPS()\
};\
const size_t count = ARRAY_COUNT_SIZE(Arr);\
if (count)\
{\
	m_nSize = count;\
	m_pProps = new const oes::rflex::Property_Base*[m_nSize];\
	for (unsigned int index = 0; index < m_nSize; ++index)\
	{\
		*(m_pProps + index) = *(Arr + index);\
	}\
}

//----------------------------------------------------------------------------------------------
#define BEGIN_AUTO_CLASS(CLASS)\
class AutoRegisterProps##CLASS : public oes::rflex::IPropertiesAllocator\
{\
public:\
	AutoRegisterProps##CLASS()\
	: m_pProps(nullptr)\
	, m_nSize(0)\
	, m_pInterfaces(nullptr)\
	, m_nIntfSize(0)\
	{\
		BEGIN_PROPS_DECLARATION(CLASS)
		

//----------------------------------------------------------------------------------------------
#define BEGIN_INTERFACE_DECLARATION(CLASS)\
const oes::rflex::SInterfaceDecl* ArrIntf[] = {


//----------------------------------------------------------------------------------------------
#define ASSEMBLE_INTERFACES(CLASS)\
};\
if (ARRAY_COUNT_SIZE(ArrIntf))\
{\
	m_nIntfSize = ARRAY_COUNT_SIZE(ArrIntf);\
	m_pInterfaces = new const oes::rflex::SInterfaceDecl*[m_nIntfSize];\
	for (unsigned int index = 0; index < m_nIntfSize; ++index)\
	{\
		*(m_pInterfaces + index) = *(ArrIntf + index);\
	}\
}

//----------------------------------------------------------------------------------------------
#define END_AUTO_CLASS(CLASS)\
ASSEMBLE_PROPS()\
CLASS_ENDL()

//----------------------------------------------------------------------------------------------
#define CLASS_ENDL()\
	}\
	virtual const oes::rflex::Property_Base** GetProps() const { return m_pProps; }\
	virtual unsigned int GetSize() const { return m_nSize; }\
	virtual const oes::rflex::SInterfaceDecl** GetInterfaces() const { return m_pInterfaces; }\
	virtual unsigned int GetIntfSize() const { return m_nIntfSize; }\
private:\
	const oes::rflex::Property_Base **m_pProps;\
	unsigned int m_nSize;\
	const oes::rflex::SInterfaceDecl **m_pInterfaces;\
	unsigned int m_nIntfSize;\
};

//----------------------------------------------------------------------------------------------
#define CLASS_INSTANCE(CLASS, BASE_CLASS)\
namespace {\
volatile static oes::rflex::CAuto<CLASS, BASE_CLASS> ClassRegistration##CLASS(#CLASS,\
	Generator##CLASS,\
	CopyGenerator##CLASS,\
	CLASS::ThisClassName(),\
	CLASS::GetPanelBaseClassName());\
}

//----------------------------------------------------------------------------------------------
#define CLASS_INSTANCE_INTERFACE(CLASS)\
namespace\
{\
	volatile static oes::rflex::CAutoInterface<CLASS, AutoRegisterProps##CLASS> ClassRegistration##CLASS(#CLASS);\
}

//----------------------------------------------------------------------------------------------
#define CLASS_INSTANCE_EX(CLASS, BASE_CLASS)\
namespace\
{\
volatile static oes::rflex::CAutoEx<CLASS, BASE_CLASS, AutoRegisterProps##CLASS> ClassRegistration##CLASS(#CLASS,\
									 Generator##CLASS,\
									 CopyGenerator##CLASS,\
									 CLASS::ThisClassName(),\
									 CLASS::GetPanelBaseClassName());\
}

//----------------------------------------------------------------------------------------------
#define CLASS_INSTANCE_PURE(CLASS, BASE_CLASS)\
namespace\
{\
	volatile static oes::rflex::CAutoEx<CLASS, BASE_CLASS, AutoRegisterProps##CLASS> ClassRegistration##CLASS(#CLASS,\
	nullptr,\
	nullptr,\
	CLASS::ThisClassName(),\
	CLASS::GetPanelBaseClassName());\
}

//----------------------------------------------------------------------------------------------
#define CLASS_INSTANCE_PURE_NOBASE(CLASS)\
	namespace\
{\
	volatile static oes::rflex::CAutoPureEx<CLASS, void, AutoRegisterProps##CLASS> ClassRegistration##CLASS(#CLASS,\
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
		Object->SetTypeId(oes::rflex::GetId(#CLASS));\
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
	if (Transient == "0" || Transient.empty() || !External.empty())\
	{\
		TYPE_FAMILY_BASE *pNewObject = static_cast<TYPE_FAMILY_BASE*>(GENERATE_OBJECT_SDK(TYPE_NAME, ValueStore.GetValueString("Name").c_str(), PARENT));\
		if (pNewObject)\
		{\
			PARENT->AddChildNode(pNewObject);\
			if (External.empty())\
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
				PARENT->m_ChildNodes[Index]->SuperDeserializer(XML_CURRENT_NODE);\
				break;\
			}\
		}\
	}\
}

//---------------------------------------------------------------------------------------------
#define DECLARE_CLASS_SIMPLE( className, baseClassName ) \
public:\
	typedef baseClassName Super;\
	typedef baseClassName BaseClass;\
	typedef className ThisClass;\
	friend class AutoRegisterProps##className;\
public:\
	static char const* GetPanelClassName() { return #className; }\
	static char const* GetPanelBaseClassName() { return #baseClassName; }\
	static char const* ThisClassName() { return #className; }

#define DECLARE_CLASS_SIMPLE_NOBASE( className )\
	typedef className ThisClass;\
	friend class AutoRegisterProps##className;\
public:\
	static char const* GetPanelClassName() { return #className; }\
	static char const* GetPanelBaseClassName() { return nullptr; }\
	static char const* ThisClassName() { return #className; }

#define DECLARE_INTERFACE_CLASS( className )\
	typedef className ThisClass;\
	friend class AutoRegisterProps##className;\
public:\
	static char const* ThisClassName() { return #className; }\
	byte ___startObjectMarker##className;
