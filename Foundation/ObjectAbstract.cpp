#include "ObjectAbstract.h"
#include "MessageMap.h"

//AllocWindows*	GAllocMemory = NULL;

AllocWindows	AllocMemory;
//NObjectFactory::TClassFactory*	GClassFactory = NULL;
/*
#ifdef WIN32
#pragma data_seg("Shared")
AllocWindows*	GAllocMemory = &AllocMemory;
#pragma data_seg()
#pragma comment(linker, "/Section:Shared,RWS")
#else
AllocWindows*	GAllocMemory;
#endif

//
// C++ style memory allocation.
//
	long AllocNum = 0;

EXTERN_EXPORT void* __cdecl memory_new(size_t Size)
{
	_CrtSetBreakAlloc(197);

	if (AllocNum == 197 || AllocNum == 784)
	{
		int k = 0;
	}
	AllocNum++;
	void* pAlloc = GAllocMemory->Malloc(Size);

 	if (sizeof(pAlloc) == 512)
 	{
 		int k = 0;
 	}
	return pAlloc;
}

EXTERN_EXPORT void __cdecl memory_delete(void* Ptr)
{
	GAllocMemory->Free(Ptr);
}*/

// void* operator new[]( size_t Size )
// {
// 	return GAllocMemory->Malloc(Size);
// }
// 
// void operator delete[]( void* Ptr )
// {
// 	GAllocMemory->Free(Ptr);
// }
void CObjectAbstract::DbgAllocCall()
{
 #if defined(DEBUG) | defined(_DEBUG)
 	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
 	//_CrtSetBreakAlloc(642801);
 #endif
}

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
extern "C"
{
   EXTERN_EXPORT CObjectAbstract* GENERATE_OBJECT_SDK(const char *Type, const char *Name, CObjectAbstract *pParent /*= NULL*/)
   {
		return static_cast<CObjectAbstract*>(NObjectFactory::GenObject(Type, Name, pParent));
   }

   //----------------------------------------------------------------------------------------------
   EXTERN_EXPORT CObjectAbstract * CLONE_OBJECT_SDK(const CObjectAbstract *pSource, const char* Name /*= NULL*/, CObjectAbstract *pNewParent /*= NULL*/)
   {
		CObjectAbstract * NewObject = static_cast<CObjectAbstract*>(NObjectFactory::CloneObject(pSource, pNewParent));
	
		if (Name){
			NewObject->SetName(Name);
		}

		return NewObject;
   }

   //----------------------------------------------------------------------------------------------
   EXTERN_EXPORT CObjectAbstract* SLICE_CLONE_OBJECT(const CObjectAbstract *pSource, const char *NewType, const char *Name /*= NULL*/)
   {
	   CObjectAbstract * NewObject = static_cast<CObjectAbstract*>(NObjectFactory::SliceCloneObject(pSource, NewType));

	   NewObject->SetType(NewType);
	   NewObject->SetTypeId(NObjectFactory::GetId(NewType));

	   if (Name){
		   NewObject->SetName(Name);
	   }
	   return NewObject;
   }

   //----------------------------------------------------------------------------------------------
   /*EXTERN_EXPORT NObjectFactory::TClassFactory* GetClassFactoryA()
   {
		return CObjectAbstract::GetClassFactoryStatic();
   }*/
}

//----------------------------------------------------------------------------------------------
// Properties
//----------------------------------------------------------------------------------------------
static const bool V_FALSE = false;

REGISTER_CLASS_PURE_NOBASE(CObjectAbstract)
	new PropertyBOOL("bExternal", (BYTE*)&((CObjectAbstract*)nullptr)->m_bExternal - (BYTE*)nullptr, "CObjectAbstract", "External", READ_ONLY, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_FALSE),
 	new PropertyString("FilenameTag", (BYTE*)&((CObjectAbstract*)nullptr)->m_FilenameTag - (BYTE*)nullptr, "CObjectAbstract", "External", READ_ONLY, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP),
END_REGISTER_CLASS_PURE_NOBASE(CObjectAbstract)

