#include "coresdkafx.h"

//----------------------------------------------------------------------------------------------
void UpdateNode(TNodeMap<CObjectAbstract, IUpdateInterface>* pNode, float DeltaTime)
{
	const_cast<IUpdateInterface*>(pNode->m_pValue)->Update(DeltaTime);
	if (pNode->m_NumChilds > 0)
	{
		TNodeMap<CObjectAbstract, IUpdateInterface>* ChildNode = pNode->m_pNodeNext;
		for (size_t Index = 0; Index < pNode->m_NumChilds; ++Index)
		{
			UpdateNode(ChildNode, DeltaTime);
			ChildNode = ChildNode->m_pNodeNextPlain;
		}
	}
}

//----------------------------------------------------------------------------------------------
void UpdateNodePostPhys(TNodeMap<CObjectAbstract, IUpdateInterface>* pNode, float DeltaTime)
{
	const_cast<IUpdateInterface*>(pNode->m_pValue)->PostPhysUpdate(DeltaTime);
	if (pNode->m_NumChilds > 0)
	{
		TNodeMap<CObjectAbstract, IUpdateInterface>* ChildNode = pNode->m_pNodeNext;
		for (size_t Index = 0; Index < pNode->m_NumChilds; ++Index)
		{
			UpdateNodePostPhys(ChildNode, DeltaTime);
			ChildNode = ChildNode->m_pNodeNextPlain;
		}
	}
}
//----------------------------------------------------------------------------------------------
UpdateManager::UpdateManager(CCoreSDK* pSDK)
: m_pCoreSDK(pSDK)
{

}

//----------------------------------------------------------------------------------------------
UpdateManager::~UpdateManager()
{

}

//----------------------------------------------------------------------------------------------
bool UpdateManager::RegisterActor(CActor *pObject)
{
	bool bResult = false;
	TVecActorChildConstIterator Iter = std::find(VUpdateList.begin(), VUpdateList.end(), pObject);

	if (Iter == VUpdateList.end())
	{
		VUpdateList.push_back(pObject);
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
bool UpdateManager::UnRegisterActor(CActor * Object)
{
	bool bResult = false;
	TVecActorChildConstIterator Iter = std::find(VUpdateList.begin(), VUpdateList.end(), Object);

	if (Iter != VUpdateList.end())
	{
		VUpdateList.erase(Iter);
		bResult = true;
	}
	return bResult;
}

//----------------------------------------------------------------------------------------------
UpdateManager::TTNodeMap* UpdateManager::RegisterObject(const IUpdateInterface *Src,
											const CObjectAbstract *Key,
											const CObjectAbstract *KeyParent)
{
	return m_List.PushBack(Key, Src, KeyParent);
}

//----------------------------------------------------------------------------------------------
void UpdateManager::UnregisterObject(UpdateManager::TTNodeMap* pNode)
{
	m_List.Erase(pNode);
}

//----------------------------------------------------------------------------------------------
void UpdateManager::Update(float DeltaTime)
{
	TNodeMap<CObjectAbstract, IUpdateInterface> *pNode = m_List.m_pFirstElement;

	while (pNode)
	{
		UpdateNode(pNode, DeltaTime);
		pNode = pNode->m_pNodeNextPlain;
	}
}

//----------------------------------------------------------------------------------------------
void UpdateManager::PostPhysUpdate(float DeltaTime)
{
	TNodeMap<CObjectAbstract, IUpdateInterface> *pNode = m_List.m_pFirstElement;

	while (pNode)
	{
		UpdateNodePostPhys(pNode, DeltaTime);
		pNode = pNode->m_pNodeNextPlain;
	}
}

//----------------------------------------------------------------------------------------------
void UpdateManager::PostPhysEvent(CActor *pObject, EPhysEventCallback EVENT_ID, void *pParamA, void *pParamB)
{
	// find in view port array
	if (TNodeMap<CObjectAbstract, IUpdateInterface> *pVNode = m_List.FindNodeByKey(pObject))
	{
		const_cast<IUpdateInterface*>(pVNode->m_pValue)->PostPhysEvent(EVENT_ID, pParamA, pParamB);
	}
}