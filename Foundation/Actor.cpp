#include "Actor.h"
#include "ICollisionInterface.h"

static const bool V_TRUE = true;

REGISTER_CLASS_PURE(CActor, CObjectAbstract)
	new oes::rflex::PropertyBOOL("bEnabled",(BYTE*)&((CActor*)nullptr)->bEnabled - (BYTE*)nullptr, "CActor", "Value",	CTRL_COMBO,	READ_WRITE,	SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_TRUE),
END_REGISTER_CLASS_PURE(CActor, CObjectAbstract);

//----------------------------------------------------------------------------------------------
CActor::CActor(const CObjectAbstract *pParent /*= NULL*/)
: Super(pParent)
, m_pParentActor(const_cast<CActor*>(static_cast<const CActor*>(pParent))) // TODO: make more smart
, bEnabled(V_TRUE)
, bMarkerDeleteFlag(false)
, ControlEvent(SOEvent_None)
{
}

//----------------------------------------------------------------------------------------------
CActor::~CActor()
{	
	
}

//----------------------------------------------------------------------------------------------
bool CActor::OnPrePropertyChange(const char *pPropertyName)
{
	for (std::vector<IBaseInterface*>::iterator Iter = m_VecInterfacesImp.begin(); Iter != m_VecInterfacesImp.end(); ++Iter)
	{
		if (!(*Iter)->PrePropertyChangeIntf(pPropertyName))
		{
			return false;
		}
	}
	return Super::OnPrePropertyChange(pPropertyName);
}

//----------------------------------------------------------------------------------------------
void CActor::OnPropertyChanged(const char *pPropertyName)
{
	Super::OnPropertyChanged(pPropertyName);

	for (std::vector<IBaseInterface*>::iterator Iter = m_VecInterfacesImp.begin(); Iter != m_VecInterfacesImp.end(); ++Iter)
	{
		(*Iter)->OnPropertyChangedIntf(pPropertyName);
	}
}

//----------------------------------------------------------------------------------------------
void CActor::SuperDeserializer(tinyxml2::XMLElement *xml_current_tree)
{
	Super::SuperDeserializer(xml_current_tree);

    BroadcastEvent(Event_PostLoad);
    BroadcastEvent(Event_Updated);
}

//----------------------------------------------------------------------------------------------
void CActor::SuperSerializer(std::ofstream &stream, bool bSaveAsExternal /*= false*/)
{	
	SerializeImpl<std::ofstream>(stream, bSaveAsExternal);
}

//----------------------------------------------------------------------------------------------
void CActor::SuperSerializer(std::stringstream &stream, bool bSaveAsExternal /*= false*/)
{
	SerializeImpl<std::stringstream>(stream, bSaveAsExternal);
}

//----------------------------------------------------------------------------------------------
void CActor::FinishGeneration()
{
	Super::FinishGeneration();

	CActor *pAParent = GetParent();
	if (pAParent)
	{
		pAParent->BroadcastEvent(Event_ObjectGenerated, this);
	}

	TVecActorChildConstIterator Iterator = m_ChildNodes.begin();
	while (Iterator != m_ChildNodes.end())
	{
		if ((*Iterator)->IsTransient()){
			BroadcastEvent(Event_ObjectGenerated, (*Iterator));
		}
		++Iterator;
	}
}

//----------------------------------------------------------------------------------------------
void CActor::Initialize()
{
	assert(!IsInitialized());

	std::for_each(m_ChildNodes.begin(), m_ChildNodes.end(), std::mem_fun(&CActor::Initialize));
	
	Super::Initialize();

    BroadcastEvent(Event_PostInitialize);
}

//----------------------------------------------------------------------------------------------
void CActor::ResetInitialize()
{
	Super::ResetInitialize();

	std::for_each(m_ChildNodes.begin(),m_ChildNodes.end(), std::mem_fun(&CActor::ResetInitialize));
}