//----------------------------------------------------------------------------------------------
CObjectAbstract::CObjectAbstract(const CObjectAbstract *parent /*= NULL*/)
: m_bPendingToDelete(false)
//, m_ScriptObject(0)
, m_Type("")
, m_Name("")
, m_bGenerationFinished(0)
, m_bIsInitialize(false)
, m_bTransient(false)
, m_nPlainDeserializationPos(0)
, m_bExternal(V_FALSE)
, m_FilenameTag("")
, m_pUserData(0)
, m_pListener(0)
{

    static long g_uID = 0;
    m_uID = g_uID++;

	m_pArrayUserData[0] = m_pArrayUserData[1] = m_pArrayUserData[2] = nullptr;

	if (parent)
	{
		m_pUserData			= parent->m_pUserData;

		m_pArrayUserData[0] = parent->m_pArrayUserData[0];
		m_pArrayUserData[1] = parent->m_pArrayUserData[1];
		m_pArrayUserData[2] = parent->m_pArrayUserData[2];
	}
}

//----------------------------------------------------------------------------------------------
CObjectAbstract::CObjectAbstract(const CObjectAbstract &Source)
: m_bPendingToDelete(false)
, m_bIsInitialize(false)
, m_bGenerationFinished(0)
, m_nPlainDeserializationPos(0)
, m_pListener(0)
{  
	if (this != &Source)
	{
		m_TypeId	= Source.GetTypeId();
		m_Type		= Source.GetType();
		m_Name		= Source.m_Name;
		m_bTransient= Source.m_bTransient;
		m_bExternal	= Source.m_bExternal;
		m_FilenameTag = Source.m_FilenameTag;
		m_pUserData = Source.m_pUserData;

		m_pArrayUserData[0] = Source.m_pArrayUserData[0];
		m_pArrayUserData[1] = Source.m_pArrayUserData[1];
		m_pArrayUserData[2] = Source.m_pArrayUserData[2];
	}
}

//----------------------------------------------------------------------------------------------
CObjectAbstract::~CObjectAbstract()
{
}

//----------------------------------------------------------------------------------------------
void CObjectAbstract::FinishGeneration()
{
	assert(m_bGenerationFinished == 0);
#ifdef WIN32
	InterlockedExchangeAdd(&m_bGenerationFinished, 1);
#endif
}
//----------------------------------------------------------------------------------------------
bool CObjectAbstract::IsGenerationFinished()
{
	return m_bGenerationFinished != 0;
}

//----------------------------------------------------------------------------------------------
void CObjectAbstract::SuperDeserializer(tinyxml2::XMLElement *xml_current_tree)
{
	AppClassTree &CTree = NObjectFactory::GetClassTree();

	if (ClassNode *pCNode = CTree.Find(GetType()))
	{
		while (pCNode)
		{
			TVecPropertyConstIterator IterProp = pCNode->PropertyMap.begin();

			// deserialize embedded class props
			while (IterProp != pCNode->PropertyMap.end())
			{
				if ((*IterProp)->IsSerializable())
				{
                    std::string group = (*IterProp)->GetGroupName();

                    const char *attribute = xml_current_tree->Attribute(group.c_str());

					const std::string VALUE(attribute ? attribute : "");

					if ((*IterProp)->IsCommonValue())
					{
						(*IterProp)->SetProperty(this, VALUE.c_str(), 0, true);
					}
					else
					{
						ValueParser ValueStore(VALUE);

						std::string ValueName = (*IterProp)->GetName();

						if (ValueStore.IsValue(ValueName))
						{
							(*IterProp)->SetProperty(this, ValueStore.GetValueString(ValueName).c_str(), 0, true);
						}
					}
				}
				++IterProp;
			}

			// add interface properties
			ClassNode::TVecInterfaceIter IterIntf = pCNode->m_VecInterfaces.begin();

			while (IterIntf != pCNode->m_VecInterfaces.end())
			{
				ClassNode *pNodeInterface = CTree.FindInterface((*IterIntf)->strType);

				if (pNodeInterface)
				{
					TVecPropertyConstIterator IterIntfProp = pNodeInterface->PropertyMap.begin();

					// deserialize embedded class props
					while (IterIntfProp != pNodeInterface->PropertyMap.end())
					{
						if ((*IterIntfProp)->IsSerializable())
						{
							int MemoryOffsetOverride = 0;
							if ((*IterIntf)->byteShift != -1) // interface relative shift
							{ 
								MemoryOffsetOverride = (*IterIntf)->byteShift;
							}

							const std::string VALUE(xml_current_tree->Attribute((*IterIntfProp)->GetGroupName().c_str()));
						
							if ((*IterIntfProp)->IsCommonValue())
							{
								(*IterIntfProp)->SetProperty((byte*)this + MemoryOffsetOverride, VALUE.c_str(), 0, true);
							}
							else
							{
								ValueParser ValueStore(VALUE);

								std::string ValueName = (*IterIntfProp)->GetName();

								if (ValueStore.IsValue(ValueName))
								{
									(*IterIntfProp)->SetProperty((byte*)this + MemoryOffsetOverride, ValueStore.GetValueString(ValueName).c_str(), 0, true);
								}
							}
						}
						++IterIntfProp;
					}
				}
				++IterIntf;
			}

			pCNode = pCNode->GetRootNode();
		}

		SuperDeserializerInternal(xml_current_tree);
	}
}

