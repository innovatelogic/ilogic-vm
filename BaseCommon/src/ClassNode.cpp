#include "Property.h"
#include "ClassNode.h"

//----------------------------------------------------------------------------------------------
ClassNode::ClassNode(const char * name)
: Name(name)
, m_pRootNode(0)
, m_pPropAlloc(0)
, m_pPropsRaw(0)
, m_PropsSize(0)
{
}

//----------------------------------------------------------------------------------------------
ClassNode::~ClassNode()
{
	if (m_pPropsRaw)
	{
		for (unsigned int Index = 0; Index < m_PropsSize; ++Index)
		{
			Property_Base * pNode = *(m_pPropsRaw + Index);

			/*Property_Base * pNodeNext = pNode->GetNext();
			while (pNodeNext)
			{
				Property_Base * pNextTo = pNodeNext->GetNext();
				delete pNodeNext;
				pNodeNext = pNextTo;
			}*/

			delete pNode;
		}
		delete [] m_pPropsRaw;
	}

	delete m_pPropAlloc;
}

//----------------------------------------------------------------------------------------------
void ClassNode::Release()
{
	for_each(Childs.begin(), Childs.end(), std::mem_fun(&ClassNode::Release));

	delete this;
}

//----------------------------------------------------------------------------------------------
ClassNode* ClassNode::Find(const char * Type)
{
	if (Name.compare(Type) == 0){
		return this;
	}
	else
	{
		for (TVecClassNodeIterator Iter = Childs.begin(); Iter != Childs.end(); ++Iter)
		{
			ClassNode * FindNode = (*Iter)->Find(Type);
			if (FindNode){
				return FindNode;
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
ClassNode* ClassNode::AddChild(ClassNode *pNode)
{
	// adds compatibility check
	Childs.push_back(pNode);
	return pNode;
}

//----------------------------------------------------------------------------------------------
ClassNode* ClassNode::RemoveNode(ClassNode * Node)
{
	TVecClassNodeIterator IterFind = std::find(Childs.begin(), Childs.end(), Node);

	if (IterFind != Childs.end())
	{
		Childs.erase(IterFind);
		return this;
	}

	for (TVecClassNodeIterator Iter = Childs.begin(); Iter != Childs.end(); ++Iter)
	{
		ClassNode *pParentToRemove = (*Iter)->RemoveNode(Node);

		if (pParentToRemove){
			return pParentToRemove;
		}
	}
	return nullptr;
}

//----------------------------------------------------------------------------------------------
Property_Base* ClassNode::GetProperty(const char *Name) const
{
	for (TVecPropertyConstIterator Iter = PropertyMap.begin(); Iter != PropertyMap.end(); ++Iter)
	{
		if (!strcmp((*Iter)->m_Name.c_str(), Name))
		{
			return *Iter;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
bool ClassNode::AddProperty(const char *Name, Property_Base *Prop)
{
	PropertyMap.push_back(Prop);
	return true;
}

//----------------------------------------------------------------------------------------------
void ClassNode::SetProprties(const IPropertiesAllocator *pPropAlloc)
{
	m_pPropAlloc = const_cast<IPropertiesAllocator*>(pPropAlloc);

	m_pPropsRaw = const_cast<Property_Base**>(pPropAlloc->GetProps());
	m_PropsSize = m_pPropAlloc->GetSize();
	
	for (unsigned int Index = 0; Index < m_PropsSize; ++Index)
	{
		PropertyMap.push_back(const_cast<Property_Base*>(*(m_pPropsRaw + Index)));
	}

	m_pInterfaces = const_cast<SInterfaceDecl**>(pPropAlloc->GetInterfaces());
	m_nIntfSize = m_pPropAlloc->GetIntfSize();

	for (unsigned int Index = 0; Index < m_nIntfSize; ++Index)
	{
		m_VecInterfaces.push_back(const_cast<SInterfaceDecl*>(*(m_pInterfaces + Index)));
	}
}

//----------------------------------------------------------------------------------------------
ClassNode* ClassNode::GetChild(char *type) const
{
    for each (auto item in Childs)
    {
        if (std::string(item->GetName()).compare(type) == 0)
        {
            return item;
        }
    }
    return nullptr;
}

//----------------------------------------------------------------------------------------------
void ClassNode::SetProprties(const Property_Base** Arr, int Count)
{
	m_pPropsRaw = const_cast<Property_Base**>(Arr);
	m_PropsSize = (unsigned int)Count;

	for (unsigned int Index = 0; Index < m_PropsSize; ++Index)
	{
		PropertyMap.push_back(const_cast<Property_Base*>(*(Arr + Index)));
	}
}

//----------------------------------------------------------------------------------------------
bool ClassNode::PrePropertyChange(const void *prt, class Property_Base *Prop)
{
    //OwnerClass *pClass = const_cast<OwnerClass*>(reinterpret_cast<const OwnerClass*>(pPtr));

    return true; // pClass->OnPrePropertyChange(Prop->GetName());
}

//----------------------------------------------------------------------------------------------
void ClassNode::PostPropertyChange(const void * Prt, class Property_Base* Prop)
{

}

//----------------------------------------------------------------------------------------------
SInterfaceDecl* ClassNode::GetInterfaceDecl(const char *Name) const
{
	for (TVecInterfaces::const_iterator Iter = m_VecInterfaces.begin(); Iter != m_VecInterfaces.end(); ++Iter)
	{
		if (!strcmp((*Iter)->strType, Name))
		{
			return *Iter;
		}
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
bool ClassNode::IsInheritBaseClass(const char* pName) const
{
	bool bResult = false;

	if (!strcmp(Name.c_str(), pName)){
		return true;
	}

	ClassNode *pIter = m_pRootNode;

	while (pIter)
	{
		if (!strcmp(pIter->GetName(), pName))
		{
			bResult = true;
			break;
		}
		pIter = pIter->GetRootNode();
	}

	return bResult;
}