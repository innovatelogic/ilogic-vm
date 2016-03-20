#include "ClassTree.h"
#include "ClassNode.h"
#include "ClassNodeInterface.h"
#include "ClassNodeType.h"
//#include "ObjectAbstract.h"
//#include "Actor.h"

//----------------------------------------------------------------------------------------------
AppClassTree::AppClassTree()
: m_pRootNode(nullptr)
{
 	//m_pRootNode = new ClassNodeType<CObjectAbstract>("CObjectAbstract");
}

//----------------------------------------------------------------------------------------------
AppClassTree::AppClassTree(const AppClassTree & Source)
{

}

//----------------------------------------------------------------------------------------------
AppClassTree::~AppClassTree()
{
	Release();
}

//----------------------------------------------------------------------------------------------
bool AppClassTree::Add(const char * Type, const char * BaseType)
{
	ClassNode* Base = Find(BaseType);
	ClassNode* Siblin = Find(Type);

	if (Base && Siblin){
		return true;
	}

	if (!Base && !Siblin) // add plain
	{
		//     !Base
		//       |
		//    !Siblin 

		ClassNode* BaseClass = new ClassNode(BaseType);
		ClassNode* InheritClass = BaseClass->AddChild(new ClassNode(Type));

		m_pRootNode->AddChild(BaseClass);

		BaseClass->SetRootNode(m_pRootNode);
		InheritClass->SetRootNode(BaseClass);
	}
	else
	{
		if (!Base && Siblin)
		{
			//     Parent
			//       |
			//     !Base
			//       |
			//     Siblin 

			//allocate new base
			ClassNode* BaseClass = new ClassNode(BaseType);

			// connect siblings
			BaseClass->Childs.push_back(Siblin);

			// disconnect sibling
			ClassNode* ParentNode = RemoveNode(Siblin);

			ParentNode->Childs.push_back(BaseClass);

			BaseClass->SetRootNode(ParentNode);
			Siblin->SetRootNode(BaseClass);
		}
		else // Base && !Sibling
		{
			ClassNode* InheritClass = new ClassNode(Type);
			Base->AddChild(InheritClass);
			InheritClass->SetRootNode(Base);
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------
bool AppClassTree::AddPure(const char *Type, const Property_Base** Arr, int Count)
{
	ClassNode* InheritClass = Find(Type);

	if (!InheritClass)
	{
		InheritClass = new ClassNode(Type);
		m_pRootNode->AddChild(InheritClass);
		InheritClass->SetRootNode(m_pRootNode);
	}
	
	InheritClass->SetProprties(Arr, Count);

	return true;
}

//----------------------------------------------------------------------------------------------
void AppClassTree::AddInterface(ClassNode *pInterface)
{
	m_VecInterfaces.push_back(pInterface);
}

//----------------------------------------------------------------------------------------------
ClassNode* AppClassTree::Find(const char *type)
{
	if (m_pRootNode)
	{
		ClassNode *pFindNode = m_pRootNode->Find(type);

		if (pFindNode){
			return pFindNode;
		}
	}
	return nullptr;
}

//----------------------------------------------------------------------------------------------
ClassNode* AppClassTree::FindInterface(const char *type) const
{
	for (TVecClassNodeInterfaceConstIter Iter = m_VecInterfaces.begin(); Iter != m_VecInterfaces.end(); ++Iter)
	{
		if (!strcmp((*Iter)->GetName(), type))
		{
			return *Iter;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------
ClassNode* AppClassTree::RemoveNode(ClassNode *pNode)
{
	return (m_pRootNode) ? m_pRootNode->RemoveNode(pNode) : nullptr;
}

//----------------------------------------------------------------------------------------------
void AppClassTree::Migrate(AppClassTree * Other)
{
	// linear tree iteration
	std::vector<ClassNode*> VNodes;
	std::vector<ClassNode*> VNodesChilds;

	if (Other->m_pRootNode)
	{
		VNodes.push_back(Other->m_pRootNode);

		while (VNodes.size() > 0)
		{
			VNodesChilds.clear();

			for (size_t Index = 0; Index < VNodes.size(); ++Index)
			{
				for (size_t IndexChild = 0; IndexChild < VNodes[Index]->Childs.size(); ++IndexChild)
				{
					Add(VNodes[Index]->Name.c_str(), VNodes[Index]->Childs[IndexChild]->Name.c_str());
					VNodesChilds.push_back(VNodes[Index]->Childs[IndexChild]);
				}
			}

			VNodes = VNodesChilds;
		}
	}
}

//----------------------------------------------------------------------------------------------
void AppClassTree::Release()
{
	if (m_pRootNode)
	{
		m_pRootNode->Release();
		m_pRootNode = nullptr;
	}
}

//----------------------------------------------------------------------------------------------
bool AppClassTree::Add2(const char *type, const char *baseType)
{
    ClassNode *Base = Find(baseType);
    ClassNode *Siblin = Find(type);

    if (Base && Siblin) {
        return true;
    }

    if (!Base && !Siblin) // add plain
    {
        //     !Base
        //       |
        //    !Siblin 

        ClassNode* BaseClass = new ClassNode(BaseType);
        ClassNode* InheritClass = BaseClass->AddChild(new ClassNode(Type));

        m_pRootNode->AddChild(BaseClass);

        BaseClass->SetRootNode(m_pRootNode);
        InheritClass->SetRootNode(BaseClass);
    }
    else
    {
        if (!Base && Siblin)
        {
            //     Parent
            //       |
            //     !Base
            //       |
            //     Siblin 

            //allocate new base
            ClassNode* BaseClass = new ClassNode(BaseType);

            // connect siblings
            BaseClass->Childs.push_back(Siblin);

            // disconnect sibling
            ClassNode* ParentNode = RemoveNode(Siblin);

            ParentNode->Childs.push_back(BaseClass);

            BaseClass->SetRootNode(ParentNode);
            Siblin->SetRootNode(BaseClass);
        }
        else // Base && !Sibling
        {
            ClassNode* InheritClass = new ClassNode(Type);
            Base->AddChild(InheritClass);
            InheritClass->SetRootNode(Base);
        }
    }

    return true;
}