//----------------------------------------------------------------------------------------------
void CObjectAbstract::SuperDeserializerExternal(tinyxml2::XMLElement *xml_current_tree)
{
	AppClassTree &CTree = NObjectFactory::GetClassTree();

	if (ClassNode *pCNode = CTree.Find(GetType()))
	{
		while (pCNode)
		{
			TVecPropertyConstIterator IterProp = pCNode->PropertyMap.begin();

			// deserialize embedded class props
			while (IterProp != pCNode->PropertyMap.end())
			{
				if ((*IterProp)->IsSerializable())
				{
					const std::string VALUE(xml_current_tree->Attribute((*IterProp)->GetGroupName().c_str()));

					if ((*IterProp)->IsExternalProp())
					{
						if ((*IterProp)->IsCommonValue())
						{
							(*IterProp)->SetProperty(this, VALUE.c_str(), 0, true);
						}
						else
						{
							ValueParser ValueStore(VALUE);

							std::string ValueName = (*IterProp)->GetName();

							if (ValueStore.IsValue(ValueName))
							{
								(*IterProp)->SetProperty(this, ValueStore.GetValueString(ValueName).c_str(), 0, true);
							}
						}
					}
				}
				++IterProp;
			}

			// add interface properties
			ClassNode::TVecInterfaceIter IterIntf = pCNode->m_VecInterfaces.begin();

			while (IterIntf != pCNode->m_VecInterfaces.end())
			{
				ClassNode *pNodeInterface = CTree.FindInterface((*IterIntf)->strType);

				if (pNodeInterface)
				{
					TVecPropertyConstIterator IterIntfProp = pNodeInterface->PropertyMap.begin();

					// deserialize embedded class props
					while (IterIntfProp != pNodeInterface->PropertyMap.end())
					{
						if ((*IterIntfProp)->IsExternalProp())
						{
							if ((*IterIntfProp)->IsSerializable())
							{
								int MemoryOffsetOverride = 0;
								if ((*IterIntf)->byteShift != -1) // interface relative shift
								{ 
									MemoryOffsetOverride = (*IterIntf)->byteShift;
								}

								const std::string VALUE(xml_current_tree->Attribute((*IterIntfProp)->GetGroupName().c_str()));

								if ((*IterIntfProp)->IsCommonValue())
								{
									(*IterIntfProp)->SetProperty((byte*)this + MemoryOffsetOverride, VALUE.c_str(), 0, true);
								}
								else
								{
									ValueParser ValueStore(VALUE);

									std::string ValueName = (*IterIntfProp)->GetName();

									if (ValueStore.IsValue(ValueName))
									{
										(*IterIntfProp)->SetProperty((byte*)this + MemoryOffsetOverride, ValueStore.GetValueString(ValueName).c_str(), 0, true);
									}
								}
							}
						}
						++IterIntfProp;
					}
				}
				++IterIntf;
			}

			pCNode = pCNode->GetRootNode();
		}
	}
}