//----------------------------------------------------------------------------------------------
bool CActor::Rename(const char* name)
{
	bool bResult = false;
	if (strcmp(GetName(), name) != 0)
	{
		SetName(name);
        BroadcastEvent(Event_ObjectRename);
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CActor::IsKeyBind(KeyCode Key)
{
	bool bResult = false;

	TVecInterruptKeysIterator Iter = InterruptKeys.begin();
	while (Iter != InterruptKeys.end())
	{
		if (*Iter == Key)
		{
			bResult = true;
			break;
		}
		++Iter;
	}
	return bResult;
}
//----------------------------------------------------------------------------------------------
void CActor::AddKeyBinding(KeyCode Key)
{
	if (!IsKeyBind(Key)) // bind key only if not binded yet
	{
		InterruptKeys.push_back(Key);
		if (GetParent() != nullptr)
		{
			GetParent()->AddKeyBinding(Key);
		}
	}
}

//----------------------------------------------------------------------------------------------
void CActor::GetObjectsByClass(std::vector<const CActor*> &OutArray, std::string CLASS_TYPE /*= ""*/, bool bIterate /*= false*/) const
{
	TVecActorChildConstIterator Iterator = m_ChildNodes.begin();
	while (Iterator != m_ChildNodes.end())
	{
		if ((*Iterator)->GetType() == CLASS_TYPE){
			OutArray.push_back((*Iterator));
		}

		if (bIterate){
			(*Iterator)->GetObjectsByClass(OutArray, CLASS_TYPE, bIterate);
		}
		++Iterator;
	}
}

//----------------------------------------------------------------------------------------------
TVecActorChildIterator CActor::GetChildIteratorNext(const TVecActorChildIterator& Iterator, std::string CLASS_TYPE /*= ""*/)
{
	TVecActorChildIterator CopyIter(Iterator);

	if (CopyIter == m_ChildNodes.end()){
		CopyIter = m_ChildNodes.begin();
	}
	else
	{
		if (++CopyIter == m_ChildNodes.end()){
			CopyIter = m_ChildNodes.begin();
		}
	}

	if (m_ChildNodes.size() == 0){ // check
		return m_ChildNodes.end();
	}

	if (CLASS_TYPE != "")
	{
        TVecActorChildIterator BeginIter = CopyIter;
		
		while ((*CopyIter)->GetType() != CLASS_TYPE)
		{
			CopyIter = GetChildIteratorNext(CopyIter); 
			
			if (BeginIter == CopyIter){
				return m_ChildNodes.end(); // do not find return none
			}
		}
	}
	return CopyIter;
}

//----------------------------------------------------------------------------------------------
TVecActorChildIterator CActor::GetChildIteratorPrev(const TVecActorChildIterator& Iterator, std::string CLASS_TYPE /*= ""*/)
{
	TVecActorChildIterator CopyIter(Iterator);

	if (CopyIter == m_ChildNodes.begin()){
		CopyIter = m_ChildNodes.end();
	}

	if (m_ChildNodes.size() == 0){ // check
		return m_ChildNodes.end();
	}

	CopyIter--;

	if (CLASS_TYPE != "")
	{
		TVecActorChildIterator BeginIter = CopyIter;
		while ((*CopyIter)->GetType() != CLASS_TYPE)
		{
			CopyIter = GetChildIteratorPrev(CopyIter); 
			
			if (BeginIter == CopyIter){
				return m_ChildNodes.end(); // do not find return none
			}
		}
	}
	return CopyIter;
}
//----------------------------------------------------------------------------------------------
CActor* CActor::GetRootNode()
{
	CActor * IterActor = this;
	while (IterActor->GetParent())
	{
		IterActor = IterActor->GetParent();
	}
	return IterActor;
}

//----------------------------------------------------------------------------------------------
void CActor::Release()
{
	std::for_each(m_ChildNodes.begin(), m_ChildNodes.end(), std::mem_fun(&CActor::Release));

	bMarkerDeleteFlag = true;

	CActor * Parent = GetParent();
	while (Parent && !Parent->bMarkerDeleteFlag)
	{
		Parent->bMarkerDeleteFlag = true;
		Parent = Parent->GetParent();
	}

	Super::Release();
}

//----------------------------------------------------------------------------------------------
bool CActor::ReleaseChilds(bool bSkipTransient /*= false*/)
{
	if (!bSkipTransient)
	{
		std::for_each(m_ChildNodes.begin(), m_ChildNodes.end(), std::mem_fun(&CActor::Release));
	}
	else
	{
		for (TVecActorChildIterator Iter = m_ChildNodes.begin(); Iter != m_ChildNodes.end(); ++Iter)
		{
			if (!(*Iter)->IsTransient())
			{
				(*Iter)->Release();
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------------------------
bool CActor::IsNeedCollect()
{
	if (!IsInitialized() || !bMarkerDeleteFlag){ // do not check not fully initialized objects
		return false;
	}

	if (IsPendingToDelete()) // need to be deleted
	{
		return true;
	}
	else
	{
		TVecActorChildIterator IterActor = m_ChildNodes.begin();
		while (IterActor != m_ChildNodes.end())
		{
			if ((*IterActor)->IsNeedCollect()) // found pending to delete child object
			{
				return true;			
			}
			++IterActor;
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------
/** Warning: this function releases all depend childs & free self memory */
bool CActor::ProcessRelease(bool ForceOperation /*= false*/)
{	
	bool bReleaseObject = false;
	
	if (ForceOperation || IsPendingToDelete()) // need to be deleted
	{
		TVecActorChildIterator IterActor = m_ChildNodes.begin();
		while (IterActor != m_ChildNodes.end())
		{
			(*IterActor)->ProcessRelease(true);   // force release childs
			(*IterActor) = nullptr;				  // zero pointer
			++IterActor;
		}

		m_ChildNodes.erase(std::remove(m_ChildNodes.begin(), m_ChildNodes.end(), static_cast<CActor*>(nullptr)), m_ChildNodes.end());

		return Super::ProcessRelease(ForceOperation); // free memory
	}
	else
	{
		TVecActorChildIterator IterActor = m_ChildNodes.begin();
		while (IterActor != m_ChildNodes.end())
		{
			if ((*IterActor)->ProcessRelease(ForceOperation)) // found pending to delete child object
			{
				(*IterActor) = nullptr;
				bReleaseObject = true;
			}
			++IterActor;
		}
		
		if (bReleaseObject == true)
		{
			m_ChildNodes.erase(std::remove(m_ChildNodes.begin(), m_ChildNodes.end(), static_cast<CActor*>(nullptr)), m_ChildNodes.end());

            BroadcastEvent(Event_PostRelease);
            BroadcastEvent(Event_Updated);
		}
	}

	return false;
}

//----------------------------------------------------------------------------------------------
void CActor::PreRelease() 
{ 
    BroadcastEvent(Event_PreRelease);

	InterruptKeys.clear();

	Super::PreRelease();
}

//----------------------------------------------------------------------------------------------
void CActor::BroadcastEvent(ESystemEventID id)
{
    m_pEventManager->BroadcastEvent(id, this);
}

//----------------------------------------------------------------------------------------------
void CActor::BroadcastEvent(ESystemEventID id, const CActor *pParam)
{
    m_pEventManager->BroadcastEvent(id, pParam);
}

//----------------------------------------------------------------------------------------------
void CActor::SetParent(const CActor *parent)
{
	m_pParentActor = const_cast<CActor*>(parent);
}

//----------------------------------------------------------------------------------------------
bool CActor::AddChildNode(const CActor *actor, int Index /*= INDEX_NONE*/)
{
	bool bResult = false;

	if (actor != nullptr && std::find(m_ChildNodes.begin(), m_ChildNodes.end(), actor) == m_ChildNodes.end())
	{
		if (Index == INDEX_NONE)
		{
			m_ChildNodes.push_back(const_cast<CActor*>(actor));
		}
		else
		{
			TVecActorChildIterator IterPlaceTo = m_ChildNodes.begin();
			std::advance(IterPlaceTo, Index);

			if (IterPlaceTo != m_ChildNodes.end()){
				m_ChildNodes.insert(IterPlaceTo, const_cast<CActor*>(actor));
			}
			else{
				m_ChildNodes.push_back(const_cast<CActor*>(actor));
			}
		}

        BroadcastEvent(Event_AddObjectChild, actor);

		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool CActor::RemoveChildNode(CActor *actor)
{
	TVecActorChildIterator IterFind = std::find(m_ChildNodes.begin(), m_ChildNodes.end(), actor);
	if (IterFind != m_ChildNodes.end())
	{
        m_ChildNodes.erase(IterFind);
        BroadcastEvent(Event_OnRemoveObject, actor);
	}
	return true;
}

//----------------------------------------------------------------------------------------------
void CActor::ForeachNodes(int (*CALLBACK_ActorInvoke) (const CActor* Source)) const
{
	for_each(m_ChildNodes.begin(), m_ChildNodes.end(), CALLBACK_ActorInvoke);
}

//----------------------------------------------------------------------------------------------
void CActor::ForeachTreeNodes(int (*CALLBACK_ActorInvoke) (const CActor* Source)) const
{	
	if (CALLBACK_ActorInvoke(this) == 1) // call callback function with self as param
	{ 
		for_each(m_ChildNodes.begin(), m_ChildNodes.end(), std::bind2nd(std::mem_fun(&CActor::ForeachTreeNodes), CALLBACK_ActorInvoke));
	}
}

//----------------------------------------------------------------------------------------------
void CActor::ApplyModificators()
{
	for_each(m_ChildNodes.begin(), m_ChildNodes.end(), std::bind2nd(std::mem_fun(&CActor::ApplyModificator), this));
}

//----------------------------------------------------------------------------------------------
CActor * CActor::FindActor(const char *Name, bool bIterate /*= true*/) const
{
  	if (!strcmp(Name, GetName())) { // check self firstly
  		return const_cast<CActor*>(this);
  	}

	if (bIterate) // iterate in to the tree if bIterate flag set
	{
		TVecActorChildConstIterator Iter = m_ChildNodes.begin();

		while (Iter != m_ChildNodes.end())
		{
			CActor * FindActor = (*Iter)->FindActor(Name, bIterate);

			if (FindActor != nullptr){
				return FindActor;
			}
			++Iter;
		}
	}
	else
	{
// 		TVecActorChildConstIterator IterFind = std::find_if(ChildNodes.begin(), ChildNodes.end(), boost::bind<bool>(&CObjectAbstract::IsNameEqual, _1, Name.c_str()));
// 
// 		if (IterFind != ChildNodes.end()){
// 			return (*IterFind);
// 		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
CActor * CActor::FindActorByPath(const std::string &Path)
{
	CStringUtility<std::string> ParseString(Path, "/");

	CActor * IterActor = 0;

	for (size_t Index = 0; Index < ParseString.m_vector.size(); ++Index)
	{
		if (ParseString.m_vector[Index] == "..")
		{
			if (!IterActor)
			{
				CActor * NextIterActor = GetParent();
				IterActor = NextIterActor;
			}
			else
			{
				CActor * NextIterActor = IterActor->GetParent();
				IterActor = NextIterActor;
			}

			if (!IterActor){
				return 0;
			}
		}
		else
		{
			if (!IterActor)
			{
				CActor * NextIterActor = FindActor(ParseString.m_vector[Index].c_str(), false);
				IterActor = NextIterActor;
			}
			else
			{
				CActor * NextIterActor = IterActor->FindActor(ParseString.m_vector[Index].c_str(), false);
				IterActor = NextIterActor;
			}

			if (!IterActor){
				return 0;
			}
		}
	}	
	return IterActor;
}

//----------------------------------------------------------------------------------------------
void CActor::UpdateEntitiesChangeWorldPos(const Matrix &World)
{
	std::for_each(m_VecCollisionEntities.begin(), m_VecCollisionEntities.end(), std::bind2nd(std::mem_fun(&oes::common_base::ICollisionInterface::SetWorldPos), World));
}

//----------------------------------------------------------------------------------------------
int CActor::GetDepth()
{
	int OutValue = 0;

	const CActor * ObjectParent = this;

	while (ObjectParent)
	{
		OutValue++;
		ObjectParent = ObjectParent->GetParent();
	}

	return OutValue;
}