//----------------------------------------------------------------------------------------------
bool IsNestedBy(ClassNode *pNode, const char *pTypeBase)
{
	bool bResult = false;

	if (pTypeBase)
	{
		AppClassTree &CTree = NObjectFactory::GetClassTree();

		ClassNode *pBaseType = CTree.Find(pTypeBase);

		assert(pBaseType && pNode);

		while (pNode)
		{
			if (pNode == pBaseType)
			{
				bResult = true;
				break;
			}
			pNode = pNode->GetRootNode();
		}
	}
	
	return bResult;
}


//----------------------------------------------------------------------------------------------
std::string& CObjectAbstract::GetValueString(std::string &OutValue, bool bSaveAsExternal /*= false*/) const
{
	typedef std::pair<std::string, std::string> TPairString;

	AppClassTree &CTree = NObjectFactory::GetClassTree();

	std::map<std::string, std::string> ValueMap;

	ClassNode *pOwnClassNode = CTree.Find(GetType());

	if (pOwnClassNode)
	{
		// prepare storage holder

		// Default block name
		ValueMap.insert(TPairString("Value", std::string("Name:") + GetName() + " "));

		if (GetExternal()){
			ValueMap.insert(TPairString("External", GetFilenameTag()));
		}

		// write down groups map
		ClassNode *pClassNode = pOwnClassNode;

		while (pClassNode)
		{
			// general properties
			const ClassNode::TVecProperties &VecProps = pClassNode->GetProperties();
			ClassNode::TVecPropertyConstIterator IterProp = VecProps.begin();

			while (IterProp != VecProps.end())
			{
				if (ValueMap.find((*IterProp)->GetGroupName()) == ValueMap.end())
				{
					ValueMap.insert(TPairString((*IterProp)->GetGroupName(), ""));
				}
				++IterProp;
			}

			// Interfaces
			const ClassNode::TVecInterfaces &VecInterfaces = pClassNode->GetInterfaceProps();
			ClassNode::TVecInterfaceConstIter IterIntf = VecInterfaces.begin();

			while (IterIntf != VecInterfaces.end())
			{
				ClassNode *pInfClass = CTree.FindInterface((*IterIntf)->strType);

				if (pInfClass)
				{
					const ClassNode::TVecProperties &IntfPropArray = pInfClass->GetProperties();
					ClassNode::TVecPropertyConstIterator &IterPropIntf = IntfPropArray.begin();

					while (IterPropIntf != IntfPropArray.end())
					{
						if (ValueMap.find((*IterPropIntf)->GetGroupName()) == ValueMap.end())
						{
							ValueMap.insert(TPairString((*IterPropIntf)->GetGroupName(), ""));
						}
						++IterPropIntf;
					}
				}
				++IterIntf;
			}
			pClassNode = pClassNode->GetRootNode();
		}

		// write down values by group name
		std::map<std::string, std::string>::iterator IterGroup = ValueMap.begin();

		while (IterGroup != ValueMap.end())
		{
			if (IterGroup->first == "External"){ // already added skip
				IterGroup++;
			}

			ClassNode *pClassNode = pOwnClassNode;

			// write 
			while (pClassNode)
			{
				const ClassNode::TVecProperties &PropsArray = pClassNode->GetProperties();
				ClassNode::TVecPropertyConstIterator IterProp = PropsArray.begin();

				while (IterProp != PropsArray.end())
				{
					if (!bSaveAsExternal && (GetExternal() && !(*IterProp)->IsExternalProp()))
					{
						++IterProp;
						continue;
					}

					if ((*IterProp)->IsSerializable() && IterGroup->first == (*IterProp)->GetGroupName())
					{
						char Buff[1024] = {0};
						char BuffDefault[1024] = {0};

						(*IterProp)->GetProperty((BYTE*)this, Buff);

						if ((*IterProp)->GetDefaultValue(BuffDefault)) // check default value skip if necessary
						{
							if (!strcmp(Buff, BuffDefault))
							{
								++IterProp;
								continue;
							}
						}

						if (!(*IterProp)->IsCommonValue()) // common strategy format [Name:Value]
						{
							IterGroup->second += std::string((*IterProp)->GetName()) + ":" + std::string(Buff) + " ";
						}
						else // uninterrupted values purposes (string etc.)
						{
							IterGroup->second += Buff;
						}
					}
					++IterProp;
				}

				//Interface Properties
				const ClassNode::TVecInterfaces &VecInterfaces = pClassNode->GetInterfaceProps();
				ClassNode::TVecInterfaceConstIter IterIntf = VecInterfaces.begin();

				while (IterIntf != VecInterfaces.end())
				{
					ClassNode *pInfClass = CTree.FindInterface((*IterIntf)->strType);

					if (pInfClass)
					{
						const ClassNode::TVecProperties &IntfPropArray = pInfClass->GetProperties();
						ClassNode::TVecPropertyConstIterator &IterPropIntf = IntfPropArray.begin();

						while (IterPropIntf != IntfPropArray.end())
						{
							if ((*IterPropIntf)->IsSerializable() && IterGroup->first == (*IterPropIntf)->GetGroupName())
							{
								char Buff[1024] = {0};
								char BuffDefault[1024] = {0};

								int MemoryOffsetOverride = 0;
								{
									SInterfaceDecl* pIntfDecl = pClassNode->GetInterfaceDecl((*IterPropIntf)->GetClassName());
									if (pIntfDecl){
										MemoryOffsetOverride = pIntfDecl->byteShift;
									}
								}

								(*IterPropIntf)->GetProperty((BYTE*)this + MemoryOffsetOverride, Buff);

								if ((*IterPropIntf)->GetDefaultValue(BuffDefault)) // check default value skip if necessary
								{
									if (!strcmp(Buff, BuffDefault))
									{
										++IterPropIntf;
										continue;
									}
								}

								if (!(*IterPropIntf)->IsCommonValue()) // common strategy format [Name:Value]
								{
									IterGroup->second += std::string((*IterPropIntf)->GetName()) + ":" + std::string(Buff) + " ";
								}
								else // uninterrupted values purposes (string etc.)
								{
									IterGroup->second += Buff;
								}
							}
							++IterPropIntf;
						}
					}
					++IterIntf;
				}
				pClassNode = pClassNode->GetRootNode();
			}
			++IterGroup;
		}

		// write down transient property
		char txt[128] = {'\0'};
		sprintf(txt, " Transient=\"%i\" ", int(IsTransient()));

		OutValue = txt;//" Transient=\"" + boost::lexical_cast<std::string>(int(IsTransient())) + "\" ";

		// put together all data
		IterGroup = ValueMap.begin();
		while (IterGroup != ValueMap.end())
		{
			if (!IterGroup->second.empty())
			{
				OutValue += IterGroup->first + std::string("=\"") + IterGroup->second + std::string("\" ");
			}
			++IterGroup;
		}
	}

	return OutValue;
}

//----------------------------------------------------------------------------------------------
void CObjectAbstract::Initialize()
{
	m_bIsInitialize = true;

	PostLink();
}

//----------------------------------------------------------------------------------------------
void CObjectAbstract::PostLink()
{
	TempLinkIndexes.clear();
}

//----------------------------------------------------------------------------------------------
/*NObjectFactory::TClassFactory * CObjectAbstract::GetClassFactory()
{
	return NObjectFactory::TGlobalClassFactory::GetInstance();
}*/

//----------------------------------------------------------------------------------------------
bool CObjectAbstract::IsNameEqual(const char *Value)
{
	return !strcmp(Value, m_Name.c_str());
}

//----------------------------------------------------------------------------------------------
void CObjectAbstract::SetUserData(void* user, unsigned int Index/* = INDEX_NONE*/) 
{
	if (Index == INDEX_NONE)
	{
		m_pUserData = user;
	}
	else
	{
		if (Index >= 0 && Index < 3)
		{
			m_pArrayUserData[Index] = user;
		}
	}
}

//----------------------------------------------------------------------------------------------
void* CObjectAbstract::GetUserData(unsigned int Index /*= INDEX_NONE*/)	const 
{
	void * ptrOut = 0;

	if (Index == INDEX_NONE)
	{
		ptrOut = m_pUserData;
	}
	else
	{
		if (Index >= 0 && Index < 3)
		{
			ptrOut = m_pArrayUserData[Index];
		}
	}
	return ptrOut;
}

// //----------------------------------------------------------------------------------------------
// void CObjectAbstract::SetScriptObject(luabind::adl::object *LuaObject)
// {
// 	m_ScriptObject = LuaObject; 
// }
//----------------------------------------------------------------------------------------------
// luabind::adl::object * CObjectAbstract::GetScriptObject()
// {
// 	if( m_ScriptObject == 0 )
// 	{
// 		m_ScriptObject = NScriptObjectCaster::Cast(m_TypeId,this);
// 	}
// 
// 	return m_ScriptObject;
// }

//----------------------------------------------------------------------------------------------
// ScriptDriver * CObjectAbstract::GetObjectScriptDriver()
// {
//    return NULL;
// }

//----------------------------------------------------------------------------------------------
void CObjectAbstract::Release()
{
	m_bPendingToDelete = true;
}

//----------------------------------------------------------------------------------------------
bool CObjectAbstract::ProcessRelease(bool ForceOperation/* = false*/)
{
	PreRelease();

	delete this;
	return true;
}

//----------------------------------------------------------------------------------------------
void CObjectAbstract::RegisterInterfaceImpl(IBaseInterface *pInt)
{
	if (std::find(m_VecInterfacesImp.begin(), m_VecInterfacesImp.end(), pInt) == m_VecInterfacesImp.end())
	{
		m_VecInterfacesImp.push_back(pInt);
	}
}

//----------------------------------------------------------------------------------------------
void CObjectAbstract::UnregisterInterfaceImpl(IBaseInterface *pInt)
{
	std::vector<IBaseInterface*>::iterator IterFind = std::find(m_VecInterfacesImp.begin(), m_VecInterfacesImp.end(), pInt);
	if (IterFind != m_VecInterfacesImp.end())
	{
		m_VecInterfacesImp.erase(IterFind);
	}
}

//----------------------------------------------------------------------------------------------
bool CObjectAbstract::RegisterRenderEntity(IRenderInterface *pPtr)
{
	bool bResult = false;

	if (std::find(m_VecRenderEntities.begin(), m_VecRenderEntities.end(), pPtr) == m_VecRenderEntities.end())
	{
		m_VecRenderEntities.push_back(pPtr);
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CObjectAbstract::UnregisterRenderEntity(IRenderInterface *pPtr)
{
	bool bResult = false;

	std::vector<IRenderInterface*>::iterator IterFind = std::find(m_VecRenderEntities.begin(), m_VecRenderEntities.end(), pPtr);
	if (IterFind != m_VecRenderEntities.end())
	{
		m_VecRenderEntities.erase(IterFind);
		bResult = true;
	}

	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CObjectAbstract::RegisterCollisionEntity(ICollisionInterface *pPtr)
{
	bool bResult = false;
	if (std::find(m_VecCollisionEntities.begin(), m_VecCollisionEntities.end(), pPtr) == m_VecCollisionEntities.end())
	{
		m_VecCollisionEntities.push_back(pPtr);
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CObjectAbstract::UnregisterCollisionEntity(ICollisionInterface *pPtr)
{
	bool bResult = false;
	
	std::vector<ICollisionInterface*>::iterator IterFind = std::find(m_VecCollisionEntities.begin(), m_VecCollisionEntities.end(), pPtr);
	if (IterFind != m_VecCollisionEntities.end())
	{
		m_VecCollisionEntities.erase(IterFind);
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
// 
// void CObjectAbstract::SetScriptObject(luabind::adl::object *LuaObject)
// {
// 	m_ScriptObject = LuaObject; 
// }
// //////////////////////////////////////////////////////////////////////////
// luabind::adl::object * CObjectAbstract::GetScriptObject()
// {
// 	if( m_ScriptObject == 0 )
// 	{
// 		m_ScriptObject = NScriptObjectCaster::Cast(m_TypeId,this);
// 	}
// 
// 	return m_ScriptObject;
// }